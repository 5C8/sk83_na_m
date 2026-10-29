// Character Debug Menu > "Start CAC Prototype Viewer" (zd's CASPrototype).
//
// zd: Simulation::Workspace::InitWorkspace makes Character::CASPrototype (a
// holder of the 14816-byte CASPrototypeImpl) in workspace +96 (Simulation
// +264), ShutWorkspace destroys it, and Simulation::PostUpdate runs
// CASPrototype::Update(sim dt) -- skipping CASSimManager::Update while the
// prototype is active. The menu item queues CASPrototype command 0
// (transplanted CharacterDebugMenu::onCommand); the impl's controller input
// opens CACPrototypeDebugMenu (GameModule::LoadCACPrototypeMenu).
// m: InitWorkspace leaves workspace +96 (m Simulation +208) empty and its
// PostUpdate has no prototype update. Both are done here around m's functions
// with zd's code (zdx_config: "Start CAC Prototype Viewer").

#include <atomic>
#include <cstring>

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"
#include "generated/sk83_na_m_init.h"

#include <rex/logging.h>

namespace {
constexpr uint32_t kWorkspaceProto = 96;     // Workspace: CASPrototype*
constexpr uint32_t kSimulationProto = 208;   // m Simulation (zd +264)
constexpr uint32_t kSimDt = 0x82222D04u;     // m: the sim step PostUpdate passes to CASSimManager::Update
std::atomic<uint32_t> g_proto{0};

bool ProtoActive(uint8_t* base) {
  const uint32_t p = g_proto.load();
  const uint32_t impl = p ? REX_LOAD_U32(p) : 0;
  return impl && REX_LOAD_U8(impl + 64) != 0;   // CASPrototype::IsActive (zd 835D1778)
}
}  // namespace

// Sk8::Simulation::Workspace::InitWorkspace(Simulation*, cLocationManager&) (m 0x829F8688).
extern "C" REX_FUNC(rex_Sk8_Simulation_Workspace_InitWorkspace_QAAXPAV23_AAVcLocationManager_3_Z_829F8688) {
  const uint32_t ws = ctx.r3.u32;
  __imp__rex_Sk8_Simulation_Workspace_InitWorkspace_QAAXPAV23_AAVcLocationManager_3_Z_829F8688(ctx, base);
  if (!zdx::PoolReady(base) || !ws || REX_LOAD_U32(ws + kWorkspaceProto)) return;
  const uint32_t holder = zdx::AllocZeroed(base, 4);
  if (!holder) return;
  {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = holder;
    call(zdx_835F4600);  // Character::CASPrototype::CASPrototype()
  }
  REX_STORE_U32(ws + kWorkspaceProto, holder);
  g_proto = holder;
  REXLOG_INFO("zdx: CASPrototype {:08X} (impl {:08X}) in workspace {:08X}", holder, REX_LOAD_U32(holder), ws);
}

// Sk8::Simulation::Workspace::ShutWorkspace() (m 0x829F9920).
extern "C" REX_FUNC(rex_Sk8_Simulation_Workspace_ShutWorkspace_QAAXXZ_829F9920) {
  const uint32_t ws = ctx.r3.u32;
  const uint32_t holder = ws ? REX_LOAD_U32(ws + kWorkspaceProto) : 0;
  if (holder && holder == g_proto.load()) {
    zdx::GuestCall call(ctx, base);
    call.c.r3.u64 = holder;
    call(zdx_835D1708);  // Character::CASPrototype::~CASPrototype()
    REX_STORE_U32(ws + kWorkspaceProto, 0);
    g_proto = 0;
  }
  __imp__rex_Sk8_Simulation_Workspace_ShutWorkspace_QAAXXZ_829F9920(ctx, base);
}

// Sk8::Simulation::PostUpdate(SimulationFlags) (m 0x829F0310).
extern "C" REX_FUNC(rex_Sk8_Simulation_PostUpdate_QAAXW4SimulationFlags_2_Z_829F0310) {
  const uint32_t sim = ctx.r3.u32;
  __imp__rex_Sk8_Simulation_PostUpdate_QAAXW4SimulationFlags_2_Z_829F0310(ctx, base);
  const uint32_t holder = sim ? REX_LOAD_U32(sim + kSimulationProto) : 0;
  if (!zd_debug::Enabled() || !holder || holder != g_proto.load()) return;
  zdx::GuestCall call(ctx, base);
  call.c.r3.u64 = holder;
  const uint32_t dt = REX_LOAD_U32(kSimDt);
  float f;
  std::memcpy(&f, &dt, 4);
  call.c.f1.f64 = f;
  call(zdx_835FAC68);  // Character::CASPrototype::Update(float)
}

// Character::CASSimManager::Update(float) (m 0x826B6020): Simulation::
// PostUpdate's call is skipped while the prototype runs (zd's PostUpdate skips
// it then: CASPrototypeImpl::Update updates the CASSimManager itself).
constexpr uint32_t kPostUpdateCasSimUpdateReturn = 0x829F05D8u;
extern "C" REX_FUNC(rex_Character_CASSimManager_Update_QAAXM_Z_826B6020) {
  if (zd_debug::Enabled() && uint32_t(ctx.lr) == kPostUpdateCasSimUpdateReturn && ProtoActive(base)) return;
  __imp__rex_Character_CASSimManager_Update_QAAXM_Z_826B6020(ctx, base);
}

namespace zdx {
// Test helper: queue a viewer command in the prototype's input handler
// (impl+256), as ViewerInputHandler::Update does on a pad event: the deque's
// fast path {cmd, value} at its end pointer (+28), bounded by +36.
bool TestPushProtoCommand(uint8_t* base, uint32_t cmd, float value) {
  const uint32_t holder = g_proto.load();
  const uint32_t impl = holder ? REX_LOAD_U32(holder) : 0;
  if (!impl) return false;
  const uint32_t h = impl + 256;
  const uint32_t end = REX_LOAD_U32(h + 28), cap = REX_LOAD_U32(h + 36);
  if (!end || end + 8 == cap) return false;
  uint32_t bits;
  std::memcpy(&bits, &value, 4);
  REX_STORE_U32(end, cmd);
  REX_STORE_U32(end + 4, bits);
  REX_STORE_U32(h + 28, end + 8);
  return true;
}
}  // namespace zdx
