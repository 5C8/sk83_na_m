// zd DebugOverlay hookup. The overlay itself is sk83_na_zd's own code
// (Sk8::Debug::DebugOverlay draw API + DebugShapeRenderer, transplanted; see
// zdx.h). This file holds the m hooks that drive it:
//   - once per simulation step: zd-only cvars, shape lifetimes, the ported
//     features' per-frame code (zd_debug::OnFrame);
//   - the world debug-draw callback: zd's 3D shapes (the screen-space ones
//     are drawn from the debug HUD text pass, zdx_host.cpp).
//
// Feature cvar: zd_debug_overlay (needs the master zd_debug switch).

#pragma once

namespace rex::ui {
class ImGuiDrawer;
}

namespace zd_debug::overlay {

bool Active();  // zd_debug && zd_debug_overlay

// Kept for the app hook (sk83_na_m_app.h); nothing to set up any more.
void Create(rex::ui::ImGuiDrawer* drawer);

}  // namespace zd_debug::overlay
