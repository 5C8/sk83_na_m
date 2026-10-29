// Skater debug display (kShowSkaterDebug): sk83_na_zd's own chain, transplanted.
//
// zd's Sk8::SkaterManager::PostUpdate (0x8364D3A0) ends with
//     if ((flags & 2) && mLocalSkater && kShowSkaterDebug) mLocalSkater->DebugDraw();
// which sk83_na_m compiled out (m 0x827053D8 keeps the rest; kShowSkaterDebug
// is still in its menus). The virtual is Actor::DebugDraw (zd 0x8364A948,
// skater vtable slot 14, gone from m's vtable): skater state/intent text, and
// the physical player's DebugDraw (PhysicalPlayerHiLOD::DebugDraw 0x8416F7E8:
// physics state text, trajectories, up vectors...) -> Skeleton::DebugDraw ->
// UpdateDebugDisplay (kShowAnimationSkeleton, kShowPhysicsSkeleton,
// kDrawCentreOfMass, footprints, IK) + SkeletonBody::DebugDraw. All of it runs
// as zd's code (zdx_*, see zdx.h); the options are the game's own cvars (m's
// menus drive them) or zd-only ones (zd_k* cvars).
//
// As in zd, the skeleton views need kShowSkaterDebug on as well.

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include "generated/sk83_na_m_init.h"

namespace {
constexpr uint32_t kCvarShowSkaterDebug = 0x8335A9A8;  // m cvar object
constexpr uint32_t kLocalSkater = 328;                  // SkaterManager field (zd +368)
}  // namespace

// Sk8::SkaterManager::PostUpdate(SimulationFlags, SimulationFlags) (m 0x827053D8).
extern "C" REX_FUNC(rex_Sk8_SkaterManager_PostUpdate_QAAXW4SimulationFlags_2_0_Z_827053D8) {
  const uint32_t self = ctx.r3.u32;
  const uint32_t flags = ctx.r4.u32;
  __imp__rex_Sk8_SkaterManager_PostUpdate_QAAXW4SimulationFlags_2_0_Z_827053D8(ctx, base);
  if (!zd_debug::Enabled() || !(flags & 2)) return;
  const uint32_t skater = REX_LOAD_U32(self + kLocalSkater);
  if (!skater || !zd_debug::GameCvarBool(base, kCvarShowSkaterDebug)) return;
  zdx::GuestCall call(ctx, base);
  call.c.r3.u64 = skater;
  call(zdx_8364A948);
}

// Sk8::Physics::Skeleton::ProcessData(AnimOutPhysIn*, ProcessedPhysIn*,
// AnimOut*) (m 0x82E94200). zd's version also copied the toe/hand
// "_Reparented" bone matrices into Skeleton+16320..16575 (drawn as boxes by
// kShowAnimationSkeleton); m dropped the fields and the code. zd's block runs
// here as a transplanted fragment (zdx_frag_SkeletonBoneDebugMatrices) into
// the Skeleton's extension block, with the live-in registers zd's function
// had at that point.
extern "C" REX_FUNC(rex_Sk8_Physics_Skeleton_ProcessData_QAAXPAUAnimOutPhysIn_23_PAUProcessedPhysIn_23_PAUAnimOut_23_Z_82E94200) {
  const uint32_t skel = ctx.r3.u32, anim_in = ctx.r4.u32;
  __imp__rex_Sk8_Physics_Skeleton_ProcessData_QAAXPAUAnimOutPhysIn_23_PAUProcessedPhysIn_23_PAUAnimOut_23_Z_82E94200(ctx, base);
  if (!zd_debug::Enabled() || !skel || !anim_in) return;
  zdx::GuestCall call(ctx, base, 0x200);
  // zd's frame: the fragment uses r1+96..r1+151 as scratch (zeroed there).
  for (uint32_t k = 96; k < 160; k += 4) REX_STORE_U32(call.scratch - 0x100 + k, 0);  // r1 = scratch - 0x100
  // The fragment starts immediately before FastString::Assign, after zd
  // prepared its first destination and source. Never inherit m's return
  // registers: Assign writes through r3, which may be live physics output.
  call.c.r3.u64 = call.scratch - 0x100 + 128;
  call.c.r4.u64 = zdx::Map(0x8271D9ACu) + 2448;
  call.c.r31.u64 = skel;
  call.c.r15.u64 = anim_in;
  call.c.r16.u64 = zdx::Map(0x8271D9ACu);  // zd's string-table base register
  call.c.r26.u64 = 48;
  call.c.r27.u64 = 32;
  call.c.r28.u64 = 16;
  call.c.r30.u64 = 0;
  call(zdx_frag_SkeletonBoneDebugMatrices);
}
