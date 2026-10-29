// Create-a-skater camera shadow-settings override: a retail cleanup this
// development build lacks.
//
// Character::CasCamera::AssumeControl / SetTargetState install their shadow
// settings as an override on RenderMan+32 (Attrib::Instance, class
// 0x1FFDC8E3ACA07C1F); RenderMan's per-frame shadow-settings update
// (0x82997658) copies that override into the active settings (+16) for as
// long as it is set, so the world's cascades are fitted with the menu
// scene's settings (1.8x scale, zero depth offset): the skater/NPC shadows
// vanish (original hardware included) and the native cascade loses the
// player.
//
// Retail clears the override (FindCollection(class, -1)) right after the
// front-end loose-world unloads of the trick guide and the character viewer
// (see the UnloadLooseWorld hook below); this build's counterparts unload
// without clearing it, so leaving the smartphone skater menus (Freeskate /
// Skate With character select = the character viewer) kept the override for
// good. Ported here: the override is cleared exactly as
// Character::CasCamera::RelinquishControl (0x82698A90) does it.

#include "generated/sk83_na_m_init.h"

#include <rex/hook.h>
#include <rex/logging.h>

#include <cstdint>
#include <cstring>
#include <stdlib.h>

namespace {

uint32_t LoadU32(uint8_t* base, uint32_t addr) {
  uint32_t v;
  std::memcpy(&v, REX_RAW_ADDR(addr), 4);
  return _byteswap_ulong(v);
}

constexpr uint64_t kShadowSettingsClass = 0x1FFDC8E3ACA07C1FULL;

uint32_t RenderMan(uint8_t* base) { return LoadU32(base, 0x83368E68); }

// Attrib::Instance::Change(RenderMan + 32, FindCollection(class, -1)), the
// sequence CasCamera::RelinquishControl and retail's loose-world unloads use.
void ClearCasShadowOverride(PPCContext& ctx, uint8_t* base) {
  const uint32_t rm = RenderMan(base);
  if (rm == 0) {
    return;
  }
  PPCContext call = ctx;
  call.r3.u64 = kShadowSettingsClass;
  call.r4.s64 = -1;
  rex_Attrib_FindCollection_YAPBVCollection_1_K0_Z_82D76E28(call, base);
  call.r4.u64 = call.r3.u64;
  call.r3.u64 = rm + 32;
  rex_Attrib_Instance_Change_QAAXPBVCollection_2_Z_82D779B0(call, base);
}

}  // namespace

// Sk8::LooseWorldPresentation::UnloadLooseWorld (0x82911228). Retail clears
// the override right after two of its three loose-world unloads:
//   0x82618CA8 FrontEndState_TrickGuide::OnLeave  -> this build 0x82760120
//   0x827064D8 character-viewer exit (case 1)     -> this build 0x82874990
// (the third, the CaC exit 0x82705DE8 / this build 0x8278A180, has none:
// CASPresManager's shutdown state relinquishes the camera there). This
// build's two counterparts unload but never clear it; the character viewer
// is what the smartphone Freeskate / Skate With character select runs.
// The call sites are identified by their return addresses.
extern "C" REX_FUNC(rex_Sk8_LooseWorldPresentation_UnloadLooseWorld_QAAXXZ_82911228) {
  const uint32_t ret = uint32_t(ctx.lr);
  __imp__rex_Sk8_LooseWorldPresentation_UnloadLooseWorld_QAAXXZ_82911228(ctx, base);
  const char* site = ret == 0x82760140u   ? "TrickGuide OnLeave"
                     : ret == 0x828749FCu ? "character viewer exit"
                                          : nullptr;
  if (site == nullptr) {
    return;
  }
  const uint32_t rm = RenderMan(base);
  const uint32_t before = rm != 0 ? LoadU32(base, rm + 32) : 0;
  ClearCasShadowOverride(ctx, base);
  REXLOG_INFO("cas-camera: {} unloaded the loose world - shadow-settings override {:08X} -> "
              "{:08X} (retail cleanup)",
              site, before, rm != 0 ? LoadU32(base, rm + 32) : 0);
}

// Sk8::LooseWorldPresentation::LoadLooseWorld (0x8291A510). For the trick
// guide (type 1) this build adds "data\content/world/models/
// DIST_trickguide[_Textures].rx2", but miscload.big stores the pair as
// DIST_TrickGuide[_Textures].rx2 and the archive lookup is case-sensitive:
// the CaS pair (type 2, same archive and directory, matching case) resolves
// from the archive, while the trick guide pair misses it, falls through to a
// loose-file probe that fails, and the assets never register. The unload in
// FrontEndState_TrickGuide::OnLeave then deregisters an asset the ref counter
// never held and assman::cAssetRefCounter::RemAsset asserts
// (assetrefcounter.h:41; original hardware too). Retail requests
// "DIST_TrickGuide[_Textures].rx2"; the two literals (0x821D6234 /
// 0x821D626C, addressed by lis/addi in this function) are corrected to that
// spelling in place - same length - before the first load.
namespace {
void FixTrickGuideAssetNames(uint8_t* base) {
  static bool done = false;
  if (done) {
    return;
  }
  done = true;
  struct Literal {
    uint32_t addr;
    const char* expected;
  };
  static constexpr Literal kLiterals[] = {
      {0x821D6234u, "data\\content/world/models/DIST_trickguide_Textures.rx2"},
      {0x821D626Cu, "data\\content/world/models/DIST_trickguide.rx2"},
  };
  for (const Literal& lit : kLiterals) {
    const size_t len = std::strlen(lit.expected) + 1;
    char* p = reinterpret_cast<char*>(REX_RAW_ADDR(lit.addr));
    if (std::memcmp(p, lit.expected, len) != 0) {
      REXLOG_WARN("looseworld: trick guide asset name at {:08X} is not the expected literal - "
                  "left unchanged",
                  lit.addr);
      continue;
    }
    std::memcpy(std::strstr(p, "trickguide"), "TrickGuide", 10);
    REXLOG_INFO("looseworld: trick guide asset name at {:08X} -> '{}' (retail spelling)",
                lit.addr, p);
  }
}
}  // namespace

extern "C" REX_FUNC(rex_Sk8_LooseWorldPresentation_LoadLooseWorld_QAAXW4eWorldType_12_Z_8291A510) {
  FixTrickGuideAssetNames(base);
  __imp__rex_Sk8_LooseWorldPresentation_LoadLooseWorld_QAAXW4eWorldType_12_Z_8291A510(ctx, base);
}

#ifndef SK83_RELEASE_BUILD
// DIAGNOSTIC: LooseWorldPresentation::GetWorldRenderLocation (0x82908308),
// the render_locations collection RenderMan follows every frame (lighting
// data, SH default probe, and whether the cascade pass runs). Keys:
//   loose-world type 1 -> 0x6568F346'43066684, type 2 -> 0x664B0D04'8078FE31,
//   else the loose world's own instance (+17492), else the character viewer
//   (MVManager+68) -> 0xF17B6509'256EE58B, else the default 0xEC763034'105B866B.
// Logs each change so the create-a-skater location can be identified.
extern "C" REX_FUNC(rex_Sk8_LooseWorldPresentation_GetWorldRenderLocation_QBA_KXZ_82908308) {
  const uint32_t lwp = ctx.r3.u32;
  const uint32_t type = LoadU32(base, lwp + 17488);
  const uint32_t inst = LoadU32(base, lwp + 17492);
  __imp__rex_Sk8_LooseWorldPresentation_GetWorldRenderLocation_QBA_KXZ_82908308(ctx, base);
  static uint64_t s_last = 0;
  if (ctx.r3.u64 != s_last) {
    s_last = ctx.r3.u64;
    REXLOG_INFO("cas-camera: render location -> {:016X} (loose-world type {} instance {:08X})",
                ctx.r3.u64, type, inst);
  }
}

// DIAGNOSTIC: who installs the override (the front-end state behind the
// smartphone skater menus is named by the caller).
extern "C" REX_FUNC(rex_Character_CasCamera_AssumeControl_QAAXXZ_826989B0) {
  REXLOG_INFO("cas-camera: AssumeControl (lr {:08X})", uint32_t(ctx.lr));
  __imp__rex_Character_CasCamera_AssumeControl_QAAXXZ_826989B0(ctx, base);
}

extern "C" REX_FUNC(rex_Character_CasCamera_SetTargetState_QAAX_K_Z_826987F0) {
  REXLOG_INFO("cas-camera: SetTargetState (lr {:08X})", uint32_t(ctx.lr));
  __imp__rex_Character_CasCamera_SetTargetState_QAAX_K_Z_826987F0(ctx, base);
}

extern "C" REX_FUNC(rex_Character_CasCamera_RelinquishControl_QAAXXZ_82698A90) {
  REXLOG_INFO("cas-camera: RelinquishControl (lr {:08X})", uint32_t(ctx.lr));
  __imp__rex_Character_CasCamera_RelinquishControl_QAAXXZ_82698A90(ctx, base);
}

extern "C" REX_FUNC(rex_Sk8_FE_FrontEndState_CaC_OnLeave_UAAXXZ_8278A180) {
  __imp__rex_Sk8_FE_FrontEndState_CaC_OnLeave_UAAXXZ_8278A180(ctx, base);
  const uint32_t rm = RenderMan(base);
  REXLOG_INFO("cas-camera: CaC OnLeave - override now {:08X}", rm != 0 ? LoadU32(base, rm + 32) : 0);
}
#endif  // SK83_RELEASE_BUILD
