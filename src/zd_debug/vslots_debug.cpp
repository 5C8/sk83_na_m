// Virtual debug draws m emptied (zdx_config VSLOTS).
//
// m kept these virtual DebugDraw methods' vtable slots but compiled their
// bodies out: the slots hold m's shared empty function (0x831F4B98). zd's
// functions are transplanted; their thunks go into m's vtable slots, so m's own
// virtual calls reach zd's drawing again. Each zd function checks its own
// options first, as in zd.
//
// Also m's StateMarqueeViewer flow state (a shell in m): zd's FilterImpl and
// OnLeaveState, and m's own shared OnEnterState (StateCAC's, 82851D18), so the
// Character Debug Menu's "Start Marquee Viewer" (menus_debug.cpp) works.

#include <atomic>
#include <iterator>

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include <rex/logging.h>

namespace {

constexpr uint32_t kEmptyFn = 0x831F4B98u;  // m: blr

struct VSlot {
  uint32_t vtable, slot;
  zdx::GuestFn fn;        // zd function (thunk), or null and m_fn
  const char* what;
  uint32_t expect = kEmptyFn;   // the slot must still hold this m function
  uint32_t m_fn = 0;            // an m function to put there instead of a zd one
};

// The slot's thunk target: zd's function, plus a one-time log of the first
// call (proof that m still makes the virtual call).
template <int I>
void Slot(PPCContext& ctx, uint8_t* base);

const VSlot kSlots[] = {
    {0x823F17F0u, 20, Slot<0>, "cChallengeSignUpManager debug draw (sign-up volumes / markers)"},
    {0x823F14B0u, 20, Slot<1>, "cChallengeSimManager debug draw (kShowLockedStreamerPosition)"},
    {0x823EAB30u, 12, Slot<2>, "PhysState_OffBoard_Pushing::DebugDraw"},
    {0x824556D0u, 7, Slot<3>, "RoadNetworkTargetIterator::DebugDraw"},
    {0x823F17E0u, 0, Slot<4>, "cChallengeDynamicHullManager debug draw (kShowLoadedDynamicHulls)"},
    {0x82422898u, 0, Slot<5>, "cChallengePreloadManager debug draw (kShowPreloadVolumes / NIS)"},
    // Flow::StateMarqueeViewer (m vtable 0x821ABCC8; zd 0x822BF0E8)
    {0x821ABCC8u, 4, Slot<6>, "StateMarqueeViewer::FilterImpl", 0x829D3FC0u},   // m: default (returns 0)
    {0x821ABCC8u, 7, Slot<7>, "StateMarqueeViewer::OnLeaveState"},
    // WaypointGroup::FullFPSDebugDraw (kDebugDrawWaypoint): m vtable 0x82445968 slot 26
    {0x824459D0u, 0, Slot<8>, "WaypointGroup::FullFPSDebugDraw"},
    // skater animatable's blend tree display (kDisplayBlendTree*): m vtable 0x8235EF10 slot 1
    {0x8235EF14u, 0, Slot<9>, "skater animatable blend tree display"},
    {0x821ABCC8u, 6, nullptr, "StateMarqueeViewer::OnEnterState (m's shared StateCAC one)", kEmptyFn, 0x82851D18u},
};
const zdx::GuestFn kZd[] = {zdx_840F5B38, zdx_840E0DF0, zdx_840CD510, zdx_8420C4A8, zdx_840E26A8,
                            zdx_8418F7E0, zdx_837D3600, zdx_837DA870, zdx_841F4C18,
                            zdx_83F11D08};
std::atomic<bool> g_called[std::size(kZd)];

template <int I>
void Slot(PPCContext& ctx, uint8_t* base) {
  if (!zd_debug::Enabled()) {
    if constexpr (I == 6) ctx.r3.u64=0; // m's default State::FilterImpl result
    return; // the other original slots were empty void methods
  }
  if (!g_called[I].exchange(true))
    REXLOG_INFO("zdx: first call of zd {} (this {:08X})", kSlots[I].what, ctx.r3.u32);
  kZd[I](ctx, base);
  if constexpr (I == 7) zdx::MarqueeStageEnd(ctx, base);  // StateMarqueeViewer::OnLeaveState
}

}  // namespace

namespace zdx {

void InstallVSlots(uint8_t* base) {
  static std::atomic<bool> done{false};
  if (!Ready() || done.exchange(true)) return;
  for (const VSlot& s : kSlots) {
    const uint32_t at = s.vtable + 4 * s.slot;
    const uint32_t old = REX_LOAD_U32(at);
    if (old != s.expect) {
      REXLOG_WARN("zdx: vtable {:08X}[{}] is {:08X}, not {:08X}: {} left alone",
                  s.vtable, s.slot, old, s.expect, s.what);
      continue;
    }
    const uint32_t thunk = s.fn ? FnThunk(s.fn) : s.m_fn;
    if (!thunk) continue;
    REX_STORE_U32(at, thunk);
    REXLOG_INFO("zdx: vtable {:08X}[{}] -> zd {} ({:08X})", s.vtable, s.slot, s.what,
                REX_LOAD_U32(at));
  }
}

}  // namespace zdx
