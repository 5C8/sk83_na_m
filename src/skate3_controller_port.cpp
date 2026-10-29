// Locking the front-end controller port.
//
// On the retail boot path the "press START" screen (FrontEndState_BootFlow::ReceiveInput,
// 0x8275DF58) calls FEControllerManager::LockControllerPort, which copies the signed-in
// port (+24), or failing that the last port to press a front-end button (+32), into the
// locked port (+28). This build boots through the debug front end instead
// (StateDebugMenu -> DebugFEState), so that screen never runs and the locked port stays
// at its constructed value of -1 for the whole session.
//
// Nearly every reader of the locked port clamps a negative value to zero, so the missing
// lock goes unnoticed -- except in the return-to-frontend reset on leaving an online game
// (rex_Sk8_LivingWorld_Manager_InitLWMan_AAAXXZ_828492F0), which passes it to PlayersBackEnd::SetPlayerControllerIndex unclamped.
// The HUD is then rebuilt with controller index -1, and PlayerUI::UpdateSessionMarker ->
// Sk8AppMan::GetSimInput -> InputManager::GetInput asserts (inputman.h:68) and indexes one
// slot before the group, returning a cInputPad where an InputSK8 is expected. Dereferencing
// that is the crash when leaving a lobby.
//
// So do what the skipped boot screen would have done: the first time a port has actually
// pressed something in the front end, lock it, through the game's own LockControllerPort,
// and then stay out of the way.

#include "generated/sk83_na_m_init.h"

#include <cstdint>

namespace {

constexpr uint32_t kLockedPortOffset = 28;
constexpr uint32_t kLastPressedPortOffset = 32;

}  // namespace

// Sk8::FE::FEControllerManager::Update(this, deltaMs) -- records the port that pressed a
// front-end button in +32 and dispatches the press to the front-end state manager.
extern "C" REX_FUNC(rex_Sk8_FE_FEControllerManager_Update_QAAXI_Z_8274FE80) {
  // One shot: after the port is locked (by this or by the boot screen on a boot
  // flow that has one) there is nothing left to do for the rest of the session.
  static bool done = false;
  if (done) {
    __imp__rex_Sk8_FE_FEControllerManager_Update_QAAXI_Z_8274FE80(ctx, base);
    return;
  }

  const uint32_t manager = ctx.r3.u32;
  __imp__rex_Sk8_FE_FEControllerManager_Update_QAAXI_Z_8274FE80(ctx, base);
  if (manager == 0) {
    return;
  }

  if (static_cast<int32_t>(REX_LOAD_U32(manager + kLockedPortOffset)) >= 0) {
    done = true;  // the game locked it itself
    return;
  }
  if (static_cast<int32_t>(REX_LOAD_U32(manager + kLastPressedPortOffset)) < 0) {
    return;  // no port has pressed anything yet, nothing to lock to
  }

  const auto saved_r3 = ctx.r3;
  const auto saved_r11 = ctx.r11;
  const auto saved_cr6 = ctx.cr6;
  const uint64_t saved_lr = ctx.lr;

  ctx.r3.u32 = manager;
  rex_Sk8_FE_FEControllerManager_LockControllerPort_QAAXXZ_8273C4A0(ctx, base);

  ctx.r3 = saved_r3;
  ctx.r11 = saved_r11;
  ctx.cr6 = saved_cr6;
  ctx.lr = saved_lr;
  done = true;
}
