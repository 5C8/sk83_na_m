// DIAGNOSTIC: navmeshcontroller.cpp(650) assertion during challenge loads.
//
// ToggleOffboardState::ResolveOffBoardStatus (0x825C1D10, run from the AI
// skater's SkaterAnimPhase1Job) checks the controller's NavPower path
// (controller + 160) with sub_82AD9118 and replans (rex_Sk8_AI_NavMeshController_TestForStuckSkater_AAAXXZ_825B4ED8) when it is
// invalid, then follows it through rex_Sk8_AI_NavMeshController_HandleWipeouts_AAAXXZ_825BE378, which asserts the SAME
// validity (line 650). A path segment is invalid once the graph-handle slot
// it points at (segment + 48) was zeroed: a navmesh tile streamed out
// (arena Unfix of tNavMeshData / tNavMesh2Data). Each validity read takes
// the NavPower lock (0x8337CAB4), but the check -> follow sequence does not
// hold it, so an unload landing in between fails the assertion.
//
// This records, per ResolveOffBoardStatus call on the job thread, whether it
// replanned and the result, logs every navmesh tile unload, and when the
// path is invalid at rex_Sk8_AI_NavMeshController_HandleWipeouts_AAAXXZ_825BE378 logs the controller + segment state and
// the latest unload, so the failing interleaving is on record. It does not
// change game behaviour.

#include "generated/sk83_na_m_init.h"

#include <rex/hook.h>
#include <rex/logging.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <stdlib.h>

namespace {

uint32_t LoadU32(uint8_t* base, uint32_t addr) {
  uint32_t v;
  std::memcpy(&v, REX_RAW_ADDR(addr), 4);
  return _byteswap_ulong(v);
}

int64_t NowUs() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

// Latest navmesh tile unload (any thread).
std::atomic<int64_t> g_last_unfix_us{0};
std::atomic<uint32_t> g_last_unfix_obj{0};
std::atomic<uint32_t> g_unfix_count{0};

// Per ResolveOffBoardStatus call (job thread).
thread_local bool tl_in_resolve = false;
thread_local int64_t tl_resolve_start_us = 0;
thread_local int tl_replans = 0;
thread_local uint32_t tl_replan_result = 0;
thread_local uint32_t tl_entry_following = 0;

bool PathValid(PPCContext& ctx, uint8_t* base, uint32_t path_field) {
  PPCContext call = ctx;
  call.r3.u64 = path_field;
  sub_82AD9118(call, base);
  return call.r3.u32 != 0;
}

void LogUnfix(uint32_t obj, const char* kind) {
  const uint32_t n = g_unfix_count.fetch_add(1, std::memory_order_relaxed) + 1;
  g_last_unfix_us.store(NowUs(), std::memory_order_relaxed);
  g_last_unfix_obj.store(obj, std::memory_order_relaxed);
  if (n <= 64 || (n & 255u) == 0) {
    REXLOG_INFO("navmesh-diag: {} tile unloaded obj={:08X} (n={})", kind, obj, n);
  }
}

}  // namespace

extern "C" REX_FUNC(
    rex_pegasus_tNavMeshData_Unfix_SAPAUSizeAndAlignment_arena_core_rw_PAXPAU3456_Z_82B26C30) {
  LogUnfix(ctx.r3.u32, "tNavMeshData");
  __imp__rex_pegasus_tNavMeshData_Unfix_SAPAUSizeAndAlignment_arena_core_rw_PAXPAU3456_Z_82B26C30(
      ctx, base);
}

extern "C" REX_FUNC(
    rex_pegasus_tNavMesh2Data_Unfix_SAPAUSizeAndAlignment_arena_core_rw_PAXPAU3456_Z_82B26D20) {
  LogUnfix(ctx.r3.u32, "tNavMesh2Data");
  __imp__rex_pegasus_tNavMesh2Data_Unfix_SAPAUSizeAndAlignment_arena_core_rw_PAXPAU3456_Z_82B26D20(
      ctx, base);
}

extern "C" REX_FUNC(
    rex_unknown_825C1D10) {
  tl_in_resolve = true;
  tl_resolve_start_us = NowUs();
  tl_replans = 0;
  tl_replan_result = 0;
  tl_entry_following = uint8_t(*REX_RAW_ADDR(ctx.r3.u32 + 172));
  __imp__rex_unknown_825C1D10(
      ctx, base);
  tl_in_resolve = false;
}

// NavMeshController replan (r3 = controller): returns the new "following"
// flag (+172).
extern "C" REX_FUNC(rex_Sk8_AI_NavMeshController_TestForStuckSkater_AAAXXZ_825B4ED8) {
  __imp__rex_Sk8_AI_NavMeshController_TestForStuckSkater_AAAXXZ_825B4ED8(ctx, base);
  if (tl_in_resolve) {
    ++tl_replans;
    tl_replan_result = ctx.r3.u32 & 0xFFu;
  }
}

// Follow step (r3 = controller): asserts the path is valid.
extern "C" REX_FUNC(rex_Sk8_AI_NavMeshController_HandleWipeouts_AAAXXZ_825BE378) {
  const uint32_t ctrl = ctx.r3.u32;
  const uint32_t path_field = LoadU32(base, ctrl + 160);
  if (!PathValid(ctx, base, path_field)) {
    const int64_t now = NowUs();
    const int64_t unfix_us = g_last_unfix_us.load(std::memory_order_relaxed);
    REXLOG_ERROR(
        "navmesh-diag: path INVALID at follow: ctrl={:08X} path={:08X} following(entry={} "
        "now={}) seg_index={} replans={} replan_result={} | resolve started {}us ago | last "
        "tile unload {}us ago obj={:08X} (total {})",
        ctrl, path_field, tl_entry_following, uint32_t(*REX_RAW_ADDR(ctrl + 172)),
        LoadU32(base, ctrl + 168), tl_replans, tl_replan_result,
        tl_in_resolve ? now - tl_resolve_start_us : -1,
        unfix_us != 0 ? now - unfix_us : -1, g_last_unfix_obj.load(std::memory_order_relaxed),
        g_unfix_count.load(std::memory_order_relaxed));
    // Segment table (see sub_82AD9118): path = **path_field; segments at
    // *(path + 12), count *(path + 16), 56 bytes each; +4 node, +48 handle.
    const uint32_t holder = path_field != 0 ? LoadU32(base, path_field) : 0;
    const uint32_t path = holder != 0 ? LoadU32(base, holder) : 0;
    if (path != 0) {
      const uint32_t segs = LoadU32(base, path + 12);
      const uint32_t count = LoadU32(base, path + 16);
      for (uint32_t i = 0; i < count && i < 16; ++i) {
        const uint32_t seg = segs + i * 56;
        const uint32_t handle = LoadU32(base, seg + 48);
        REXLOG_ERROR("navmesh-diag:   seg[{}] node={:08X} handle={:08X} -> {:08X}", i,
                     LoadU32(base, seg + 4), handle, handle != 0 ? LoadU32(base, handle) : 0);
      }
    }
  }
  __imp__rex_Sk8_AI_NavMeshController_HandleWipeouts_AAAXXZ_825BE378(ctx, base);
}
