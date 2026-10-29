// DIAGNOSTIC: dynamic shadows stop after the smartphone Freeskate / Skate
// With menus (emulated: skater/NPC shadows vanish; native: the cascade stops
// following the player). Reproduces on original hardware with this build.
//
// PostFX::RenderShadows_CSM (0x82985338) renders the cascades only while
//   tweak 0x83369034 is on, and
//   (RenderMan+180644 == 0 || tweak 0x83369248), and
//   (*(RenderMan+160) == 0 || byte *(RenderMan+164) + 2 != 0).
// RenderMan+180644 is the "scene mode" flag: Scene::Init sets it (0x829C8AC8
// unconditionally, 0x829CC918 when the scene's +165 option is on) together
// with world+31792 / world+31840; Scene::Purge (0x829BE5B8) clears it only
// when +165 is on and the ICE manager (0x832D3364) has no active take, and
// 0x829C1480 clears it unconditionally. A set without a matching clear
// leaves the cascades off for good.
//
// This logs every change of the CSM gate inputs (checked once per
// RenderShadows_CSM call) and every Scene init / purge / clear with the
// scene's option bytes, the ICE state and the flag before -> after. It does
// not change game behaviour.

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

uint8_t LoadU8(uint8_t* base, uint32_t addr) { return *REX_RAW_ADDR(addr); }

uint32_t RenderMan(uint8_t* base) { return LoadU32(base, 0x83368E68); }
uint32_t World(uint8_t* base) { return LoadU32(base, 0x83367290); }

// Tweak (Attrib-backed bool): value pointer at +208, else inline at +205.
int TweakBool(uint8_t* base, uint32_t tweak_global) {
  const uint32_t t = LoadU32(base, tweak_global);
  if (t == 0) {
    return -1;
  }
  const uint32_t p = LoadU32(base, t + 208);
  return p != 0 ? LoadU8(base, p) : LoadU8(base, t + 205);
}

// ICEManager take pointer (+28): nonzero = a camera take is bound.
uint32_t IceTake(uint8_t* base) {
  const uint32_t ice = LoadU32(base, 0x832D3364);
  return ice != 0 ? LoadU32(base, ice + 28) : 0;
}

struct GateState {
  int shadows_tweak = -2;
  int scene_mode = -2;
  int scene_tweak = -2;
  uint32_t view = 0xFFFFFFFFu;
  int view_flag = -2;
  int world_31792 = -2;
  int world_31840 = -2;
};
GateState g_gate;  // guest render thread

GateState ReadGate(uint8_t* base) {
  GateState s;
  const uint32_t rm = RenderMan(base);
  const uint32_t world = World(base);
  s.shadows_tweak = TweakBool(base, 0x83369034);
  s.scene_tweak = TweakBool(base, 0x83369248);
  if (rm != 0) {
    s.scene_mode = LoadU8(base, rm + 180644);
    s.view = LoadU32(base, rm + 160);
    const uint32_t vflags = LoadU32(base, rm + 164);
    s.view_flag = (s.view != 0 && vflags != 0) ? LoadU8(base, vflags + 2) : -1;
  }
  if (world != 0) {
    s.world_31792 = LoadU8(base, world + 31792);
    s.world_31840 = LoadU8(base, world + 31840);
  }
  return s;
}

void LogScene(const char* what, uint8_t* base, uint32_t scene, int before) {
  const uint32_t rm = RenderMan(base);
  const int after = rm != 0 ? LoadU8(base, rm + 180644) : -1;
  REXLOG_INFO(
      "shadow-gate: {} scene={:08X} opts[163..168]={}{}{}{}{}{} ice_take={:08X} scene_mode "
      "{} -> {}",
      what, scene, scene ? LoadU8(base, scene + 163) : 0, scene ? LoadU8(base, scene + 164) : 0,
      scene ? LoadU8(base, scene + 165) : 0, scene ? LoadU8(base, scene + 166) : 0,
      scene ? LoadU8(base, scene + 167) : 0, scene ? LoadU8(base, scene + 168) : 0,
      IceTake(base), before, after);
}

int SceneModeNow(uint8_t* base) {
  const uint32_t rm = RenderMan(base);
  return rm != 0 ? LoadU8(base, rm + 180644) : -1;
}

}  // namespace

namespace {
float LoadF32(uint8_t* base, uint32_t addr) {
  const uint32_t v = LoadU32(base, addr);
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}

// Cascade fit inputs/outputs, logged ~1/s (see the file comment). The
// cascades RenderShadows_CSM renders come precomputed from the ShadowView at
// RenderMan+49200 (matrices at +20368 / +20944, 64 bytes each); its per-frame
// update is vtable +12 (RenderModeScene, gated by RenderMan+180672/180673)
// and +8 (RenderMan::Render).
void LogCascadeState(uint8_t* base) {
  static uint32_t s_calls = 0;
  if ((s_calls++ % 120u) != 0) {
    return;
  }
  const uint32_t rm = RenderMan(base);
  const uint32_t vm = LoadU32(base, 0x83368E6C);
  if (rm == 0 || vm == 0) {
    return;
  }
  const uint32_t sv = rm + 49200;
  const uint32_t vtbl = LoadU32(base, sv);
  const uint32_t m0 = sv + 20368;
  const uint32_t idx = LoadU32(base, vm + 128);
  const uint32_t cam = vm + 144 + 6128 * idx;
  REXLOG_INFO(
      "shadow-gate: cascade0 row3=({:.2f},{:.2f},{:.2f},{:.2f}) row0=({:.3f},{:.3f},{:.3f}) | "
      "rm[180672]={} rm[180673]={} | camidx={} cam view row3=({:.2f},{:.2f},{:.2f}) | "
      "shadowview vtbl={:08X} [+8]={:08X} [+12]={:08X} | settings mode(rm+180256)={} "
      "cas_light(rm+180646)={} override(rm+32)={:08X}",
      LoadF32(base, m0 + 48), LoadF32(base, m0 + 52), LoadF32(base, m0 + 56),
      LoadF32(base, m0 + 60), LoadF32(base, m0 + 0), LoadF32(base, m0 + 4),
      LoadF32(base, m0 + 8), LoadU8(base, rm + 180672), LoadU8(base, rm + 180673), idx,
      LoadF32(base, cam + 0x20 + 48), LoadF32(base, cam + 0x20 + 52),
      LoadF32(base, cam + 0x20 + 56), vtbl, LoadU32(base, vtbl + 8), LoadU32(base, vtbl + 12),
      LoadU32(base, rm + 180256), LoadU8(base, rm + 180646), LoadU32(base, rm + 32));
}
}  // namespace

extern "C" REX_FUNC(rex_Sk8_Render_PostFX_RenderShadows_CSM_QAAXXZ_82985338) {
  LogCascadeState(base);
  const GateState s = ReadGate(base);
  if (std::memcmp(&s, &g_gate, sizeof(s)) != 0) {
    const bool renders = s.shadows_tweak == 1 && (s.scene_mode == 0 || s.scene_tweak == 1) &&
                         (s.view == 0 || s.view_flag != 0);
    REXLOG_INFO(
        "shadow-gate: CSM gate {} | shadows_tweak={} scene_mode={} scene_tweak={} "
        "view(RM+160)={:08X} view_flag={} world[31792]={} world[31840]={} ice_take={:08X}",
        renders ? "OPEN" : "CLOSED", s.shadows_tweak, s.scene_mode, s.scene_tweak, s.view,
        s.view_flag, s.world_31792, s.world_31840, IceTake(base));
    g_gate = s;
  }
  __imp__rex_Sk8_Render_PostFX_RenderShadows_CSM_QAAXXZ_82985338(ctx, base);
}

extern "C" REX_FUNC(
    rex_unknown_829C8AC8) {
  const uint32_t scene = ctx.r3.u32;
  const int before = SceneModeNow(base);
  __imp__rex_unknown_829C8AC8(
      ctx, base);
  LogScene("Scene::Init(829C8AC8)", base, scene, before);
}

extern "C" REX_FUNC(
    rex_f1Sk8_Scene_Init_QAA_NIABVVector3_vpu_math_rw_00ABUSceneLocationArray_12_Z_829CC918) {
  const uint32_t scene = ctx.r3.u32;
  const int before = SceneModeNow(base);
  __imp__rex_f1Sk8_Scene_Init_QAA_NIABVVector3_vpu_math_rw_00ABUSceneLocationArray_12_Z_829CC918(
      ctx, base);
  LogScene("Scene::Init(829CC918)", base, scene, before);
}

extern "C" REX_FUNC(rex_f1Sk8_Scene_Purge_QAAXXZ_829BE5B8) {
  const uint32_t scene = ctx.r3.u32;
  const int before = SceneModeNow(base);
  LogScene("Scene::Purge enter", base, scene, before);
  __imp__rex_f1Sk8_Scene_Purge_QAAXXZ_829BE5B8(ctx, base);
  LogScene("Scene::Purge exit", base, scene, before);
}

// Presentation handler of message 0x4B69E3CE (payload bool at +16; very
// likely cMsgPresCASNotifySimShutdown{IsInSkatepark}): copies the bool into
// RenderMan+180646, which makes the per-frame shadow-settings update
// (0x82997658) use the camera-based settings 0x313B9A01 instead of the
// world's 0xE7AC68B9.
extern "C" REX_FUNC(sub_829286D8) {
  const uint32_t msg = ctx.r4.u32;
  const uint32_t rm = RenderMan(base);
  const int before = rm != 0 ? LoadU8(base, rm + 180646) : -1;
  const int value = msg != 0 ? LoadU8(base, msg + 16) : -1;
  __imp__sub_829286D8(ctx, base);
  REXLOG_INFO("shadow-gate: msg 0x4B69E3CE handled: bool={} rm[180646] {} -> {}", value, before,
              rm != 0 ? LoadU8(base, rm + 180646) : -1);
}

// Lua binding that posts message 0x4B69E3CE (args: port name, bool).
extern "C" REX_FUNC(rex_cMsgAssetSimRegArena_HandleMessage_LuaBinding_824F6320) {
  REXLOG_INFO("shadow-gate: Lua posts msg 0x4B69E3CE (lua_State {:08X})", ctx.r3.u32);
  __imp__rex_cMsgAssetSimRegArena_HandleMessage_LuaBinding_824F6320(ctx, base);
}

extern "C" REX_FUNC(sub_829C1480) {
  const int before = SceneModeNow(base);
  __imp__sub_829C1480(ctx, base);
  LogScene("scene-mode clear (829C1480)", base, 0, before);
}

extern "C" REX_FUNC(rex_f1Sk8_Scene_UnBindToGame_AAAXXZ_829C1628) {
  const uint32_t scene = ctx.r3.u32;
  const int before = SceneModeNow(base);
  __imp__rex_f1Sk8_Scene_UnBindToGame_AAAXXZ_829C1628(ctx, base);
  LogScene("Scene::UnBindToGame", base, scene, before);
}
