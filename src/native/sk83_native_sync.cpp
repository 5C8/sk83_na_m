// Host-side fast paths for guest synchronisation that the game takes many times per frame.
//
// Csis (EA's audio scripting interface) guards everything with ONE guest mutex, created unowned
// in Csis::System::Init (0x82a6f2d0) and kept at 0x8336ebe8. Lock / Unlock / Alloc / Free /
// SetMemberData / Function::Call / ... (all ten users found by scanning the generated code for the
// handle load) take it with WaitForSingleObject(h, INFINITE) and drop it with ReleaseMutex on the
// same thread. The audio update on the render thread does that 30-50 times a frame, and each pair
// was two kernel calls plus the object-table lookups (~3% of the render thread).
//
// Here that one handle maps to a host CRITICAL_SECTION (recursive, owner-tracked, no kernel call
// when uncontended); every other handle goes to the recompiled wrappers unchanged. The choice is
// latched on the first call so a cvar change can never split one acquisition across both paths.
#include "generated/sk83_na_m_init.h"

#include <rex/cvar.h>

#include <atomic>
#include <cstdint>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

REXCVAR_DEFINE_BOOL(sk83_native_csis_lock, true, "Skate 3",
                    "Take the audio library's (Csis) guest mutex with a host critical section "
                    "instead of kernel mutex calls (read once at the first lock)");

namespace {

constexpr uint32_t kCsisMutexHandle = 0x8336ebe8;  // HANDLE written by Csis::System::Init

CRITICAL_SECTION g_csis_cs;
std::atomic<int> g_csis_mode{-1};  // -1 undecided, 0 kernel mutex, 1 critical section
thread_local uint32_t t_csis_depth = 0;

bool CsisFast() {
  int m = g_csis_mode.load(std::memory_order_acquire);
  if (m < 0) {
    static const int decided = [] {
      const bool on = REXCVAR_GET(sk83_native_csis_lock);
      if (on) InitializeCriticalSectionAndSpinCount(&g_csis_cs, 2000);
      return on ? 1 : 0;
    }();
    g_csis_mode.store(decided, std::memory_order_release);
    m = decided;
  }
  return m == 1;
}

inline bool IsCsisHandle(uint32_t h, uint8_t* base) {
  return h != 0 && h == REX_LOAD_U32(kCsisMutexHandle) && CsisFast();
}

}  // namespace

// WaitForSingleObject(h, ms): XAPI wrapper, tail-calls WaitForSingleObjectEx(h, ms, FALSE).
extern "C" REX_FUNC(rex_WaitForSingleObject_82DAF6F8) {
  if (ctx.r4.u32 == 0xffffffffu && IsCsisHandle(ctx.r3.u32, base)) {
    EnterCriticalSection(&g_csis_cs);
    ++t_csis_depth;
    ctx.r5.u64 = 0;  // the wrapper's own `li r5,0`
    ctx.r3.u64 = 0;  // WAIT_OBJECT_0
    return;
  }
  __imp__rex_WaitForSingleObject_82DAF6F8(ctx, base);
}

// ReleaseMutex(h): TRUE on success; FALSE when the caller does not own it (as NtReleaseMutant).
extern "C" REX_FUNC(rex_ReleaseMutex_82DAF700) {
  if (IsCsisHandle(ctx.r3.u32, base)) {
    ctx.r4.u64 = 0;  // the wrapper's `li r4,0`
    if (t_csis_depth == 0) {
      ctx.r3.u64 = 0;
      return;
    }
    --t_csis_depth;
    LeaveCriticalSection(&g_csis_cs);
    ctx.r3.u64 = 1;
    return;
  }
  __imp__rex_ReleaseMutex_82DAF700(ctx, base);
}
