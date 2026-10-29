// RenderMan debug display: sk83_na_zd's Sk8::Render::RenderMan::DebugRender.
//
// zd's DebugRender (0x839253B0) is m's (0x829ABC88) plus the blocks m compiled
// out: kbEnableBEStateName (Presentation::DisplayBEStateNames), the marquee
// viewer text (DisplayMarqueeViewerText), cImageOperateMan::DoTest, the sun
// light debug draw (SunLightMan::DebugDraw: sun direction / world map) and the
// print channel warnings (PrintChannelMan RenderWarnings). With zd_debug on
// the whole function runs as zd's code (zdx_839253B0) on m's RenderMan
// (field map: zdx_config.py STRUCTS["RenderMan"]).

#include <atomic>
#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include "generated/sk83_na_m_init.h"

// Sk8::Render::RenderMan::DebugRender(float, bool) (m 0x829ABC88).
extern "C" REX_FUNC(rex_Sk8_Render_RenderMan_DebugRender_AAAXM_N_Z_829ABC88) {
  if (!zd_debug::Enabled() || !zdx::Ready()) {  // m's own until in game
    __imp__rex_Sk8_Render_RenderMan_DebugRender_AAAXM_N_Z_829ABC88(ctx, base);
    return;
  }
  zdx::Run(ctx, base, zdx_839253B0);
}

// Sk8::Render::RenderMan::RenderStats() (m 0x829A2E18), called from
// RenderModeScene and DebugRender. zd's version (0x83917590) also has
// kbDrawCSM (shadow-map quads), kbDrawCaCSnapshot, kShowPauseManagerStates,
// kDrawRWACProfile, the arena stats and the debug graph.
extern "C" REX_FUNC(rex_Sk8_Render_RenderMan_RenderStats_AAAXXZ_829A2E18) {
  if (!zd_debug::Enabled() || !zdx::Ready()) {
    __imp__rex_Sk8_Render_RenderMan_RenderStats_AAAXXZ_829A2E18(ctx, base);
    return;
  }
  zdx::Run(ctx, base, zdx_83917590);
}
