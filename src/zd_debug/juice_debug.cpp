// Juice RPCs zd has and m dropped.
//
// Both builds register their RPCs with Juice::RPCPlugin (RegisterASyncRPC m
// 0x82A4FE68, RegisterRPC m 0x82A4FDC0) from JuiceRPCNG::Register_Fe / _Sim /
// _Pres / RegisterRPC; the server lists them and invokes one by its handler
// address. zd registers 17 more (metrics files, NPC / vehicle / DMO spawning,
// hole detection, marquee viewer control, shader / scope queries, ...). They
// are registered here, after m's own, with zd's (transplanted) handlers
// behind guest thunks.
//
// Sim / Pres handlers post a JuiceData message to their thread, whose
// controller (JuiceRPCNG::Sim_Cltr / Pres_Cltr) switches on the message type.
// The two builds number the types differently (GetSimFPS: zd 8, m 9) and m's
// controllers lack zd's cases, so a message sent from zd code is tagged
// (type + kZdTypeBase) in m's senders and m's controllers hand tagged
// messages to zd's controllers (transplanted) with zd's type restored.

#include <algorithm>
#include <atomic>
#include <cstring>
#include <string>

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"
#include "generated/sk83_na_m_init.h"

#include <rex/logging.h>

namespace {
constexpr uint32_t kZdTypeBase = 0x1000;
constexpr uint32_t kZdCodeLo = 0x833E0000u, kZdCodeHi = 0x847E2354u;

enum Ret { kRetBool, kRetString, kRetInt, kRetSync };
// m's async return writers (zd 83AE7758 / 60 / 68 by the m registrations of
// the same RPC kinds: IsInBackEnd / GetSimFPS / LoadChar_SCV).
constexpr uint32_t kMRet[] = {0x82B35878u, 0x82B35880u, 0x82B35888u, 0};

struct ZdRpc {
  const char* category;
  const char* name;
  const char* args;
  const char* returns;
  const char* help;
  zdx::GuestFn handler;
  Ret ret;
};

// zd's registrations (Register_Sim 83755F10, Register_Pres 837567C8,
// RegisterRPC 83756A40) of the RPCs m does not register.
const ZdRpc kZdRpcs[] = {
    {"Sim", "PrintSimFPSMetricsFile", "String fileSufFix", "none",
     "Prints the FPS Metrics for Sim Thread, returns 0 on success", zdx_83754360, kRetInt},
    {"Sim", "PrintMemoryMetricsFile", "String fileSufFix", "none",
     "Prints the Memory Metrics, returns 0 on success", zdx_83754470, kRetInt},
    {"Sim", "PrintLoadTimeMetricsFile", "none", "none", "PrintLoadTimeMetricsFile", zdx_83754408,
     kRetInt},
    {"Sim", "SetPhoneListCash", "Int32 Cash", "none", "Set the BE PhoneList Cash", zdx_83754968,
     kRetInt},
    {"Sim", "SpawnNPC", "String entityKey, String modelKey", "String RecipeID",
     "Spawn an Pedestrian using 'livingworld_entities' & 'livingworld_models' [returns the recipe "
     "used]",
     zdx_837549E0, kRetString},
    {"Sim", "SpawnVehicle", "String entityKey, String modelKey", "Int32 Vehicle",
     "Spawn a vehicle using 'livingworld_entities' & 'livingworld_models', skater must be on a "
     "road with rain data. [return the recipeID used]",
     zdx_83754AB0, kRetString},
    {"Sim", "SpawnDMO", "long cAssetID", "String RecipeID",
     "Spawn a DMO using the cAssetID from 'dmo_models'", zdx_83754B80, kRetString},
    {"Sim", "SetAutoSpawnVehicle", "Bool bEnable", "none", "Should Living World spawn vehicles?",
     zdx_83750F58, kRetInt},
    {"Sim", "GetNPCLocation", "Int32 NPC_ID", "String NPC_Location", "Get the NPC location",
     zdx_83754C08, kRetString},
    {"Sim", "HoleDetection",
     "Float x, Float y, Float z, Float width, Int32 height, Float Increament, Float HoleWidth",
     "none", "Collision Detection of surface, return hole location or no_hole or error msg",
     zdx_837550D8, kRetString},
    {"Pres", "PrintPresFPSMetricsFile", "String fileSufFix", "none",
     "Prints the FPS Metrics for Pres Thread, returns 0 on success", zdx_837553F8, kRetInt},
    {"Pres", "LoadCharacterInMarqueeViewer", "String validName, Int32 pos", "none",
     "Loads the new character in the marquee viewer at the specified pos and returns 1 on "
     "success.",
     zdx_83755740, kRetInt},
    {"Pres", "BootUpMarqueeViewer", "none", "none",
     "Starts up the marquee viewer for character verification.", zdx_83755AC8, kRetBool},
    {"Pres", "SetAnimationInMarqueeViewer", "String animName", "none",
     "Sets the specified animation in the marquee viewer on the characters shown.", zdx_837557F8,
     kRetBool},
    {"Pres", "GetShader", "Int32 index", "Int32 count", "1-Env Shader, 2-Char Shader, 3-DMO Shader",
     zdx_837558A0, kRetInt},
    {"Pres", "GetScopes", "Int32 index", "Int32 count",
     "1-Effect Scope, 2-Material Scope, 3-Instance Scope", zdx_83755918, kRetInt},
    {"System", "IsHOMCompleted", "none", "none", "Checks to see if the HOM completed.",
     zdx_837503C0, kRetSync},
};

std::atomic<uint32_t> g_plugin{0};

uint32_t GuestString(uint8_t* base, const char* s) {
  const uint32_t n = uint32_t(std::strlen(s)) + 1;
  const uint32_t a = zdx::AllocZeroed(base, n, 4);
  if (a) std::memcpy(REX_RAW_ADDR(a), s, n);
  return a;
}

bool FromZdCode(PPCContext& ctx) {
  const uint32_t lr = uint32_t(ctx.lr);
  return lr >= kZdCodeLo && lr < kZdCodeHi;
}

// A sender's (type, JuiceData) from zd code: tag the type (r3 and the
// JuiceData's first word, the high half of r4).
void TagZdMessage(PPCContext& ctx) {
  ctx.r3.u64 = ctx.r3.u32 + kZdTypeBase;
  ctx.r4.u64 = ctx.r4.u64 + (uint64_t(kZdTypeBase) << 32);
}

// A controller's JuiceData (r4:r5): a tagged type, restored, for zd's controller.
bool UntagZdMessage(PPCContext& ctx) {
  const uint32_t type = uint32_t(ctx.r4.u64 >> 32);
  if (type < kZdTypeBase) return false;
  ctx.r4.u64 = ctx.r4.u64 - (uint64_t(kZdTypeBase) << 32);
  return true;
}
}  // namespace

void zdx::GameVersionForPath(PPCContext& ctx, uint8_t* base) {
  static uint32_t buf = 0;
  constexpr uint32_t kSize = 64;
  rex_Sk8_GetGameVersionString_YAPBDXZ_826EB138(ctx, base);
  if (!buf) buf = zdx::AllocZeroed(base, kSize, 4);
  if (!buf || !ctx.r3.u32) return;
  const char* s = reinterpret_cast<const char*>(REX_RAW_ADDR(ctx.r3.u32));
  char* d = reinterpret_cast<char*>(REX_RAW_ADDR(buf));
  uint32_t i = 0;
  for (; i + 1 < kSize && s[i]; ++i) d[i] = (s[i] == '/' || s[i] == '\\') ? '_' : s[i];
  d[i] = 0;
  ctx.r3.u64 = buf;
}

// Juice::RPCPlugin::RegisterASyncRPC (m 0x82A4FE68): the plugin `this`.
extern "C" REX_FUNC(sub_82A4FE68) {
  if (!g_plugin.load()) g_plugin = ctx.r3.u32;
  __imp__sub_82A4FE68(ctx, base);
}

// JuiceRPCNG::Register_Pres (m 0x827F5290), m's last registration batch:
// zd's extra RPCs follow.
extern "C" REX_FUNC(rex_Juice_JuiceRPCNG_Register_Pres_AAAXXZ_827F5290) {
  __imp__rex_Juice_JuiceRPCNG_Register_Pres_AAAXXZ_827F5290(ctx, base);
  const uint32_t plugin = g_plugin.load();
  if (!zd_debug::Enabled() || !zdx::PoolReady(base) || !plugin) return;
  int n = 0;
  for (const ZdRpc& r : kZdRpcs) {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = plugin;
    call.c.r4.u64 = GuestString(base, r.category);
    call.c.r5.u64 = GuestString(base, r.name);
    call.c.r6.u64 = GuestString(base, r.args);
    call.c.r7.u64 = GuestString(base, r.returns);
    call.c.r8.u64 = GuestString(base, r.help);
    call.c.r9.u64 = zdx::FnThunk(r.handler);
    call.c.r10.u64 = kMRet[r.ret];
    call(r.ret == kRetSync ? sub_82A4FDC0 : sub_82A4FE68);
    ++n;
  }
  REXLOG_INFO("zd_debug: registered {} zd Juice RPCs", n);
}

// JuiceRPCNG::SendMsg_AsyncSim (m 0x827F3708) / SendMsg_AsyncPres (m 0x827F40E8).
extern "C" REX_FUNC(sub_827F3708) {
  if (zd_debug::Enabled() && FromZdCode(ctx)) TagZdMessage(ctx);
  __imp__sub_827F3708(ctx, base);
}
extern "C" REX_FUNC(sub_827F40E8) {
  if (zd_debug::Enabled() && FromZdCode(ctx)) TagZdMessage(ctx);
  __imp__sub_827F40E8(ctx, base);
}

// JuiceRPCNG::Sim_Cltr (m 0x827F56D0) / Pres_Cltr (m 0x827F4848): tagged
// messages go to zd's controllers (83756C80 / 83757218).
extern "C" REX_FUNC(rex_Juice_JuiceRPCNG_Sim_Cltr_QAAXUJuiceData_2_Z_827F56D0) {
  if (zd_debug::Enabled() && zdx::Ready() && UntagZdMessage(ctx)) {
    zdx::Run(ctx, base, zdx_83756C80);
    return;
  }
  __imp__rex_Juice_JuiceRPCNG_Sim_Cltr_QAAXUJuiceData_2_Z_827F56D0(ctx, base);
}
// SetAnimationInMarqueeViewer (zd Pres message 16): zd's worker hands any
// name to the viewer's animators, and a name the loaded animation db lacks
// leaves them evaluating garbage (a crash ~1 min later). The name is checked
// against the viewer's own list (MVManager::GetAvailableAnimationNames, what
// Juice's GetAvailableAnimationNames returns); an unknown one answers false.
namespace {
constexpr uint32_t kZdMsgSetAnimation = 16;
constexpr uint32_t kMvViewing = 8;                 // MVManager+68 viewing state (zd 7; zdx_config)
constexpr uint32_t kZdPresentation = 0x848FC844u;  // zd Sk8::Presentation* (zdx-mapped)
constexpr uint32_t kZdAnimNameBuf = 0x848F5730u;    // the RPC's name buffer (zdx-mapped)
constexpr uint32_t kMRpcPlugin = 0x8337D328u;       // m RPCPlugin* (zd 0x84913BC8)

bool MarqueeAnimationKnown(PPCContext& ctx, uint8_t* base, bool& viewer_up) {
  viewer_up = false;
  const uint32_t pres = REX_LOAD_U32(zdx::Map(kZdPresentation));
  if (!pres) return true;
  uint32_t mv = 0;
  {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = pres;
    call(rex_Sk8_Presentation_GetMarqueeViewerManager_QBAPAVMVManager_Character_XZ_82925FB0);
    mv = call.c.r3.u32;
  }
  if (!mv || REX_LOAD_U32(mv + 68) != kMvViewing) return true;  // not viewing: zd's ApplyAnimation is a no-op
  viewer_up = true;
  const uint32_t vec = zdx::AllocZeroed(base, 16);
  if (!vec) return true;
  {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = mv;
    call.c.r4.u64 = vec;
    call(zdx_838D9D40);  // MVManager::GetAvailableAnimationNames(xvector<xfixed_string<256>>&)
  }
  const char* want = reinterpret_cast<const char*>(REX_RAW_ADDR(zdx::Map(kZdAnimNameBuf)));
  bool found = false;
  const uint32_t b = REX_LOAD_U32(vec), e = REX_LOAD_U32(vec + 4), cap = REX_LOAD_U32(vec + 8);
  for (uint32_t p = b; p && p < e; p += 272) {
    const uint32_t s = REX_LOAD_U32(p);
    if (s && std::strcmp(reinterpret_cast<const char*>(REX_RAW_ADDR(s)), want) == 0) found = true;
  }
  if (b) {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = b;
    call.c.r4.u64 = cap - b;
    call.c.r5.u64 = 26;
    call(rex_XCore_EASTL_Category_Free_YAXPAXIH_Z_82A3F518);
  }
  if (!found) REXLOG_WARN("zd_debug: SetAnimationInMarqueeViewer: '{}' is not in the viewer's animation db", want);
  return found;
}
}  // namespace

extern "C" REX_FUNC(sub_827F4848) {
  if (zd_debug::Enabled() && zdx::Ready() && UntagZdMessage(ctx)) {
    if (uint32_t(ctx.r4.u64 >> 32) == kZdMsgSetAnimation) {
      bool up = false;
      const uint64_t data = ctx.r5.u64;  // (returnId, returnFn)
      PPCContext saved = ctx;
      if (!MarqueeAnimationKnown(ctx, base, up)) {
        ctx = saved;
        const uint32_t result = zdx::AllocZeroed(base, 4);
        zdx::GuestCall call(ctx, base);
        call.c.r3.u64 = REX_LOAD_U32(kMRpcPlugin);
        call.c.r4.u64 = uint32_t(data >> 32);
        call.c.r5.u64 = uint32_t(data);
        call.c.r6.u64 = result;  // false
        call(sub_82A4FF30);      // RPCPlugin::ASyncRPCReturn
        return;
      }
      ctx = saved;
    }
    zdx::Run(ctx, base, zdx_83757218);
    return;
  }
  __imp__sub_827F4848(ctx, base);
}

// Sk8::Challenge::ChallengeHOMManager::Update (m 0x826CBD30). zd (83611560)
// ends a finished Hall of Meat run with its listener call and then
// JuiceRPCNG::SetIsHOMCompleted(true), which Juice's IsHOMCompleted reports
// (and clears); m kept the listener call but dropped the setter. m's ending
// path is the same: [this+32] set -> listener, [this+44] = 1, [this+32] = 0.
extern "C" REX_FUNC(rex_Sk8_Challenge_ChallengeHOMManager_Update_UAAXPBVcTriggerTester_3_Z_826CBD30) {
  const uint32_t self = ctx.r3.u32;
  const uint8_t pre_pending = REX_LOAD_U8(self + 32);
  const uint32_t pre_done = REX_LOAD_U32(self + 44);
  __imp__rex_Sk8_Challenge_ChallengeHOMManager_Update_UAAXPBVcTriggerTester_3_Z_826CBD30(ctx, base);
  if (!zd_debug::Enabled() || !zdx::PoolReady(base)) return;
  if (REX_LOAD_U8(self + 32) == 0 && REX_LOAD_U32(self + 44) == 1 && (pre_pending || pre_done != 1)) {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = 1;
    call(zdx_8374FE60);  // Juice::JuiceRPCNG::SetIsHOMCompleted(true)
    REXLOG_INFO("zd_debug: Hall of Meat completed (Juice IsHOMCompleted)");
  }
}

// Sk8::LivingWorld::CensusMan::SpawnDynamicObject (m 0x8282FE78), called by
// zd's SpawnDMO worker (83750D58) through the census interface. zd's spawn
// request passes its two cAssetIDs by reference (+96: variant, the zd static
// 0x848F5880; +100: the DMO model from the RPC) and ends at +106; m's request
// holds them by value (+96, +104) and has three more flags at +112..114. The
// request is rebuilt in m's layout (zd +104..106 -> m +112..114, as m's own
// callers fill them).
namespace {
constexpr uint32_t kNullAssetId = 0x833C2368u;  // m's null cAssetID (DynamicObject::OnSpawnComplete's "no locator")
}
extern "C" REX_FUNC(rex_Sk8_LivingWorld_CensusMan_SpawnDynamicObject_UAAPAVIDynamicObject_3_KABVcAssetID_assman_1ABVMatrix44Affine_vpu_math_rw_N333_Z_8282FE78) {
  if (!zd_debug::Enabled() || !FromZdCode(ctx) || !zdx::PoolReady(base)) {
    __imp__rex_Sk8_LivingWorld_CensusMan_SpawnDynamicObject_UAAPAVIDynamicObject_3_KABVcAssetID_assman_1ABVMatrix44Affine_vpu_math_rw_N333_Z_8282FE78(ctx, base);
    return;
  }
  constexpr uint32_t kMInfoSize = 128;
  thread_local uint32_t m_info = 0;
  if (!m_info) m_info = zdx::AllocZeroed(base, kMInfoSize, 16);
  const uint32_t zd_info = ctx.r4.u32;
  if (!m_info || !zd_info) {
    ctx.r3.u64 = 0;
    return;
  }
  std::memset(REX_RAW_ADDR(m_info), 0, kMInfoSize);
  std::memcpy(REX_RAW_ADDR(m_info), REX_RAW_ADDR(zd_info), 96);  // key, matrix, flags +80..+95
  const uint32_t variant = REX_LOAD_U32(zd_info + 96), model = REX_LOAD_U32(zd_info + 100);
  uint64_t v = variant ? REX_LOAD_U64(variant) : 0;
  if (!v) v = REX_LOAD_U64(kNullAssetId);
  REX_STORE_U64(m_info + 96, v);
  REX_STORE_U64(m_info + 104, model ? REX_LOAD_U64(model) : REX_LOAD_U64(kNullAssetId));
  for (uint32_t i = 0; i < 3; ++i) REX_STORE_U8(m_info + 112 + i, REX_LOAD_U8(zd_info + 104 + i));
  ctx.r4.u64 = m_info;
  __imp__rex_Sk8_LivingWorld_CensusMan_SpawnDynamicObject_UAAPAVIDynamicObject_3_KABVcAssetID_assman_1ABVMatrix44Affine_vpu_math_rw_N333_Z_8282FE78(ctx, base);
}

// Character::MVManager::LoadMarquee (m 0x82968308), from zd's
// LoadCharacterInMarqueeViewer worker (837510A8; slots 0/1). Replacing one
// loaded marquee in place breaks the others in m: animation ids resolve
// through per-asset ranges in Andale's DataBaseManager, and the other slots'
// animators use ids from a range the unloaded character's asset owned
// (DataBaseManager.cpp(296) on their next update; same with zd's LoadMarquee).
// So every loaded slot is unloaded first and all are loaded again, the
// requested character in the requested slot: no animator outlives the ranges
// it resolved. Slot names: earlier replacements, else mvsettings' entries.
namespace {
constexpr uint32_t kMaxMarqueeSlots = 16;
uint32_t g_marquee_names[kMaxMarqueeSlots] = {};  // guest strings (replacements)
}
extern "C" REX_FUNC(rex_Character_MVManager_LoadMarquee_QAAXPBDH_Z_82968308) {
  if (!zd_debug::Enabled() || !FromZdCode(ctx) || !zdx::PoolReady(base)) {
    __imp__rex_Character_MVManager_LoadMarquee_QAAXPBDH_Z_82968308(ctx, base);
    return;
  }
  const uint32_t mv = ctx.r3.u32;
  const int32_t pos = ctx.r5.s32;
  const char* req = reinterpret_cast<const char*>(REX_RAW_ADDR(ctx.r4.u32));
  const uint32_t vb = REX_LOAD_U32(mv + 28), ve = REX_LOAD_U32(mv + 32);
  const uint32_t slots = std::min<uint32_t>(vb && ve > vb ? (ve - vb) / 4 : 0, kMaxMarqueeSlots);
  if (pos < 0 || uint32_t(pos) >= slots) {
    __imp__rex_Character_MVManager_LoadMarquee_QAAXPBDH_Z_82968308(ctx, base);
    return;
  }
  g_marquee_names[pos] = GuestString(base, req);
  // names of the loaded slots
  const uint32_t settings = REX_LOAD_U32(mv + 60);
  const uint32_t sb = settings ? REX_LOAD_U32(settings + 60) : 0, se = settings ? REX_LOAD_U32(settings + 64) : 0;
  constexpr uint32_t kMarqueeSettingSize = 336;
  uint32_t names[kMaxMarqueeSlots] = {};
  for (uint32_t i = 0; i < slots; ++i) {
    if (!REX_LOAD_U32(vb + 4 * i) && int32_t(i) != pos) continue;
    names[i] = g_marquee_names[i];
    if (!names[i] && sb + (i + 1) * kMarqueeSettingSize <= se) {
      const uint32_t e = sb + i * kMarqueeSettingSize;
      if (REX_LOAD_U32(e + 20) > REX_LOAD_U32(e + 16)) names[i] = REX_LOAD_U32(e + 16);
    }
  }
  for (uint32_t i = 0; i < slots; ++i) {
    if (!REX_LOAD_U32(vb + 4 * i)) continue;
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = mv;
    call.c.r4.u64 = i;
    call(rex_Character_MVManager_UnloadMarquee_AAAXH_Z_82967D48);
  }
  for (uint32_t i = 0; i < slots; ++i) {
    if (!names[i]) continue;
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = mv;
    call.c.r4.u64 = names[i];
    call.c.r5.u64 = i;
    call(__imp__rex_Character_MVManager_LoadMarquee_QAAXPBDH_Z_82968308);
  }
  REXLOG_INFO("zd_debug: marquee slot {} -> {} (all slots reloaded)", pos, req);
}
