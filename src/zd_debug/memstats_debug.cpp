// Per-category memory statistics (XAllocator category records).
//
// Every memory category has a 108-byte record in XAllocator::Internal (the
// array at +49588 in m); XAllocator::GetCategoryInfo (m 82B5B218) returns
// record+40, and its readers -- MVMetrics::GatherMemStats (the marquee
// viewer's "[N Kb h, N Kb sb, N Kb gx, #allocs ...]" status lines), the arena
// stats table -- read three per-region counter blocks at record+60+16*region:
// {bytes, peak bytes, allocations, frees}. zd's category allocator
// (83B12580) and free (83B14CF8) keep them; m's (82B58CD0 / 82B58DB0) were
// compiled without that bookkeeping, so every reader showed 0. Restored here
// as zd does it: the block's real size (the heap's GetBlockSize, vtable +16),
// the region of the heap entry (+16).

#include <atomic>
#include <mutex>
#include <unordered_map>

#include "zd_debug.h"
#include "zdx.h"
#include "generated/sk83_na_m_init.h"

namespace {
struct Block {
  uint32_t record, region, size;
};
std::mutex g_mu;
std::unordered_map<uint32_t, Block> g_blocks;

void CallGuest(PPCContext& ctx, uint8_t* base, uint32_t target) { REX_CALL_INDIRECT_FUNC(target); }

uint32_t BlockSize(PPCContext& ctx, uint8_t* base, uint32_t heap, uint32_t ptr) {
  const uint32_t vt = heap ? REX_LOAD_U32(heap) : 0;
  const uint32_t fn = vt ? REX_LOAD_U32(vt + 16) : 0;  // GetBlockSize(const void*, bool)
  if (!fn) return 0;
  zdx::GuestCall call(ctx, base);
  call.c.r3.u64 = heap;
  call.c.r4.u64 = ptr;
  call.c.r5.u64 = 0;
  CallGuest(call.c, base, fn);
  return call.c.r3.u32;
}

void Add(uint8_t* base, uint32_t a, int32_t v) { REX_STORE_U32(a, REX_LOAD_U32(a) + uint32_t(v)); }
}  // namespace

// XAllocator::Internal category allocation (m 0x82B58CD0; zd 83B12580):
// r4 = category record, r5 = heap entry, r6 = size.
extern "C" REX_FUNC(sub_82B58CD0) {
  const uint32_t record = ctx.r4.u32, entry = ctx.r5.u32;
  __imp__sub_82B58CD0(ctx, base);
  const uint32_t ptr = ctx.r3.u32;
  if (!zd_debug::Enabled() || !ptr || !record || !entry) return;
  const uint32_t region = REX_LOAD_U32(entry + 16);
  if (region > 2) return;
  const uint32_t r3 = ctx.r3.u32;
  const uint32_t size = BlockSize(ctx, base, REX_LOAD_U32(entry), ptr);
  ctx.r3.u64 = r3;
  const uint32_t c = record + 60 + 16 * region;
  std::lock_guard<std::mutex> lock(g_mu);
  Add(base, c + 0, int32_t(size));
  Add(base, c + 8, 1);
  if (REX_LOAD_U32(c + 0) > REX_LOAD_U32(c + 4)) REX_STORE_U32(c + 4, REX_LOAD_U32(c + 0));
  g_blocks[ptr] = {record, region, size};
}

// XAllocator::Internal category free (m 0x82B58DB0; zd 83B14CF8):
// r4 = heap entry, r5 = block.
extern "C" REX_FUNC(sub_82B58DB0) {
  if (const uint32_t ptr = ctx.r5.u32) {
    std::lock_guard<std::mutex> lock(g_mu);
    auto it = g_blocks.find(ptr);
    if (it != g_blocks.end()) {
      const uint32_t c = it->second.record + 60 + 16 * it->second.region;
      Add(base, c + 0, -int32_t(it->second.size));
      Add(base, c + 12, 1);
      g_blocks.erase(it);
    }
  }
  __imp__sub_82B58DB0(ctx, base);
}

