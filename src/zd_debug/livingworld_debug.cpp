// Living-world debug state m dropped (README item 11).
//
// LivingWorldAssetDebugger (the world painter / nav-mesh debug views,
// kWorldPainterDebugger*) is LivingWorldPresentation+25228 in zd and +25360
// in m. m's object keeps only the vtable and the world-painter tile map; zd's
// has a fixed_map<cAssetID, CharacterEntity*, 128> before the tile map (zd
// +4..+4167), which zdx keeps in extension block 18 (zdx_config). zd's
// DebugDraw clears that map every frame the debugger is off: a zero-filled
// block is not an empty eastl map (its anchor does not point to itself) and
// the clear loop never ends. The block is constructed here, with m's
// constructor of the same map type, when m constructs the presentation.

#include "zd_debug.h"
#include "zdx.h"
#include "generated/sk83_na_m_init.h"

#include <rex/logging.h>

namespace {
constexpr uint32_t kAssetDebuggerOffset = 25360;  // m LivingWorldPresentation member
constexpr uint32_t kCharacterMapExt = 18;         // zdx_config EXT[18]
}  // namespace

// Sk8::LivingWorldPresentation::LivingWorldPresentation() (m 0x8294F860).
extern "C" REX_FUNC(sub_8294F860) {
  const uint32_t lwp = ctx.r3.u32;
  __imp__sub_8294F860(ctx, base);
  if (!zdx::PoolReady(base) || !lwp) return;  // debug map must have a valid empty state
  const uint32_t map = zdx::Ext(lwp + kAssetDebuggerOffset, kCharacterMapExt);
  if (!map) return;
  zdx::GuestCall call(ctx, base);
  call.c.r3.u64 = map;
  call(rex_f0_fixed_map_I_K_0IA_0A_U_less_I_eastl_Vallocator_2_eastl_QAA_XZ_82948488);
  REXLOG_INFO("zd_debug: LivingWorldAssetDebugger {:08X}: character map constructed", lwp + kAssetDebuggerOffset);
}
