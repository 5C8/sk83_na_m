// In-game debug menu entries m dropped (README item 16).
//
// zd's GameDebugMenu (zd 0x837C9E10) builds two more sub-menus than m's
// (m 0x8284F2F8): "Debug Graph" (Debug::GraphMenu, member +1216, its
// cSubMenuItem +1696, the 4th entry) and "AI Paths" (AIPath::AIPathMainMenu).
// Both classes sit on the same cMenu base as m's (384 bytes, 28-byte items),
// so zd's sub-menu objects are built in guest memory after m's ctor, wrapped
// in m's cSubMenuItem with zd's label and inserted at zd's position;
// GameDebugMenu::onGainFocus runs their Setup first, as zd's did.

#include <algorithm>
#include <atomic>
#include <cstring>
#include <string>
#include <unordered_map>

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include "generated/sk83_na_m_init.h"

#include <rex/cvar.h>
#include <rex/logging.h>

namespace {

constexpr uint32_t kGraphMenuSize = 480;      // zd GameDebugMenu +1216 .. +1696
constexpr uint32_t kSubMenuItemSize = 28;
constexpr uint32_t kLabelDebugGraph = 0x8229C42Cu;  // zd "Debug Graph"
constexpr int kDebugGraphIndex = 3;           // after title, separator, Config Variables
constexpr uint32_t kAIPathMainMenuSize = 28684;  // zd GameDebugMenu +6316 .. +35000
constexpr uint32_t kLabelAIPaths = 0x8229C4A4u;   // zd "AI Paths"
constexpr int kAIPathsIndex = 8;                  // zd's item order (after Debug Graph is in)

// "Memory Category" (zd GameDebugMenu +40672, id 3): an enum item, Print /
// Print(Verbose) / Save to file / Save to file(Verbose); onCommand runs
// XAllocator::PrintReport (to the TTY) or the report to "memLog.csv" (zd
// 837CA530). m's own item ids include 3, so ours gets an id of its own.
constexpr uint32_t kEnumMenuItemSize = 36;        // zd GameDebugMenu +40672 .. +40708
constexpr uint32_t kLabelMemoryCategory = 0x8229C4F0u;  // zd "Memory Category"
constexpr uint32_t kMemCatOptionNames[4] = {0x8229C3A4u, 0x8229C3ACu, 0x8229C3BCu, 0x8229C3CCu};
constexpr uint32_t kLabelMemLogFile = 0x8229C524u;  // zd "memLog.csv"
constexpr uint32_t kLabelMemLogDistrict = 0x8229C5A4u;  // zd "Financial" (zd's fixed district)
constexpr int32_t kMemCategoryItemId = 0x4D43;
std::atomic<uint32_t> g_memcat_value{0};          // guest int the enum item edits
std::atomic<uint32_t> g_memcat_item{0};

struct Injected {
  uint32_t graph_menu = 0;
};
std::atomic<uint32_t> g_ai_menu{0};   // AIPathMainMenu (for the menu test)
std::unordered_map<uint32_t, Injected> g_menus;  // GameDebugMenu -> its zd sub-menus

}  // namespace

// Sk8::GameDebugMenu::GameDebugMenu(Simulation&) (m 0x8284F2F8).
extern "C" REX_FUNC(rex_f0GameDebugMenu_Sk8_QAA_AAVSimulation_1_Z_8284F2F8) {
  const uint32_t menu = ctx.r3.u32;
  __imp__rex_f0GameDebugMenu_Sk8_QAA_AAVSimulation_1_Z_8284F2F8(ctx, base);
  if (!zdx::PoolReady(base) || !menu) return;  // provision menus for runtime enabling
  Injected inj;
  // "Debug Graph": zd's GraphMenu in an m cSubMenuItem.
  const uint32_t graph = zdx::AllocZeroed(base, kGraphMenuSize);
  const uint32_t item = zdx::AllocZeroed(base, kSubMenuItemSize);
  if (graph && item) {
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = graph;
      call(zdx_836D9A78);  // Debug::GraphMenu::GraphMenu()
    }
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = item;
      call.c.r4.u64 = graph;
      call.c.r5.u64 = zdx::Map(kLabelDebugGraph);
      call(rex_f0cSubMenuItem_QAA_PAVcMenu_PBD_Z_82D84F28);
    }
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = menu;
      call.c.r4.u64 = item;
      call.c.r5.u64 = uint64_t(int64_t(kDebugGraphIndex));
      call(rex_cMenu_addItem_QAAXPAVcMenuItem_H_Z_82D842C0);
    }
    inj.graph_menu = graph;
    REXLOG_INFO("zdx: in-game debug menu: added \"Debug Graph\" (GraphMenu {:08X})", graph);
  }
  // "AI Paths": zd's AIPathMainMenu (the AI path editing / recording / test
  // sub-menus) in an m cSubMenuItem.
  const uint32_t ai = zdx::AllocZeroed(base, kAIPathMainMenuSize);
  const uint32_t ai_item = zdx::AllocZeroed(base, kSubMenuItemSize);
  if (ai && ai_item) {
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = ai;
      call(zdx_83503B48);  // AIPath::AIPathMainMenu::AIPathMainMenu()
    }
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = ai_item;
      call.c.r4.u64 = ai;
      call.c.r5.u64 = zdx::Map(kLabelAIPaths);
      call(rex_f0cSubMenuItem_QAA_PAVcMenu_PBD_Z_82D84F28);
    }
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = menu;
      call.c.r4.u64 = ai_item;
      call.c.r5.u64 = uint64_t(int64_t(kAIPathsIndex));
      call(rex_cMenu_addItem_QAAXPAVcMenuItem_H_Z_82D842C0);
    }
    g_ai_menu = ai;
    REXLOG_INFO("zdx: in-game debug menu: added \"AI Paths\" (AIPathMainMenu {:08X})", ai);
  }
  // "Memory Category": m's cEnumMenuItem over a guest int, appended.
  const uint32_t tags = zdx::AllocZeroed(base, 8 * 4);
  const uint32_t value = g_memcat_value.load() ? g_memcat_value.load() : zdx::AllocZeroed(base, 4);
  const uint32_t mc_item = zdx::AllocZeroed(base, kEnumMenuItemSize);
  if (tags && value && mc_item) {
    g_memcat_value = value;
    g_memcat_item = mc_item;
    for (uint32_t i = 0; i < 4; ++i) {
      REX_STORE_U32(tags + 8 * i, zdx::Map(kMemCatOptionNames[i]));
      REX_STORE_U32(tags + 8 * i + 4, i);
    }
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = mc_item;
      call.c.r4.u64 = uint64_t(int64_t(kMemCategoryItemId));
      call.c.r5.u64 = value;
      call.c.r6.u64 = zdx::Map(kLabelMemoryCategory);
      call.c.r7.u64 = 4;
      call.c.r8.u64 = tags;
      call(rex_f0cEnumMenuItem_QAA_HPAHPBDHPBUtEnumMenuItemTag_Z_82D85BE8);
    }
    {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = menu;
      call.c.r4.u64 = mc_item;
      call.c.r5.u64 = uint64_t(int64_t(-1));
      call(rex_cMenu_addItem_QAAXPAVcMenuItem_H_Z_82D842C0);
    }
    REXLOG_INFO("zdx: in-game debug menu: added \"Memory Category\"");
  }
  g_menus[menu] = inj;
}

// Sk8::GameDebugMenu::onCommand (m 0x8284F648): the Memory Category item, as
// zd's onCommand (837CA530) handles it.
extern "C" REX_FUNC(rex_Sk8_GameDebugMenu_onCommand_UAA_AW4tMIR_PAVcMenuItem_H_Z_8284F648) {
  const uint32_t item = ctx.r4.u32;
  const uint32_t value = g_memcat_value.load();
  if (!item || !value || int32_t(REX_LOAD_U32(item + 12)) != kMemCategoryItemId || !zd_debug::Enabled() ||
      !zdx::PoolReady(base)) {
    __imp__rex_Sk8_GameDebugMenu_onCommand_UAA_AW4tMIR_PAVcMenuItem_H_Z_8284F648(ctx, base);
    return;
  }
  const int32_t v = int32_t(REX_LOAD_U32(value));
  const uint32_t verbose = (v == 1 || v == 3) ? 1 : 0;
  uint32_t alloc = 0;
  {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = 0;
    call(rex_xcore_VXAllocator_SingletonHolder_Get_V_StaticMemorySingletonTypeTraits_VXAllocator_xcore_SAAAVXAllocator_xcore_W4eSingletonThreadOwnerShipTransfer_Z_825AC278);  // SingletonHolder<XAllocator>::Get()
    alloc = call.c.r3.u32;
  }
  if (alloc && (v == 0 || v == 1)) {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = alloc;
    call.c.r4.u64 = verbose;
    call(zdx_83B152C8);  // XAllocator::PrintReport(verbose)
  } else if (alloc && (v == 2 || v == 3)) {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = alloc;
    call.c.r4.u64 = zdx::Map(kLabelMemLogFile);
    call.c.r5.u64 = zdx::Map(kLabelMemLogDistrict);
    call.c.r6.u64 = verbose;
    call(zdx_83B152E0);  // XAllocator::PrintReport(file, district, verbose)
  }
  REXLOG_INFO("zdx: Memory Category: {}", v);
  ctx.r3.u64 = 2;
}

// Sk8::GameDebugMenu::onGainFocus(cMenuSys*) (m 0x8284E340): zd's set up its
// sub-menus first (GraphMenu::Setup lists the graph channels).
extern "C" REX_FUNC(rex_Sk8_GameDebugMenu_onGainFocus_UAAXPAVcMenuSys_Z_8284E340) {
  const auto it = g_menus.find(ctx.r3.u32);
  if (it != g_menus.end() && zd_debug::Enabled() && zdx::PoolReady(base)) {
    if (it->second.graph_menu) {
      zdx::GuestCall call(ctx, base);
      call.c.r3.u64 = it->second.graph_menu;
      call(zdx_836D1160);  // Debug::GraphMenu::Setup()
    }
  }
  __imp__rex_Sk8_GameDebugMenu_onGainFocus_UAAXPAVcMenuSys_Z_8284E340(ctx, base);
}

// ---- Character Debug Menu: "Start Marquee Viewer" -------------------------
// zd: CharacterDebugMenu::onCommand (837CA5F8) item 0 sends flow message 83;
// StateOnlineFE (FilterImpl) answers it by running StateMarqueeViewer::
// DoMarqueeViewer on its marquee state (pause 11 -> WaitForSimPause ->
// InViewer -> UnloadViewer) and waiting for that state.
// m: the menu handler is a stub; m's StateMarqueeViewer is a shell, and m's
// message 82 (zd's 83 slot) starts the newer StateCharacterViewer instead.
// Here item 0 runs zd's handler (message 82) and flags the request; m's message
// handler (82874900, StateOnlineFE's "do" call for 82) then runs zd's
// DoMarqueeViewer on m's StateMarqueeViewer object, and StateOnlineFE waits on
// that state (its saved r30, which m's code passes on, is pointed at it). zd's
// FilterImpl / OnLeaveState are in that state's vtable (vslots_debug.cpp).
namespace {
std::atomic<uint32_t> g_online_fe{0};      // StateOnlineFE (its FilterImpl's this)
std::atomic<bool> g_marquee_request{false};
std::atomic<uint32_t> g_game_module{0};    // Sk8::GameModule (SetupInGameDebugMenus' this)
constexpr uint32_t kStateMarqueeViewerVt = 0x821ABCC8u;

uint32_t FindMarqueeState(uint8_t* base) {
  const uint32_t fe = g_online_fe.load();
  const uint32_t list = fe ? REX_LOAD_U32(fe + 4) : 0;
  for (uint32_t off = 0; list && off < 512; off += 4) {
    const uint32_t o = REX_LOAD_U32(list + off);
    if (o >= 0x30000000u && o < 0x60000000u && REX_LOAD_U32(o) == kStateMarqueeViewerVt) return o;
  }
  return 0;
}
}  // namespace

// Sk8::GameModule::SetupInGameDebugMenus (m 0x82845A08). zd also makes
// Character::MarqueeViewerDebugMenu here (GameModule+60, ext:9 in m): the
// marquee viewer's own menu (exit, animations, camera, the console vars), which
// zd's MVManager::ProcessInput (zdx_config REPLACE) opens on the viewer's menu
// input. Its console-var browser needs zd's DebugHeap size (heaps_debug.cpp).
extern "C" REX_FUNC(rex_Sk8_GameModule_SetupInGameDebugMenus_QAAXXZ_82845A08) {
  const uint32_t gm = ctx.r3.u32;
  g_game_module = gm;
  __imp__rex_Sk8_GameModule_SetupInGameDebugMenus_QAAXXZ_82845A08(ctx, base);
  if (!zdx::PoolReady(base) || !gm) return;  // provision menus for runtime enabling
  const uint32_t ext = zdx::Ext(gm, 9);
  if (!ext || REX_LOAD_U32(ext + 0)) return;
  constexpr uint32_t kMarqueeViewerDebugMenuSize = 1636;  // zd's allocation
  const uint32_t menu = zdx::AllocZeroed(base, kMarqueeViewerDebugMenuSize);
  if (!menu) return;
  {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = menu;
    call(zdx_837CAB08);  // Character::MarqueeViewerDebugMenu::MarqueeViewerDebugMenu()
  }
  REX_STORE_U32(ext + 0, menu);
  REXLOG_INFO("zdx: MarqueeViewerDebugMenu {:08X} (GameModule {:08X})", menu, gm);
  // Character::CACPrototypeDebugMenu (GameModule+64): the CAC prototype
  // viewer's menu (cacproto_debug.cpp), opened by LoadCACPrototypeMenu.
  constexpr uint32_t kCACPrototypeDebugMenuSize = 2208;  // zd's allocation
  if (const uint32_t cac = zdx::AllocZeroed(base, kCACPrototypeDebugMenuSize)) {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = cac;
    call(zdx_837C8760);  // Character::CACPrototypeDebugMenu::CACPrototypeDebugMenu()
    REX_STORE_U32(ext + 4, cac);
    REXLOG_INFO("zdx: CACPrototypeDebugMenu {:08X}", cac);
  }
}

// Character::CharacterDebugMenu::onCommand (m 0x8284E460).
extern "C" REX_FUNC(rex_Character_CharacterDebugMenu_onCommand_UAA_AW4tMIR_PAVcMenuItem_H_Z_8284E460) {
  if (!zd_debug::Enabled() || !zdx::Ready() || zdx::MarqueeMessageId() <= 0) {
    __imp__rex_Character_CharacterDebugMenu_onCommand_UAA_AW4tMIR_PAVcMenuItem_H_Z_8284E460(ctx, base);
    return;
  }
  const uint32_t item = REX_LOAD_U32(ctx.r4.u32 + 12);
  REXLOG_INFO("zdx: Character Debug Menu item {}", item);
  if (item == 0) g_marquee_request = true;
  zdx::Run(ctx, base, zdx_837CA5F8);
}

// Sk8::Flow::StateOnlineFE::FilterImpl (m 0x828956A0): remembers the state.
extern "C" REX_FUNC(rex_Sk8_Flow_StateOnlineFE_FilterImpl_MAA_AW4FilterResult_23_ABV_Ptr_VStateMessage_Flow_Sk8_Net_3_Z_828956A0) {
  g_online_fe = ctx.r3.u32;
  __imp__rex_Sk8_Flow_StateOnlineFE_FilterImpl_MAA_AW4FilterResult_23_ABV_Ptr_VStateMessage_Flow_Sk8_Net_3_Z_828956A0(ctx, base);
}

// StateOnlineFE's "do" call for message 82 (m 0x82874900; m starts the
// character viewer state passed in r3).
extern "C" REX_FUNC(sub_82874900) {
  if (g_marquee_request.exchange(false)) {
    if (const uint32_t mq = FindMarqueeState(base)) {
      REXLOG_INFO("zdx: marquee viewer: zd DoMarqueeViewer on StateMarqueeViewer {:08X}", mq);
      zdx::MarqueeStageBegin(ctx, base);
      ctx.r3.u64 = mq;
      zdx::Run(ctx, base, zdx_837EF348);   // StateMarqueeViewer::DoMarqueeViewer
      ctx.r30.u64 = mq;                     // StateOnlineFE then waits for this state
      return;
    }
    REXLOG_WARN("zdx: marquee viewer: no StateMarqueeViewer object found");
  }
  __imp__sub_82874900(ctx, base);
}


// ---- The marquee viewer's stage ---------------------------------------------
// zd's viewer puts its camera at mvsettings.xml's absolute <camera> position
// (815, -60.3, 1012) and stands the characters relative to it; m has nothing
// to draw there. m's own character viewer (StateCharacterViewer, the
// smartphone skater select) shows MVManager's characters in the create-a-
// skater stage: LooseWorldPresentation::LoadLooseWorld(Presentation+72, CaS)
// on entry, UnloadLooseWorld on exit. The marquee viewer uses that stage the
// same way, and mvsettings' camera anchor is moved onto it (the camera m's
// viewer uses there); zd's layout (offsets, pan/tilt/fov, animations) stays.
REXCVAR_DEFINE_BOOL(zdx_marquee_stage, true, "Skate 3 zd debug",
                    "Marquee viewer: show the characters on m's create-a-skater stage (m's own "
                    "character viewer's stage) instead of mvsettings' absolute camera position");
REXCVAR_DEFINE_DOUBLE(zdx_marquee_stage_x, -3.8, "Skate 3 zd debug", "Marquee viewer stage camera x");
REXCVAR_DEFINE_DOUBLE(zdx_marquee_stage_y, 0.9, "Skate 3 zd debug", "Marquee viewer stage camera y");
REXCVAR_DEFINE_DOUBLE(zdx_marquee_stage_z, -7.6, "Skate 3 zd debug", "Marquee viewer stage camera z");

namespace {
constexpr uint32_t kPresentation = 0x83367290u;  // Sk8::Presentation*
constexpr uint32_t kLooseWorldCaS = 2;           // LooseWorldPresentation eWorldType
std::atomic<bool> g_stage_loaded{false};
}  // namespace

namespace zdx {
void MarqueeStageBegin(PPCContext& ctx, uint8_t* base) {
  const uint32_t pres = REX_LOAD_U32(kPresentation);
  if (!REXCVAR_GET(zdx_marquee_stage) || !pres || g_stage_loaded.exchange(true)) return;
  GuestCall call(ctx, base);
  call.c.r3.u64 = pres + 72;
  call.c.r4.u64 = kLooseWorldCaS;
  call(rex_Sk8_LooseWorldPresentation_LoadLooseWorld_QAAXW4eWorldType_12_Z_8291A510);
  REXLOG_INFO("zdx: marquee viewer: create-a-skater stage loaded");
}

void MarqueeStageEnd(PPCContext& ctx, uint8_t* base) {
  const uint32_t pres = REX_LOAD_U32(kPresentation);
  if (!pres || !g_stage_loaded.exchange(false)) return;
  GuestCall call(ctx, base);
  call.c.r3.u64 = pres + 72;
  call(rex_Sk8_LooseWorldPresentation_UnloadLooseWorld_QAAXXZ_82911228);
  REXLOG_INFO("zdx: marquee viewer: create-a-skater stage unloaded");
}
}  // namespace zdx

// Character::ViewerCamera::SetDefault(const Vector3& pos, pan, tilt, fov)
// (m 0x826980A8): MVManager::Update passes mvsettings' camera anchor here when
// it starts the camera move, before it places the characters relative to the
// same anchor (MVSettings +32, MVManager +60). Both move onto the stage.
extern "C" REX_FUNC(rex_Character_ViewerCamera_SetDefault_QAAXABVVector3_vpu_math_rw_MMM_Z_826980A8) {
  if (g_stage_loaded.load() && ctx.r4.u32) {
    const float v[3] = {float(REXCVAR_GET(zdx_marquee_stage_x)), float(REXCVAR_GET(zdx_marquee_stage_y)),
                        float(REXCVAR_GET(zdx_marquee_stage_z))};
    const uint32_t pres = REX_LOAD_U32(kPresentation);
    const uint32_t mv = pres ? REX_LOAD_U32(pres + 199972) : 0;  // Presentation's MVManager
    const uint32_t settings = mv ? REX_LOAD_U32(mv + 60) : 0;
    for (int i = 0; i < 3; ++i) {
      uint32_t u;
      std::memcpy(&u, &v[i], 4);
      REX_STORE_U32(ctx.r4.u32 + 4 * i, u);
      if (settings) REX_STORE_U32(settings + 32 + 4 * i, u);
    }
    REXLOG_INFO("zdx: marquee viewer: camera anchored on the stage ({}, {}, {})", v[0], v[1], v[2]);
  }
  __imp__rex_Character_ViewerCamera_SetDefault_QAAXABVVector3_vpu_math_rw_MMM_Z_826980A8(ctx, base);
}

// Test hook: zdx_test_charmenu_item=N fires the Character Debug Menu command N
// once, ~10 s into the game (as if picked in the menu; for tests without input).
REXCVAR_DEFINE_INT32(zdx_test_charmenu_item, -1, "Skate 3 zd debug",
                     "Test: run Character Debug Menu item N once, 10 s into the game (-1 = off; 100+N = start item N only)");

REXCVAR_DEFINE_INT32(zdx_test_memcat, -1, "Skate 3 zd debug",
                     "Test: fire the in-game debug menu's Memory Category command with value N once, ~15 s in (-1 = off)");

REXCVAR_DEFINE_STRING(zdx_test_cac_items, "", "Skate 3 zd debug",
                      "Test (with zdx_test_charmenu_item=101): CAC Prototype menu item ids to fire, "
                      "comma-separated, one every 6 s from 20 s; cN[:v] queues viewer command N as from the pad");

REXCVAR_DEFINE_INT32(zdx_marquee_message, 82, "Skate 3 zd debug",
                     "Flow message the Character Debug Menu's Start Marquee Viewer sends (zd 83; "
                     "m's number for that StateOnlineFE slot); 0 = keep m's no-op menu");

namespace zdx {
int32_t MarqueeMessageId() { return REXCVAR_GET(zdx_marquee_message); }
// Juice's BootUpMarqueeViewer (zd 83756FA0) sends the same message as the
// menu item: flag the request so m's handler runs zd's DoMarqueeViewer.
int32_t RequestMarqueeViewer() {
  g_marquee_request = true;
  return MarqueeMessageId();
}
int32_t MarqueeDoneMessageId() { return 81; }

void RunMenuTests(PPCContext& ctx, uint8_t* base) {
  if (const int mc = REXCVAR_GET(zdx_test_memcat); mc >= 0 && Ready()) {
    static int mc_frames = 0;
    if (++mc_frames == 900 && g_memcat_item.load() && !g_menus.empty()) {
      REX_STORE_U32(g_memcat_value.load(), uint32_t(mc));
      GuestCall call(ctx, base);
      call.c.r3.u64 = g_menus.begin()->first;
      call.c.r4.u64 = g_memcat_item.load();
      call.c.r5.u64 = 0;
      REXLOG_INFO("zdx: test: Memory Category {}", mc);
      call(rex_Sk8_GameDebugMenu_onCommand_UAA_AW4tMIR_PAVcMenuItem_H_Z_8284F648);
    }
  }
  const int item_cvar = REXCVAR_GET(zdx_test_charmenu_item);
  if (item_cvar < 0 || !Ready()) return;
  static int frames = 0;
  ++frames;

  // 100 + N: only start item N (the rest is left to real input)
  if (item_cvar >= 100) {
    const std::string script = REXCVAR_GET(zdx_test_cac_items);
    const uint32_t gm = g_game_module.load();
    const uint32_t cm = gm ? REX_LOAD_U32(Ext(gm, 9) + 4) : 0;
    if (!script.empty() && cm && frames == 1080) {
      // open the CAC Prototype menu first (its onGainFocus takes the CASPrototype)
      GuestCall call(ctx, base);
      call.c.r3.u64 = gm;
      REXLOG_INFO("zdx: test: open the CAC prototype menu");
      call(zdx_837A9678);
      return;
    }
    if (!script.empty() && cm && frames >= 1200 && (frames - 1200) % 360 == 0) {
      int k = (frames - 1200) / 360, id = -1;
      std::string tok;
      size_t pos = 0;
      for (int i = 0; i <= k && pos != std::string::npos; ++i) {
        const size_t e = script.find(',', pos);
        if (i == k) tok = script.substr(pos, e == std::string::npos ? std::string::npos : e - pos);
        pos = e == std::string::npos ? e : e + 1;
      }
      if (tok.empty()) return;
      if (tok[0] == 'c') {  // cN[:value]: a viewer command, as from the pad
        const size_t colon = tok.find(':');
        const uint32_t cmd = uint32_t(std::atoi(tok.substr(1, colon == std::string::npos ? std::string::npos : colon - 1).c_str()));
        const float v = colon == std::string::npos ? 1.0f : float(std::atof(tok.substr(colon + 1).c_str()));
        REXLOG_INFO("zdx: test: CAC prototype viewer command {} ({}) -> {}", cmd, v, TestPushProtoCommand(base, cmd, v));
        return;
      }
      id = std::atoi(tok.c_str());
      if (id < 0) return;
      if (!REX_LOAD_U32(cm + 1708)) {  // closed by the last item: reopen it (as Start does)
        GuestCall open(ctx, base);
        open.c.r3.u64 = gm;
        open(zdx_837A9678);
      }
      GuestCall call(ctx, base, 0x40);
      REX_STORE_U32(call.scratch + 12, uint32_t(id));
      call.c.r3.u64 = cm;
      call.c.r4.u64 = call.scratch;
      call.c.r5.u64 = 0;
      REXLOG_INFO("zdx: test: CAC prototype menu item {}", id);
      call(zdx_837C8040);
      const uint32_t result = call.c.r3.u32;
      if (result == 2) {  // the menu system closes the menu on this result
        GuestCall pop(ctx, base);
        pop.c.r3.u64 = REX_LOAD_U32(gm + 48);
        pop(rex_cMenuSys_popMenu_QAAXXZ_82D84950);
      }
      REXLOG_INFO("zdx: test: item {} result {}", id, result);
      return;
    }
    if (frames != 600) return;
    GuestCall call(ctx, base, 0x40);
    REX_STORE_U32(call.scratch + 12, uint32_t(item_cvar - 100));
    call.c.r3.u64 = 0;
    call.c.r4.u64 = call.scratch;
    call.c.r5.u64 = 0;
    REXLOG_INFO("zdx: test: Character Debug Menu item {} (start only)", item_cvar - 100);
    call(rex_Character_CharacterDebugMenu_onCommand_UAA_AW4tMIR_PAVcMenuItem_H_Z_8284E460);
    return;
  }
  const int item = item_cvar;
  const uint32_t gm = g_game_module.load();
  const uint32_t mv_menu = gm ? REX_LOAD_U32(Ext(gm, 9)) : 0;
  if (item == 0 && frames == 1500 && mv_menu) {
    // the viewer's menu input (event 1): GameModule::LoadMarqueeViewerMenu
    GuestCall call(ctx, base);
    call.c.r3.u64 = gm;
    REXLOG_INFO("zdx: test: open the marquee viewer menu");
    call(zdx_837A9668);
    return;
  }
  const uint32_t cac_menu = gm ? REX_LOAD_U32(Ext(gm, 9) + 4) : 0;
  if (item == 3 && gm && frames >= 600) {
    // every sub-menu of "AI Paths": push it for 240 frames, then pop it
    static uint32_t idx = 0, pushed = 0;
    const uint32_t ai = g_ai_menu.load(), ms = REX_LOAD_U32(gm + 48);
    if (!ai || (frames - 600) % 240 != 0) return;
    GuestCall call(ctx, base);
    if (pushed) {
      call.c.r3.u64 = ms;
      call(rex_cMenuSys_popMenu_QAAXXZ_82D84950);
      pushed = 0;
      return;
    }
    const uint32_t n = REX_LOAD_U32(ai + 4);
    for (; idx < n; ++idx) {
      const uint32_t it = REX_LOAD_U32(ai + 16 + 4 * idx);
      if (it && REX_LOAD_U32(it) == 0x82301F40u && REX_LOAD_U32(it + 24)) break;  // cSubMenuItem
    }
    if (idx >= n) return;
    const uint32_t sub = REX_LOAD_U32(REX_LOAD_U32(ai + 16 + 4 * idx) + 24);
    const uint32_t label = REX_LOAD_U32(REX_LOAD_U32(ai + 16 + 4 * idx) + 20);
    REXLOG_INFO("zdx: test: AI Paths item {} '{}' -> sub-menu {:08X}", idx,
                label ? reinterpret_cast<const char*>(REX_RAW_ADDR(label)) : "", sub);
    if (REX_LOAD_U32(ms) == 0) {  // open the debug menu first
      call.c.r3.u64 = gm;
      call.c.r4.u64 = REX_LOAD_U32(gm + 52);
      call.c.r5.u64 = uint64_t(int64_t(-1));
      call(rex_Sk8_GameModule_ActivateMenu_QAAXPAVcMenu_H_Z_82835D28);
    }
    GuestCall push(ctx, base);
    push.c.r3.u64 = ms;
    push.c.r4.u64 = sub;
    push(rex_cMenuSys_pushMenu_QAAXPAVcMenu_Z_82D84848);
    pushed = sub;
    ++idx;
    return;
  }
  if (item == 2 && (frames == 600 || frames == 900) && gm) {
    // the in-game debug menu (GameModule+52), then its "AI Paths" sub-menu
    GuestCall call(ctx, base);
    if (frames == 600) {
      call.c.r3.u64 = gm;
      call.c.r4.u64 = REX_LOAD_U32(gm + 52);
      call.c.r5.u64 = uint64_t(int64_t(-1));
      REXLOG_INFO("zdx: test: open the in-game debug menu");
      call(rex_Sk8_GameModule_ActivateMenu_QAAXPAVcMenu_H_Z_82835D28);
    } else if (const uint32_t ai = g_ai_menu.load()) {
      call.c.r3.u64 = REX_LOAD_U32(gm + 48);
      call.c.r4.u64 = ai;
      REXLOG_INFO("zdx: test: push the AI Paths menu");
      call(rex_cMenuSys_pushMenu_QAAXPAVcMenu_Z_82D84848);
    }
    return;
  }
  if (item == 1 && frames == 1500 && cac_menu) {
    // the prototype's menu input: GameModule::LoadCACPrototypeMenu
    GuestCall call(ctx, base);
    call.c.r3.u64 = gm;
    REXLOG_INFO("zdx: test: open the CAC prototype menu");
    call(zdx_837A9678);
    return;
  }
  if (item == 1 && frames == 2100 && cac_menu) {
    // its item 0: leave the prototype (CASPrototype command 1, Shutdown)
    GuestCall call(ctx, base, 0x40);
    REX_STORE_U32(call.scratch + 12, 0u);
    call.c.r3.u64 = cac_menu;
    call.c.r4.u64 = call.scratch;
    call.c.r5.u64 = 0;
    REXLOG_INFO("zdx: test: CAC prototype menu item 0 (leave)");
    call(zdx_837C8040);
    return;
  }
  if (item == 0 && frames == 1800 && mv_menu) {
    // its item 3: apply the last animation in the "Apply animation" list
    const uint32_t count = REX_LOAD_U32(mv_menu + 1352);
    if (count) REX_STORE_U32(mv_menu + 1324, count - 1);
    GuestCall call(ctx, base, 0x40);
    REX_STORE_U32(call.scratch + 12, 3u);
    call.c.r3.u64 = mv_menu;
    call.c.r4.u64 = call.scratch;
    call.c.r5.u64 = 0;
    REXLOG_INFO("zdx: test: marquee viewer menu item 3 (apply animation {})", count ? count - 1 : 0);
    call(zdx_837CA7D0);
    return;
  }
  if (item == 0 && frames == 2100 && mv_menu) {
    // its item 0: exit the viewer
    GuestCall call(ctx, base, 0x40);
    REX_STORE_U32(call.scratch + 12, 0u);
    call.c.r3.u64 = mv_menu;
    call.c.r4.u64 = call.scratch;
    call.c.r5.u64 = 0;
    REXLOG_INFO("zdx: test: marquee viewer menu item 0 (exit)");
    call(zdx_837CA7D0);
    return;
  }
  if (frames != 600) return;
  GuestCall call(ctx, base, 0x40);
  REX_STORE_U32(call.scratch + 12, uint32_t(item));   // cMenuItem: id at +12
  call.c.r3.u64 = 0;
  call.c.r4.u64 = call.scratch;
  call.c.r5.u64 = 0;
  REXLOG_INFO("zdx: test: Character Debug Menu item {}", item);
  call(rex_Character_CharacterDebugMenu_onCommand_UAA_AW4tMIR_PAVcMenuItem_H_Z_8284E460);
}
}  // namespace zdx

// Character::MarqueeViewerDebugMenu::onGainFocus (zd 837CAD38): zd's function,
// then the "Apply animation" tags (menu+1364, {name, value}, count at +1352)
// re-pointed at the names' current buffers (eastl::string vector at +1620,
// 16 bytes each: the vector moved them while it grew).
void zdx_host_MarqueeMenuGainFocus(PPCContext& __restrict ctx, uint8_t* base) {
  const uint32_t menu = ctx.r3.u32;
  zdx_837CAD38(ctx, base);
  const uint32_t count = REX_LOAD_U32(menu + 1352), tags = REX_LOAD_U32(menu + 1356);
  const uint32_t names = REX_LOAD_U32(menu + 1620), names_end = REX_LOAD_U32(menu + 1624);
  for (uint32_t i = 0; tags && i < count && names + 16 * i < names_end; ++i)
    REX_STORE_U32(tags + 8 * i, REX_LOAD_U32(names + 16 * i));
}

// ---- Graph::CreateChannel: one channel per name -----------------------------
// zd re-creates its graph channels whenever their owners are rebuilt (a world
// or location load) and releases the old ones. Code that took a copy of a
// handle before the reload (scoped graph timers keep it on the stack) then
// submits into the freed channel, whose memory the new channel of the same
// size has just taken: its sample lists break and Graph::Update spins forever
// (the freeze on location changes). Here every call site of CreateChannel
// (zdx_config HOST) gets the existing channel of that name, so no channel is
// ever destroyed and handles never go stale.
void zdx_host_CreateChannel(PPCContext& __restrict ctx, uint8_t* base) {
  const uint32_t graph = ctx.r3.u32;
  const uint32_t name = ctx.r4.u32;
  if (graph && name) {
    const char* want = reinterpret_cast<const char*>(REX_RAW_ADDR(name));
    const size_t want_len = strnlen(want, 256);
    const uint32_t begin = REX_LOAD_U32(graph + 56), end = REX_LOAD_U32(graph + 60);
    for (uint32_t it = begin; it && it < end; it += 4) {
      const uint32_t ch = REX_LOAD_U32(it);
      if (!ch) continue;
      const uint32_t s = REX_LOAD_U32(ch + 8), e = REX_LOAD_U32(ch + 12);  // name: xstring
      if (e < s || e - s != want_len) continue;
      if (std::memcmp(REX_RAW_ADDR(s), want, want_len) == 0) {
        REXLOG_INFO("zdx: graph channel \"{}\" re-created: reusing {:08X}", want, ch);
        ctx.r3.u64 = ch;
        return;
      }
    }
  }
  zdx_8362D098(ctx, base);  // Graph::CreateChannel (zd)
}

// ---- Debug Graph channels from the command line / toml -------------------
// zdx_graph_enable="Render Time;Max Update Time" turns graph channels on (as
// the "Debug Graph" menu does, Graph::EnableChannel) once they exist and
// shows the graph; the list is re-applied whenever it changes.
REXCVAR_DEFINE_STRING(zdx_graph_enable, "", "Skate 3 zd debug",
                      "Debug Graph channels to show (names as in the Debug Graph menu, ';'-separated)");

namespace zdx {

void ApplyGraphChannels(PPCContext& ctx, uint8_t* base) {
  static std::string applied;
  static int tries = 0;
  const std::string want = REXCVAR_GET(zdx_graph_enable);
  if (want == applied || !Ready()) return;
  if (want.empty()) { applied = want; return; }
  // Graph instance (SingletonHolder<Debug::Graph>::Instance, zd 0x836AB1E8).
  GuestCall inst(ctx, base);
  inst(zdx_836AB1E8);
  const uint32_t graph = inst.c.r3.u32;
  if (!graph) return;
  // and show the graph (Graph+72, as Lua's DebugEnableGraph(true) sets it)
  REX_STORE_U8(graph + 72, 1);
  bool all = true;
  size_t p = 0;
  while (p <= want.size()) {
    size_t q = want.find(';', p);
    if (q == std::string::npos) q = want.size();
    const std::string name = want.substr(p, q - p);
    p = q + 1;
    if (name.empty()) continue;
    GuestCall call(ctx, base, 0x100);
    std::memcpy(REX_RAW_ADDR(call.scratch), name.c_str(), std::min<size_t>(name.size() + 1, 0xFF));
    REX_STORE_U8(call.scratch + 0xFF, 0);
    call.c.r3.u64 = graph;
    call.c.r4.u64 = call.scratch;
    call.c.r5.u64 = 1;
    call(zdx_83629320);  // Graph::EnableChannel(name, true)
    GuestCall chk(ctx, base, 0x100);
    std::memcpy(REX_RAW_ADDR(chk.scratch), name.c_str(), std::min<size_t>(name.size() + 1, 0xFF));
    chk.c.r3.u64 = graph;
    chk.c.r4.u64 = chk.scratch;
    chk(zdx_83629378);   // Graph::IsChannelEnabled(name)
    all = all && (chk.c.r3.u32 & 0xFF) != 0;
  }
  // channels appear as their owners are built: retry for a while
  if (all || ++tries > 600) {
    applied = want;
    tries = 0;
  }
}

}  // namespace zdx
