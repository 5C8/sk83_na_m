// zd's DebugHeap size.
//
// Both builds route the "Debug" memory category (26: debug menus, the console
// var browsers, the marquee viewer's characters and animators, DebugStrings,
// graph channels, ...) to the DebugHeap. zd's heap table gives it 10 MB
// (0xA00000); m's gives it 3 MB, sized for m's own debug menus. With zd's debug
// features in, the heap sits ~220 KB from full: the marquee viewer's animator
// allocations fail (xcore general.cpp(129)), the assert stops the render thread
// and the viewer freezes on "loading animation db".
//
// The heap table (m 0x832D1CA8, 22 entries of {name, id, ?, region, size}) is
// read once, by xcore::GetNextCategorySetup, when XAllocator lays out its
// memory regions; each region is allocated from the sum of its heaps, so the
// larger DebugHeap (the last heap in region 1) just makes region 1 larger.

#include "zd_debug.h"
#include "generated/sk83_na_m_init.h"

#include <cstdlib>
#include <string>

#include <rex/cvar.h>
#include <rex/logging.h>

REXCVAR_DEFINE_INT32(zdx_debug_heap_mb, 10, "Skate 3 zd debug",
                     "DebugHeap size in MB (m 3, zd 10): debug menus and the marquee viewer's "
                     "character animators");
REXCVAR_DEFINE_STRING(zdx_heap_sizes, "", "Skate 3 zd debug",
                      "Heap sizes in MB by heap-table name, e.g. \"AnimACSHeap=32,CharGFX=200\" "
                      "(more marquee viewer characters; GFX heaps come out of ReservedGFX)");

namespace {
constexpr uint32_t kDebugHeapEntry = 0x832D1E4Cu;  // m heap table: DebugHeap
constexpr uint32_t kDebugHeapName = 0x82249D00u;   // "DebugHeap"
constexpr uint32_t kMDebugHeapSize = 0x300000u;
constexpr uint32_t kZdDebugHeapSize = 0xA00000u;   // zd 0x848122E0 entry
constexpr uint32_t kHeapTable = 0x832D1CA8u;       // 22 entries of 20 bytes
constexpr int kHeapCount = 22;
}  // namespace

// xcore::GetNextCategorySetup() (m 0x82A3FFA8): the heap table iterator.
extern "C" REX_FUNC(rex_xcore_GetNextCategorySetup_YAPAUtCategorySetup_1_XZ_82A3FFA8) {
  static bool done = false;
  if (!done) {
    done = true;
    // Runtime toggling needs the same debug capacity in either startup mode.
    if (REX_LOAD_U32(kDebugHeapEntry) == kDebugHeapName &&
        REX_LOAD_U32(kDebugHeapEntry + 16) == kMDebugHeapSize) {
      const uint32_t mb = uint32_t(REXCVAR_GET(zdx_debug_heap_mb));
      const uint32_t size = mb ? mb << 20 : kZdDebugHeapSize;
      REX_STORE_U32(kDebugHeapEntry + 16, size);
      REXLOG_INFO("zd_debug: DebugHeap {} KB -> {} KB", kMDebugHeapSize / 1024, size / 1024);
    }
    const std::string spec = REXCVAR_GET(zdx_heap_sizes);
    size_t pos = 0;
    while (zd_debug::Enabled() && pos < spec.size()) {
      size_t end = spec.find(',', pos);
      if (end == std::string::npos) end = spec.size();
      const std::string item = spec.substr(pos, end - pos);
      pos = end + 1;
      const size_t eq = item.find('=');
      if (eq == std::string::npos) continue;
      const std::string name = item.substr(0, eq);
      const uint32_t mb = uint32_t(std::atoi(item.c_str() + eq + 1));
      for (int i = 0; i < kHeapCount && mb; ++i) {
        const uint32_t e = kHeapTable + 20 * i;
        const uint32_t np = REX_LOAD_U32(e);
        if (!np || name != reinterpret_cast<const char*>(REX_RAW_ADDR(np))) continue;
        REXLOG_INFO("zd_debug: heap {} {} KB -> {} KB", name, REX_LOAD_U32(e + 16) / 1024, mb << 10);
        REX_STORE_U32(e + 16, mb << 20);
      }
    }
  }
  __imp__rex_xcore_GetNextCategorySetup_YAPAUtCategorySetup_1_XZ_82A3FFA8(ctx, base);
}
