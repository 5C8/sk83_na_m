// zd DebugOverlay hookup (see overlay.h).

#include "overlay.h"

#include "generated/sk83_na_m_init.h"
#include "zd_debug.h"
#include "zdx.h"

REXCVAR_DEFINE_BOOL(zd_debug_overlay, true, "Skate 3 zd debug",
                    "Draw sk83_na_zd's DebugOverlay shapes and text (the debug draw every ported "
                    "debug view uses)");

namespace zd_debug::overlay {

bool Active() { return zd_debug::On(REXCVAR_GET(zd_debug_overlay)); }

void Create(rex::ui::ImGuiDrawer*) {}

}  // namespace zd_debug::overlay

// Sk8::SimController::Update2 (m 0x829F13F8): once per simulation step. zd
// ticks DebugOverlay::Update from Simulation::SimUpdate2 (zd 0x839735C0),
// which this calls; SimUpdate2 itself is only called from its own TU here, so
// the hook sits on the caller.
extern "C" REX_FUNC(rex_Sk8_SimController_Update2_QAA_AW4SimulationFlags_2_W432_Z_829F13F8) {
  __imp__rex_Sk8_SimController_Update2_QAA_AW4SimulationFlags_2_W432_Z_829F13F8(ctx, base);
  if (!zd_debug::Enabled()) return;
  zdx::BeginFrame(ctx, base);
  zdx::InstallVSlots(base);
  zdx::ApplyGraphChannels(ctx, base);
  zdx::RunMenuTests(ctx, base);
  zd_debug::OnFrame(base);
}

// Sk8::SkaterPresentation::DebugDraw (m 0x82912E28): the world debug-draw
// callback on the render thread (m's own cv_ShowRenderSkeleton draws here
// through SimpleDraw). zd's 3D shapes are drawn after it.
extern "C" REX_FUNC(rex_Sk8_SkaterPresentation_DebugDraw_QAAXXZ_82912E28) {
  __imp__rex_Sk8_SkaterPresentation_DebugDraw_QAAXXZ_82912E28(ctx, base);
  if (zd_debug::overlay::Active()) zdx::DrawShapes(ctx, base, false);
}
