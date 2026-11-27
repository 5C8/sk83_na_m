// Decompiled render-thread game code (REXGLUE/skate-physics-decomp, tools/sync_sk83.py copies it
// into src/native/decomp), run natively when sk83_native_render_game is on. Built for guest memory
// (RWP_GUEST); calls into not-yet-decompiled guest code go through rwp::Call / rwp::VCall on the
// caller's context (decomp/support/rwp_guestcall.h). Verified with --sk83_replay (sk83_guest_replay.cpp).
#include "generated/sk83_na_m_init.h"

#include <rex/cvar.h>

#include <cmath>
#include <cstdint>
#include <cstdio>

#define RWP_GUEST 1
#include "native/decomp/game/source/render/renderview.cpp"
#include "native/decomp/game/source/render/renderman.cpp"
#include "native/decomp/game/source/render/PostProcessor.cpp"
#include "native/decomp/game/source/scene/sys/scenemanagerimpl.cpp"
#include "native/decomp/packages/ea/attribsys/attribhashmap.cpp"
#include "native/decomp/packages/ea/nfsmix/nfsmixmap.cpp"
#include "native/decomp/packages/ea/apt/aptrender.cpp"
#include "native/decomp/game/source/fe/apt/aptintegration.cpp"
#include "native/decomp/packages/ea/apt/aptcxform.cpp"
#include "native/decomp/game/source/fe/apt/aptrenderingintegration.cpp"
#include "native/decomp/packages/sk8/sk8aptint/source/aptanimation.cpp"
#include "native/decomp/game/source/render/simpledraw.cpp"
#include "native/decomp/game/source/fe/apt/aptrenderingunit.cpp"
#include "native/decomp/game/source/render/rpostfx/rwgpfxrendertarget.cpp"
#include "native/decomp/packages/ea/nfsmix/nfsmixshape.cpp"
#include "native/decomp/game/source/audio/sfxctl/sfxctl_3dobjpos.cpp"
#include "native/decomp/packages/ea/csiscore/source/cmn/csis.cpp"
#include "native/decomp/game/source/audio/audiostates.cpp"
#include "native/decomp/game/source/render/modelinstance.cpp"
#include "native/decomp/game/source/presentation/presentationentity.cpp"
#include "native/decomp/game/source/presentation/world/worldpresentity.cpp"
#include "native/decomp/packages/skate/rendering/sk8shader/dev/source/stateshadow.cpp"
#include "native/decomp/packages/skate/rendering/sk8shader/dev/source/materialman.cpp"
#include "native/decomp/xdk/runtime_mem.cpp"
#include "native/decomp/xdk/d3d/d3d_state.cpp"
#include "native/decomp/packages/ea/eastl/hashtable_find.cpp"
#include "native/decomp/packages/ea/eastl/hashtable_insert.cpp"
#include "native/decomp/packages/ea/eastl/fixed_pool.cpp"
#include "native/decomp/packages/ea/eastl/red_black_tree.cpp"
#include "native/decomp/packages/skate/junktofixup/sk8util/dev/include/util/kdtree.h"
#include "native/decomp/packages/sk8/rawdiscretize/dev/include/rawdiscretize/quatcompress.h"
#include "native/decomp/game/source/presentation/skater/skaterpresentation.cpp"
#include "native/decomp/game/source/render/font/font.cpp"
#include "native/decomp/build/livesk8/sk8shader/dev/xbox2/final/gen/sk83_binder_sources.inc"

REXCVAR_DECLARE(bool, sk83_native_render_game);

namespace rwp {
thread_local PPCContext* g_ctx;

GuestFn ResolveGuest(u32 addr) {
  PPCFunc* f = nullptr;
  if (uint32_t(addr - REX_CODE_BASE) < REX_CODE_SIZE + REX_THUNK_RESERVE_SIZE) f = REX_LOOKUP_FUNC(g_base, addr);
  if (!f) f = rex::runtime::ResolveIndirectFunction(addr);
  return f;
}
}  // namespace rwp

namespace {
// enters hand-written code on this guest context (nested entries keep the outer one)
struct Enter {
  PPCContext* saved;
  Enter(PPCContext& ctx, uint8_t* base) : saved(rwp::g_ctx) {
    rwp::g_base = base;
    rwp::g_ctx = &ctx;
  }
  ~Enter() { rwp::g_ctx = saved; }
};

// native-call counters per hook: the replay report lists them (which functions a snapshot set covers)
struct HookCount {
  const char* name;
  uint64_t n;
  HookCount* next;
};
HookCount* g_hooks = nullptr;
struct HookReg {
  explicit HookReg(HookCount& h) {
    h.next = g_hooks;
    g_hooks = &h;
  }
};
HookCount hc_RenderMeshes{"RenderMeshes (0x829A77B8)", 0, nullptr};
HookReg hr_RenderMeshes(hc_RenderMeshes);
}  // namespace

// One hook per decompiled function: the native body when sk83_native_render_game is on.
#define RENDER_GAME_HOOK(sym, body)             \
  static HookCount hc_##sym{#sym, 0, nullptr};  \
  static HookReg hr_##sym(hc_##sym);            \
  extern "C" REX_FUNC(sym) {                    \
    if (REXCVAR_GET(sk83_native_render_game)) { \
      ++hc_##sym.n;                             \
      Enter e_(ctx, base);                      \
      body;                                     \
      return;                                   \
    }                                           \
    __imp__##sym(ctx, base);                    \
  }

// as RENDER_GAME_HOOK, natively only when cond holds (the rest stays with the recompiled code)
#define RENDER_GAME_HOOK_IF(sym, cond, body)                static HookCount hc_##sym{#sym, 0, nullptr};              static HookReg hr_##sym(hc_##sym);                        extern "C" REX_FUNC(sym) {                                  if (REXCVAR_GET(sk83_native_render_game) && (cond)) {       ++hc_##sym.n;                                             Enter e_(ctx, base);                                      body;                                                     return;                                                 }                                                         __imp__##sym(ctx, base);                                }

namespace sk83_render_game {
// for the replay report: native calls per decompiled function since the last call
void ReportCoverage(FILE* f) {
  for (HookCount* h = g_hooks; h; h = h->next) {
    std::fprintf(f, "    native %-90s %llu calls\n", h->name, (unsigned long long)h->n);
    h->n = 0;
  }
}

// Sk8::Render::SceneRenderView::RenderMeshes (0x829A77B8): called from the existing hook in
// skate3_native_render.cpp instead of the recompiled body.
void RenderMeshes(PPCContext& ctx, uint8_t* base) {
  if (REXCVAR_GET(sk83_native_render_game)) {
    ++hc_RenderMeshes.n;
    Enter e_(ctx, base);
    Sk8::Render::SceneRenderView_RenderMeshes(RWP_FROM_ADDR(Sk8::Render::SceneRenderView, ctx.r3.u32),
                                              RWP_FROM_ADDR(Sk8::Render::SortKeyVector, ctx.r4.u32), ctx.r5.u32,
                                              ctx.r6.u32);
    return;
  }
  __imp__rex_f0Sk8_SceneRenderView_Render_UAAXXZ_829A77B8(ctx, base);
}
}  // namespace sk83_render_game

// game/source/render/renderview.cpp
RENDER_GAME_HOOK(sub_8297A5F8, Sk8::Render::RenderMesh(RWP_FROM_ADDR(Sk8::Render::MeshContext, ctx.r3.u32), ctx.r4.u32))
RENDER_GAME_HOOK(sub_829712A8, Sk8::Render::RenderMeshZ(RWP_FROM_ADDR(Sk8::Render::MeshContext, ctx.r3.u32), ctx.r4.u32))
namespace skate3::native_scene { bool SkipGuestZPrepass(uint32_t view); bool WorldShadeMeshDirect(uint8_t* base, uint32_t mesh_context); }  // skate3_native_scene_gpu.cpp
// RenderZPrePass: skipped for the main view while the native renderer draws the world (the pass
// only fills the emulated depth buffer; see skate3_native_render_scene_skip_guest_zprepass).
// The decomp's SceneRenderView::Render reaches it through this address too.
static HookCount hc_ZPrePassSkipped{"RenderZPrePass skipped (main view, native frame)", 0, nullptr};
static HookReg hr_ZPrePassSkipped(hc_ZPrePassSkipped);
static HookCount hc_sub_8297A478{"sub_8297A478", 0, nullptr};
static HookReg hr_sub_8297A478(hc_sub_8297A478);
extern "C" REX_FUNC(sub_8297A478) {
  if (skate3::native_scene::SkipGuestZPrepass(ctx.r3.u32)) {
    ++hc_ZPrePassSkipped.n;
    return;
  }
  if (REXCVAR_GET(sk83_native_render_game)) {
    ++hc_sub_8297A478.n;
    Enter e_(ctx, base);
    Sk8::Render::SceneRenderView_RenderZPrePass(RWP_FROM_ADDR(Sk8::Render::SceneRenderView, ctx.r3.u32));
    return;
  }
  __imp__sub_8297A478(ctx, base);
}
RENDER_GAME_HOOK(sub_831F4B98, Sk8::Render::PerfMarker(ctx.r3.u32))
// RenderShadowMesh: inside the world-shade pass, recorded directly instead of drawn (see
// skate3::native_scene::WorldShadeMeshDirect).
static HookCount hc_sub_82971360{"sub_82971360", 0, nullptr};
static HookReg hr_sub_82971360(hc_sub_82971360);
static HookCount hc_WsDirect{"RenderShadowMesh (world-shade direct)", 0, nullptr};
static HookReg hr_WsDirect(hc_WsDirect);
extern "C" REX_FUNC(sub_82971360) {
  if (skate3::native_scene::WorldShadeMeshDirect(base, ctx.r3.u32)) {
    ++hc_WsDirect.n;
    return;
  }
  if (REXCVAR_GET(sk83_native_render_game)) {
    ++hc_sub_82971360.n;
    Enter e_(ctx, base);
    Sk8::Render::RenderShadowMesh(RWP_FROM_ADDR(Sk8::Render::MeshContext, ctx.r3.u32), ctx.r4.u32);
    return;
  }
  __imp__sub_82971360(ctx, base);
}
RENDER_GAME_HOOK(sub_8298E6F8, Sk8::Render::SceneRenderView_Sort(RWP_FROM_ADDR(Sk8::Render::SceneRenderView, ctx.r3.u32)))

// Skater restore leaves (decomp agent, 2026-11-17)
RENDER_GAME_HOOK(sub_82905658, ANIM_CODEC::RawDiscretize::QuatDecompressQuantize3((int)ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(sub_82905730, ANIM_CODEC::RawDiscretize::QuatDecompressQuantize4((int)ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(sub_829057D8, ANIM_CODEC::RawDiscretize::TransDecompress((int)ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, (float)ctx.f1.f64))
// r3 = n is kept: the caller reads it
RENDER_GAME_HOOK(sub_829068D0, Sk8::SkaterPresentation_ApplyRelativeSQTs(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32))

// Scene sort keys + shadow-caster gather containers (decomp agent, 2026-11-17)
RENDER_GAME_HOOK(sub_8291A2A0, Sk8::WorldPresEntityOptimesh_FillMtlSrtKeyListWithCulledKeys(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_eastl_CBGH_GU_pair_hashtable_DoFreeNodes_V_eastl_allocator_0DC_00_xcore_U_use_first_U_pair_CBGH_eastl_2_U_equal_to_G_2_U_hash_G_2_Umod_range_hashing_2_Udefault_ranged_has_82980C20,
                 eastl::Hashtable_DoFreeNodes(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(rex_Sk8_QAVPresentationEntity_U_pair_U_hashtable_iterator_insert_PAV12_eastl_0A_0A_eastl_hashtable_PAVPresentationEntity_Sk8_U_pair_QAVPresentationEntity_Sk8_PAV12_eastl_V_ea_829A1A80,
                 eastl::EntityMap_InsertRange(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// a zero bucket count traps (twllei) in the original: left to the recompiled code
RENDER_GAME_HOOK_IF(rex_Sk8_PAVPresentationEntity_hashtable_DoInsertValue_U_pair_QAVPresentationEntity_Sk8_PAV12_eastl_V_eastl_allocator_0DC_0A_xcore_U_use_first_U_pair_QAVPresentationEntity_Sk8_829910C8,
                    REX_LOAD_U32(ctx.r4.u32 + 8) != 0,
                    ctx.r3.u64 = eastl::EntityMap_DoInsertValue(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(sub_8297B3A0, Sk8::Render::ShadowView_Render(ctx.r3.u32, ctx.r4.u32, (unsigned char)ctx.r5.u32))

RENDER_GAME_HOOK(rex_Sk8_Render_SceneRenderView_PrepZPrePass_QAAXXZ_829A6238,
                 Sk8::Render::SceneRenderView_PrepZPrePass(RWP_FROM_ADDR(Sk8::Render::SceneRenderView, ctx.r3.u32)))

// Sk8::Render::SceneRenderView::Render (0x829A9E00), also a --sk83_replay capture point
namespace sk83_replay { void CapturePoint(uint32_t fn, PPCContext& ctx, uint8_t* base); }
static HookCount hc_SceneRender{"SceneRenderView::Render (0x829A9E00)", 0, nullptr};
static HookReg hr_SceneRender(hc_SceneRender);
extern "C" REX_FUNC(sub_829A9E00) {
  sk83_replay::CapturePoint(0x829A9E00, ctx, base);
  if (REXCVAR_GET(sk83_native_render_game)) {
    ++hc_SceneRender.n;
    Enter e_(ctx, base);
    Sk8::Render::SceneRenderView_Render(RWP_FROM_ADDR(Sk8::Render::SceneRenderView, ctx.r3.u32));
    return;
  }
  __imp__sub_829A9E00(ctx, base);
}

// game/source/render/renderman.cpp
RENDER_GAME_HOOK(sub_829715C8, Sk8::Render::RenderMan_RenderSceneView(ctx.r3.u32))

// sk8shader materialman.cpp
RENDER_GAME_HOOK(sub_82F46CF8, Sk8::ShaderSystem::ResetCurrentEffect())
RENDER_GAME_HOOK(sub_82F46DE0, Sk8::ShaderSystem::DispatchEffect(RWP_FROM_ADDR(Sk8::ShaderSystem::EffectBinding, ctx.r3.u32)))
RENDER_GAME_HOOK(sub_82F46D18, Sk8::ShaderSystem::BeginEffect(RWP_FROM_ADDR(Sk8::ShaderSystem::EffectBinding, ctx.r3.u32), ctx.r4.u32))

// sk8shader stateshadow.cpp
RENDER_GAME_HOOK(sub_82F63B00, StateShadow::Reset())
RENDER_GAME_HOOK(rex_GFrameworkParametersImpl_Fesl_UAAPAXI_Z_82F63B88, StateShadow::SetRasterizerState(ctx.r3.u32))
RENDER_GAME_HOOK(sub_82F63BD0, StateShadow::SetDepthStencilState(ctx.r3.u32))
RENDER_GAME_HOOK(rex_StateShadow_SetState_YAXPAVBlendState_renderengine_Z_82F63C18, StateShadow::SetBlendState(ctx.r3.u32))
RENDER_GAME_HOOK(sub_82F63C60, StateShadow::SetRenderTargetState(ctx.r3.u32))
RENDER_GAME_HOOK(sub_82F63CA8, StateShadow::SetTexture(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(sub_82F63D70, StateShadow::SetSamplerState(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_StateShadow_SetVertexProgramState_YAXPBVVertexProgramState_renderengine_Z_82F63DD8,
                 StateShadow::SetVertexProgramState(ctx.r3.u32))


// Sk8::Render::RenderMan::Render(dt) (0x829ACA40): the zd debug port's generated hook owns the
// symbol (and the replay capture point); it calls this instead of the recompiled body
static HookCount hc_RenderBody{"RenderMan::Render (0x829ACA40)", 0, nullptr};
static HookReg hr_RenderBody(hc_RenderBody);
namespace sk83_render_game {
void RenderBody(PPCContext& ctx, uint8_t* base) {
  if (REXCVAR_GET(sk83_native_render_game)) {
    ++hc_RenderBody.n;
    Enter e_(ctx, base);
    Sk8::Render::RenderMan_Render(ctx.r3.u32, (float)ctx.f1.f64);
    return;
  }
  __imp__sub_829ACA40(ctx, base);
}
}  // namespace sk83_render_game

// XDK / CRT / rw memory copies (decomp/xdk/runtime_mem.cpp): native unless the ranges overlap
// native copies (decomp/xdk/runtime_mem.cpp); entry = the routine's own stack writes, made first
#define RENDER_GAME_COPY_HOOK(sym, entry, D_, S_, N_)        \
  static HookCount hc_##sym{#sym, 0, nullptr};              \
  static HookReg hr_##sym(hc_##sym);                        \
  extern "C" REX_FUNC(sym) {                                \
    if (REXCVAR_GET(sk83_native_render_game)) {             \
      rwp::g_base = base;                                   \
      entry;                                                \
      if (RwpRuntime::Copy(D_, S_, N_)) {                   \
        ++hc_##sym.n;                                       \
        return;                                             \
      }                                                     \
    }                                                       \
    __imp__##sym(ctx, base);                                \
  }
RENDER_GAME_COPY_HOOK(rex_XMemCpy_82DAE9C0, RwpRuntime::XMemCpyEntry(ctx), ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)
RENDER_GAME_COPY_HOOK(rex_memcpy_83190EC0, RwpRuntime::MemcpyEntry(ctx), ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)
RENDER_GAME_COPY_HOOK(rex_rw_core_stdc_MemCopy_YAXPAXPBXI_Z_82CE6AB0, (void)0, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)

// --sk83_replay capture points on the render thread's other frame parts (not decompiled yet)
namespace sk83_job_timeline {
int64_t FnBegin();
void FnEnd(uint32_t addr, int64_t t0);
}  // namespace sk83_job_timeline
#define REPLAY_CAPTURE_HOOK(sym, addr)                 \
  extern "C" REX_FUNC(sym) {                           \
    sk83_replay::CapturePoint(addr, ctx, base);        \
    const int64_t jt0_ = sk83_job_timeline::FnBegin(); \
    __imp__##sym(ctx, base);                           \
    sk83_job_timeline::FnEnd(addr, jt0_);              \
  }
// a capture point that is decompiled too: capture first, then the native body or the recompiled one
#define RENDER_GAME_CAPTURE_HOOK(sym, addr, body)   static HookCount hc_##sym{#sym, 0, nullptr};    static HookReg hr_##sym(hc_##sym);              extern "C" REX_FUNC(sym) {                        sk83_replay::CapturePoint(addr, ctx, base);     if (REXCVAR_GET(sk83_native_render_game)) {       ++hc_##sym.n;                                   Enter e_(ctx, base);                            body;                                           return;                                       }                                               __imp__##sym(ctx, base);                      }
RENDER_GAME_CAPTURE_HOOK(sub_829A9F38, 0x829A9F38u, Sk8::Render::ShadowView_CullShadowCasters(ctx.r3.u32))   // light-view culling
REPLAY_CAPTURE_HOOK(sub_829A0388, 0x829A0388u)                                         // scene culling
REPLAY_CAPTURE_HOOK(rex_Sk8_Presentation_Update_QAAXM_Z_8292ED90, 0x8292ED90u)
REPLAY_CAPTURE_HOOK(rex_Sk8_Scene_SceneManagerImpl_UpdateFrameBegin_QAAXM_Z_829CD290, 0x829CD290u)
REPLAY_CAPTURE_HOOK(rex_Sk8_DataSysMan_RestoreFrame_QAAXXZ_8289D5E0, 0x8289D5E0u)
// AudioSystem::Update: hook + capture point in sk83_audio_thread.cpp
REPLAY_CAPTURE_HOOK(rex_Sk8_Scene_SceneManagerImpl_UpdateDeltaFrameBegin_AAAXM_Z_829CC060, 0x829CC060u)
RENDER_GAME_CAPTURE_HOOK(rex_NFSMixMap_Update3DMixCtls_QAAXXZ_82B0EEF8, 0x82B0EEF8u, NFSMixMap_Update3DMixCtls(ctx.r3.u32))

// XDK D3D runtime (decomp/xdk/d3d/d3d_state.cpp): the native renderer's hooks own these symbols
// (skate3_native_render.cpp) and call these instead of the recompiled bodies
#define RENDER_GAME_DISPATCH(fn, imp, body)          static HookCount hc_##fn{#fn, 0, nullptr};         static HookReg hr_##fn(hc_##fn);                   namespace sk83_render_game {                       void fn(PPCContext& ctx, uint8_t* base) {            if (REXCVAR_GET(sk83_native_render_game)) {          ++hc_##fn.n;                                       Enter e_(ctx, base);                               body;                                              return;                                          }                                                  imp(ctx, base);                                  }                                                  }
// skate3_apt_spread.cpp owns the hook (APT HUD spread) and calls this for the original body
RENDER_GAME_DISPATCH(AptLegacyTransform, __imp__rex_CallLegacyTransformCallbacks_831610C8,
                     ::CallLegacyTransformCallbacks(ctx.r3.u32))
RENDER_GAME_DISPATCH(SetTexture, __imp__rex_D3DDevice_SetTexture_82D99C48,
                     D3D::SetTexture(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u64))
RENDER_GAME_DISPATCH(SetStreamSource, __imp__rex_D3DDevice_SetStreamSource_82D94340,
                     D3D::SetStreamSource(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u64))
RENDER_GAME_DISPATCH(SetIndices, __imp__rex_D3DDevice_SetIndices_82D944E8, D3D::SetIndices(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_DISPATCH(SetPendingRenderStates, __imp__rex_D3D_SetPending_RenderStates_YAXPAVCDevice_1_KKPAX_Z_82D9F618,
                     D3D::SetPending_RenderStates(ctx.r3.u32, ctx.r4.u64, ctx.r5.u32, ctx.r6.u32))
RENDER_GAME_HOOK(rex_D3D_SetPending_FetchConstants_YAXPAVCDevice_1_K_Z_82D9F858, D3D::SetPending_FetchConstants(ctx.r3.u32, ctx.r4.u64))
RENDER_GAME_DISPATCH(SetVertexShader, __imp__rex_D3DDevice_SetVertexShader_82D9DFD8, D3D::SetVertexShader(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_DISPATCH(SetPixelShader, __imp__rex_D3DDevice_SetPixelShader_82D9DE18, D3D::SetPixelShader(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_DISPATCH(SetViewport, __imp__rex_D3D_SetViewport_YAXPAUD3DDevice_MMMMMMK_Z_82D94930,
                     D3D::SetViewport(ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.f5.f64, ctx.f6.f64))
RENDER_GAME_HOOK(rex_ChallengeRuntime_cStateGraph_RegisterState_AAAXPAVcState_2_Z_82F63EE0, StateShadow::SetPixelProgram(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(sub_82975A78, Sk8::Render::ModelInstance_SetTechnique(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(sub_82929C40, Sk8::PresentationEntity_SetTechnique(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_Sk8_WorldPresEntityOptimesh_GetShadowCulledMeshList_UAAABV_xvector_PAUMeshContext_Render_Sk8_0DC_0A_xcore_XZ_8291C998,
                 ctx.r3.u64 = Sk8::WorldPresEntityOptimesh_GetShadowCulledMeshList(ctx.r3.u32))
RENDER_GAME_HOOK(sub_82980CF8, Sk8::Render::SortKeys(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// a zero bucket count traps (twi) in the original: left to the recompiled code
RENDER_GAME_HOOK_IF(
    rex_eastl_CBGH_GU_pair_hashtable_find_V_eastl_allocator_0DC_00_xcore_U_use_first_U_pair_CBGH_eastl_2_U_equal_to_G_2_U_hash_G_2_Umod_range_hashing_2_Udefault_ranged_hash_2_Upr_82B6DCB0,
    REX_LOAD_U32(ctx.r4.u32 + 8) != 0, eastl::HashFindU16(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK_IF(
    rex_Sk8_FE_W4eSk8FEScreenState_hashtable_find_U_pair_CBW4eSk8FEScreenState_FE_Sk8_H_eastl_V_eastl_allocator_0L_0A_xcore_U_use_first_U_pair_CBW4eSk8FEScreenState_FE_Sk8_H_east_82990780,
    REX_LOAD_U32(ctx.r4.u32 + 8) != 0, eastl::HashFindS32(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(rex_Sk8_Render_cFont_FindChar_QBAPBUtGlyph_123_G_Z_82999480,
                 ctx.r3.u64 = Sk8::Render::cFont_FindChar(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_IsInLightView_YA_AW4eShadowCullType_Render_Sk8_AAVMatrix44_vpu_math_rw_ABUtAABB_pegasus_Z_8297AF48,
                 ctx.r3.u64 = Sk8::Render::IsInLightView(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(sub_829A7C48, Sk8::Render::ShadowView_GatherCasters(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
RENDER_GAME_HOOK(sub_829A7E30, Sk8::Render::ShadowView_GatherCasters2(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(rex_Sk8_Render_RenderView_DetermineVisibilityFromGameState_IAA_NPAVPresentationEntity_3_Z_8297A228,
                 ctx.r3.u64 = Sk8::Render::RenderView_DetermineVisibilityFromGameState(ctx.r3.u32, ctx.r4.u32) ? 1u : 0u)
RENDER_GAME_HOOK(rex_Sk8_BufferedIterativeJob_Flush_QAAXXZ_82974C38, Sk8::BufferedIterativeJob_Flush(ctx.r3.u32))
RENDER_GAME_HOOK(rex_Sk8_BufferedIterativeJob_Wait_QAAXXZ_82974CE0, Sk8::BufferedIterativeJob_Wait(ctx.r3.u32))
RENDER_GAME_HOOK(sub_829A7608, Sk8::Render::SceneRenderView_Cull(ctx.r3.u32))
RENDER_GAME_HOOK(rex_Sk8_VectorizedCullingJob_YAXIIII_Z_829ADDD8,
                 Sk8::Render::VectorizedCullingJob(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
RENDER_GAME_HOOK(sub_82995040, Sk8::Render::PostFX_Render(ctx.r3.u32))
RENDER_GAME_HOOK(sub_829A0120, Sk8::Render::PostFX_Frame(ctx.r3.u32))
RENDER_GAME_HOOK(sub_829C6FE8, Sk8::Scene::SceneAssetLoaders_Update(ctx.r3.u32))
// a zero table size traps (twi) in the original: left to the recompiled code
RENDER_GAME_HOOK_IF(rex_Attrib_HashMap_FindIndex_QBAI_K_Z_82D7CB18, REX_LOAD_U16(ctx.r3.u32 + 8) != 0,
                    ctx.r3.u64 = Attrib::HashMap_FindIndex(ctx.r3.u32, ctx.r4.u64))
RENDER_GAME_CAPTURE_HOOK(rex_NFSMixMap_MixMasterChannels_QAAXXZ_82B0DFF0, 0x82B0DFF0u, NFSMixMap_MixMasterChannels(ctx.r3.u32))
RENDER_GAME_CAPTURE_HOOK(rex_NFSMixMap_UpdateMasterChannels_QAAXXZ_82B0DF30, 0x82B0DF30u, NFSMixMap_UpdateMasterChannels(ctx.r3.u32))
RENDER_GAME_CAPTURE_HOOK(rex_NFSMixMap_ProcessMixMap_UAAXMH_Z_82B123A0, 0x82B123A0u,
                         NFSMixMap_ProcessMixMap(ctx.r3.u32, ctx.f1.f64, (int)ctx.r5.u32))
RENDER_GAME_HOOK(rex_NFSMixMap_UpdateSubChannels_QAAXXZ_82B0DE70, NFSMixMap_UpdateSubChannels(ctx.r3.u32))
RENDER_GAME_HOOK(rex_AptRenderElement_Render_QBAXI_Z_83164140, AptRenderElement_Render(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_AptRenderBuffer_Render_QAAXI_Z_83164400, AptRenderBuffer_Render(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_AptRenderListSet_Render_QAAXI_Z_83164460, AptRenderListSet_Render(ctx.r3.u32, ctx.r4.u32))
// sk8shader's generated effect binders (tools/gen_shader_binders.py in the decomp repo)
#include "native/decomp/build/livesk8/sk8shader/dev/xbox2/final/gen/sk83_binder_hooks.inc"
RENDER_GAME_HOOK(rex_D3DDevice_SetScissorRect_82D94248, D3D::SetScissorRect(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_D3D_SetSurfaceClip_YAXPAVCDevice_1_HHHH_Z_82D91C90,
                 D3D::SetSurfaceClip(ctx.r3.u32, (int)ctx.r4.u32, (int)ctx.r5.u32, (int)ctx.r6.u32, (int)ctx.r7.u32))
RENDER_GAME_HOOK(rex_NFSMixShape_GetPitchMultFromCents_YAMH_Z_82B0B998,
                 ctx.f1.f64 = NFSMixShape::GetPitchMultFromCents_((int)ctx.r3.u32))
RENDER_GAME_HOOK(rex_NFSMixShape_GetCurveOutput_YAHW4eMIXTABLEID_H_N_Z_82B0BAB8,
                 ctx.r3.u64 = NFSMixShape::GetCurveOutput_(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK_IF(rex_Sk8_HUD_HUDManager2_CBIPAUHUDComponentInfo_IU_pair_hashtable_find_eastl_V_fixed_hashtable_allocator_0CC_0M_0CA_03_0A_0A_V_eastl_allocator_0BJ_00_xcore_2_U_use_first_U_pai_82FF1910,
                    REX_LOAD_U32(ctx.r4.u32 + 8) != 0, eastl::HashFindS32(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// a zero size traps (twi) in the original: left to the recompiled code
RENDER_GAME_HOOK_IF(rex_Attrib_KVCollection_VecHashMap_FindIndex_VTablePolicy_Class_2_00_0GA_QBAI_K_Z_82D7B830,
                    REX_LOAD_U16(ctx.r3.u32 + 4) != 0, ctx.r3.u64 = Attrib::VecHashMap_FindIndex(ctx.r3.u32, ctx.r4.u64))
RENDER_GAME_HOOK_IF(rex_Attrib_HashMap_Find_QBAPAVNode_2_K_Z_82D7CC00, REX_LOAD_U16(ctx.r3.u32 + 8) != 0,
                    ctx.r3.u64 = Attrib::HashMap_Find(ctx.r3.u32, ctx.r4.u64))
RENDER_GAME_HOOK(rex_Attrib_Collection_GetNode_QBAPAVNode_2_KAAPBV12_Z_82D81FE0,
                 ctx.r3.u64 = Attrib::Collection_GetNode(ctx.r3.u32, ctx.r4.u64, ctx.r5.u32))
RENDER_GAME_HOOK(sub_82D82178, ctx.r3.u64 = Attrib::Collection_GetData(ctx.r3.u32, ctx.r4.u64, ctx.r5.u32))
RENDER_GAME_HOOK(rex_Attrib_VCollection_GetPointer_Node_Attrib_QBAPAXPAXPBVCollection_1_Z_82D7ACA0,
                 ctx.r3.u64 = Attrib::Node_GetPointer(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(rex_Attrib_VCollection_GetArray_Node_Attrib_QBAPAVArray_1_PAXPBVCollection_1_Z_82D7AD50,
                 ctx.r3.u64 = Attrib::Node_GetArray(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(rex_Sk8_Audio_CSTATEMGR_Base_UpdateParams_UAAXM_Z_82A694B0,
                 Sk8::Audio::CSTATEMGR_Base_UpdateParams(ctx.r3.u32, ctx.f1.f64))
RENDER_GAME_HOOK(rex_Sk8_Audio_CSTATEMGR_Base_ProcessUpdate_UAAXXZ_82A69530, Sk8::Audio::CSTATEMGR_Base_ProcessUpdate(ctx.r3.u32))
RENDER_GAME_HOOK(rex_Sk8_Audio_CSTATE_Base_UpdateParams_UAAXM_Z_82A699C0,
                 Sk8::Audio::CSTATE_Base_UpdateParams(ctx.r3.u32, ctx.f1.f64))

// Audio update (decomp agent, 2026-11-17)
RENDER_GAME_HOOK(rex_NFSMixShape_GetCentsFromPitchMult_YAHM_Z_82B0B438,
                 ctx.r3.u64 = NFSMixShape::GetCentsFromPitchMult_(ctx.f1.f64))
RENDER_GAME_HOOK(rex_NFSMixShape_GetAzimShapeOutput_YAHW4eMIXTABLEID_0PAHH_Z_82B0BC58,
                 ctx.r3.s64 = (int)NFSMixShape::GetAzimShapeOutput_(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
RENDER_GAME_HOOK(rex_Sk8_Audio_CSTATE_Base_ProcessUpdate_UAAXXZ_82A69970, Sk8::Audio::CSTATE_Base_ProcessUpdate(ctx.r3.u32))
RENDER_GAME_HOOK(rex_Sk8_Audio_SFXCTL_3DObjPos_UpdateParams_UAAXM_Z_82645290,
                 Sk8::Audio::SFXCTL_3DObjPos_UpdateParams(ctx.r3.u32, ctx.f1.f64))
RENDER_GAME_HOOK(rex_Sk8_Audio_SFXCTL_3DObjPos_UpdateDoppler_IAAXHM_Z_8261F0A0,
                 Sk8::Audio::SFXCTL_3DObjPos_UpdateDoppler(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
RENDER_GAME_HOOK(rex_Csis_ClassData_SendParameters_AAAXPATParameter_2_Z_82A6F238,
                 Csis::ClassData_SendParameters(ctx.r3.u32, ctx.r4.u32))

// PostFX + UI 2D (decomp agent, 2026-11-17)
RENDER_GAME_HOOK(rex_Sk8_FE_AptIntegration_SetAptMatrix_SAXAAVMatrix44_vpu_math_rw_PAUAptMatrix_Z_8273B450,
                 Sk8::FE::AptIntegration_SetAptMatrix(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(sub_8273B6D8, Sk8::FE::AptRenderingIntegration_SetVertexMatrix(ctx.r3.u32))
RENDER_GAME_HOOK(sub_82746420, Sk8::FE::AptRenderingIntegration_SetColourTransform(ctx.r3.u32))
RENDER_GAME_HOOK(rex_AptColorHelper_CopyToFloatArray4_QBAXPAM_Z_83154B28, AptColorHelper_CopyToFloatArray4(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_Sk8_FE_AptShape_Draw_QAAXABVMatrix44_vpu_math_rw_AAUtRGBA_xcore_1_NM_Z_82F43890,
                 Sk8::FE::AptShape_Draw(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.f1.f64))
RENDER_GAME_HOOK(rex_Sk8_FE_AptTextureUnit_Draw_UAAXABVMatrix44_vpu_math_rw_AAUtRGBA_xcore_1_NM_Z_827467A0,
                 Sk8::FE::AptTextureUnit_Draw(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.f1.f64))
RENDER_GAME_HOOK(rex_DtRGBA_xcore_QBA_AU01_ABU01_Z_8273D858, ctx.r3.u64 = Sk8::FE::TRgba_Mul(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(rex_HtRGBA_xcore_QBA_AU01_ABU01_Z_8273D7D0, ctx.r3.u64 = Sk8::FE::TRgba_Add(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(sub_82987D38, Sk8::Render::SimpleDraw_Draw_TriListTextured_Fasttrack(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                 ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32))
RENDER_GAME_HOOK(rex_renderengine_postfx_Target_Resolve_QAAXXZ_8296EBE0, renderengine::postfx::Target_Resolve(ctx.r3.u32))
RENDER_GAME_HOOK(rex_renderengine_postfx_RenderTarget_Begin_QAAXXZ_8296ED48,
                 renderengine::postfx::RenderTarget_Begin(ctx.r3.u32, ctx.r4.u32))
RENDER_GAME_HOOK(rex_renderengine_postfx_RenderTarget_End_QAAX_N0PAUresolve_RECT_23_Z_82976250,
                 renderengine::postfx::RenderTarget_End(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32))
RENDER_GAME_HOOK(sub_8296F158, Sk8::Render::PostFX_DrawQuad(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                 ctx.r8.u32, ctx.r9.u32, ctx.r10.u32, REX_LOAD_U32(ctx.r1.u32 + 0x54u), REX_LOAD_U32(ctx.r1.u32 + 0x5cu)))
RENDER_GAME_HOOK(sub_8296F490, Sk8::Render::PostFX_BoxTapOffsets(ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r6.s32))

// Presentation::Update anti-frustum k-d tree + EASTL helpers (decomp agent, 2026-11-17)
// a zero node size traps in the original: left to the recompiled code
RENDER_GAME_HOOK_IF(rex_eastl_fixed_pool_base_init_QAAXPAXIIII_Z_82D173B8, ctx.r6.u32 >= 4u || ctx.r7.u32 <= 0xfffffffcu,
                    eastl::fixed_pool_base_init(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32))
RENDER_GAME_HOOK(rex_eastl_RBTreeIncrement_YAPAUrbtree_node_base_1_PBU21_Z_82D17430, ctx.r3.u64 = eastl::RBTreeIncrement(ctx.r3.u32))
RENDER_GAME_HOOK(rex_eastl_RBTreeInsert_YAXPAUrbtree_node_base_1_00W4RBTreeSide_1_Z_82D17620,
                 eastl::RBTreeInsert(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, (int)ctx.r6.u32))
RENDER_GAME_HOOK(sub_82EF15E8, ctx.r3.u64 = Sk8::Render::KdResultSet_Insert(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
RENDER_GAME_HOOK(rex_Sk8_LivingWorld_PAUtRAINItemData_V_KDObjDist_rbtree_DoInsertValueImpl_VVector2_vpu_math_rw_01_V1_U_less_V_KDObjDist_PAUtRAINItemData_LivingWorld_Sk8_VVector2_vpu_math_rw__82EF1240,
                 ctx.r3.u64 = Sk8::Render::KdResultSet_DoInsertValueImpl(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32))
RENDER_GAME_HOOK(rex_Sk8_FE_CBIPAUMapIcon_IU_pair_rbtree_DoNukeSubtree_eastl_U_less_I_2_V_fixed_node_pool_0BI_00_03_0A_0A_V_eastl_allocator_07_0P_xcore_2_U_use_first_U_pair_CBIPAUMapIcon_FE_S_830B1E48,
                 Sk8::Render::KdResultSet_DoNukeSubtree(ctx.r3.u32, ctx.r4.u32))
// dimstart 0..2 only (the plane value is read from a stack copy at 4 * dim)
RENDER_GAME_HOOK_IF(rex_Sk8_Render_PAUAntiFrustumQuad_KDNode_FindNear_VVector3_vpu_math_rw_02_QAA_NFABVVector3_vpu_math_rw_MHAAV_fixed_vector_PAV_KDNode_PAUAntiFrustumQuad_Render_Sk8_VVector3_vp_829A94B8,
                    uint32_t(int32_t(int16_t(ctx.r4.u32))) <= 2u,
                    ctx.r3.u64 = Sk8::Render::KDNode_FindNear(ctx.r3.u32, int16_t(ctx.r4.u32), ctx.r5.u32, ctx.f1.f64, (int)ctx.r6.u32,
                                                              (int)ctx.r7.u32, ctx.r8.u32, ctx.r9.u32))
// numItems > 256 asserts in the original: left to the recompiled code
RENDER_GAME_HOOK_IF(rex_Sk8_Render_PAUAntiFrustumQuad_KDTree_FindNear_VVector3_vpu_math_rw_02_QAAXABVVector3_vpu_math_rw_MIAAIPAPAUAntiFrustumQuad_Render_Sk8_PAM_Z_829AAA28,
                    ctx.r6.u32 <= 256u,
                    Sk8::Render::KDTree_FindNear(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32))
