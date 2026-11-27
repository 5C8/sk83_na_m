// Living-world job critical path helpers (EA::Jobs LocalBackend).
#include "generated/sk83_na_m_init.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <map>
#include <mutex>
#include <tuple>
#include <vector>
#include <cstring>

#include <rex/system/kernel_state.h>

// Scene culling jobs (VectorizedCullingJob, BufferedIterativeJobEntryPoint 0x82974450) are
// created with affinity 4, the same as the animation command system's jobs. A thread waiting in
// AnimJobScheduler::WaitForJob joins the local queue with affinity 4 and so could pick up a
// culling batch: the living-world job (the render frame's critical path once the audio update
// runs on its own thread) then ran culling for 59 us per frame on average, 350+ us in the worst
// 10% of frames. With affinity 8 the culling batches are run by the job threads (affinity 0x3F)
// only. 0x82AB7120 is a folded `stw r4,20(r3)` setter (Job description affinity); only the
// three SceneRenderView call sites are changed, identified by their return address.
REXCVAR_DEFINE_BOOL(sk83_cull_jobs_job_threads_only, true, "Skate 3",
                    "Scene culling jobs run on the job threads only, never inside an animation "
                    "wait (keeps them off the living-world job's critical path).")
    .lifecycle(rex::cvar::Lifecycle::kHotReload);

extern "C" REX_FUNC(rex_Fesl_GameManagerParametersImpl_SetMaxPossibleGamePlayerCount_UAAXH_Z_82AB7120) {
  if (ctx.r4.u32 == 4 && (ctx.lr == 0x829A7268u || ctx.lr == 0x829A72B8u || ctx.lr == 0x829A8114u) &&
      REXCVAR_GET(sk83_cull_jobs_job_threads_only)) {
    ctx.r4.u64 = 8;
  }
  __imp__rex_Fesl_GameManagerParametersImpl_SetMaxPossibleGamePlayerCount_UAAXH_Z_82AB7120(ctx, base);
}

// ---- EA::Jobs::LocalBackend::RunSingleJob(JobAffinity) 0x82ABA558: native ------------------
// The job pick every job thread and every waiter (AnimJobScheduler::WaitForJob ->
// JobScheduler::JoinLocalQueueForOneJob, vtable +96) runs in a loop. It scans the whole 64-bit
// slot array each call; recompiled, that scan is the dominant EA::Jobs cost (1% of all process
// samples) and sits between every pair of dependent animation jobs on the living-world job's
// critical path. Transcribed from the listing (recomp.51, 0x82ABA558):
//   slots: u64 array at [be+8], count [be+16]; JobInstance records [be+4] + i*656.
//   slot word hi: state = hi >> 29 (0 ready, 1 running, 3 done, ...), exclusion mask
//   (hi >> 23) & 0x3F; a slot is eligible for affinity a when a & ~mask.
//   scan: found = any slot ready && eligible; best = argmin over ALL slots of
//         (word | notaff) with notaff = ((~a << 23) & 0x1F800000) << 32 (unsigned 64-bit).
//   none found / no best -> return 0. Re-check best (ready + eligible, else rescan), CAS its word
//   from state 0 to state 1 (fail -> rescan), run JobInstance::Run(rec), then
//     rec[41] != 0: Increment(rec+624); CAS running -> done (state 3);
//                   rec[41] != 0 -> Semaphore::Post(rec+628, 1); Decrement(rec+624)
//     else:         CAS running -> (debug byte [[be+1588]] ? done : ~0 = free slot)
//   then the backend's vtable +28 (r3 = be); return 1.
// The original's debug trace path ([[be+1588]] set: snprintf of the job before running it) is
// left to the recompiled body. 64-bit CASes are host compare-exchanges on guest memory, exactly
// how the recompiler lowers ldarx/stdcx., so this interoperates with recompiled job code.
REXCVAR_DEFINE_BOOL(sk83_native_runsinglejob, true, "Skate 3",
                    "Native EA::Jobs LocalBackend::RunSingleJob (the job pick of every job thread "
                    "and job waiter).")
    .lifecycle(rex::cvar::Lifecycle::kHotReload);
REXCVAR_DEFINE_BOOL(sk83_native_runsinglejob_verify, false, "Skate 3",
                    "DIAGNOSTIC: every 64th call, run the recompiled and the native RunSingleJob on "
                    "two private copies of the live slot array and compare.")
    .lifecycle(rex::cvar::Lifecycle::kHotReload);

REXCVAR_DEFINE_BOOL(sk83_native_runsinglejob_selftest, false, "Skate 3",
                    "DIAGNOSTIC: the verify fixture runs the recompiled function on both copies.")
    .lifecycle(rex::cvar::Lifecycle::kHotReload);

namespace {
inline bool Cas64(uint8_t* base, uint32_t a, uint64_t expect, uint64_t desired) {
  return __sync_bool_compare_and_swap(reinterpret_cast<uint64_t*>(REX_RAW_ADDR(a)),
                                      __builtin_bswap64(expect), __builtin_bswap64(desired));
}
struct Pick {
  int32_t best;  // -1 none
  bool found;
};
// The selection scan, exactly as the guest's loop at 0x82ABA584.
inline Pick Scan(uint8_t* base, uint32_t be, uint32_t aff) {
  const uint64_t notaff = uint64_t((~aff << 23) & 0x1F800000u) << 32;
  const uint32_t n = REX_LOAD_U32(be + 16);
  const uint32_t arr = REX_LOAD_U32(be + 8);
  Pick p{-1, false};
  uint64_t bestv = ~0ull;
  const volatile uint64_t* w = reinterpret_cast<const volatile uint64_t*>(REX_RAW_ADDR(arr));
  for (int32_t i = 0; i < int32_t(n); ++i) {
    const uint64_t v = __builtin_bswap64(w[i]);
    const uint32_t hi = uint32_t(v >> 32);
    if ((aff & ~((hi >> 23) & 0x3Fu) & 0xFFu) != 0 && (hi & 0xE0000000u) == 0) p.found = true;
    const uint64_t o = v | notaff;
    if (o < bestv) {
      bestv = o;
      p.best = i;
    }
  }
  return p;
}
// Verify fixture (sk83_native_runsinglejob_verify): a private fake backend in guest memory.
// Every 64th call copies the live slot array into it twice and runs the recompiled function on
// one copy and the native one on the other (same affinity), then compares the return value, the
// record each picked and both arrays afterwards. Race free: nothing else sees the fake arrays.
// The JobInstance::Run hook does not run fake records; the fake vtable's +28 is a harmless guest
// leaf (GetCurrentThreadId).
constexpr uint32_t kFakeMaxSlots = 4096;
struct Fake {
  uint32_t be = 0, arr = 0, recs = 0, recs_end = 0;
};
Fake g_fake;
std::mutex g_fake_mu;
thread_local uint32_t t_fake_ran = 0;  // fake record the last fake Run got
std::atomic<uint64_t> g_fx_runs{0}, g_fx_bad{0}, g_fx_picks{0};

bool FakeInit(uint8_t* base) {
  if (g_fake.be) return true;
  auto* mem = rex::system::kernel_memory();
  const uint32_t be = mem->SystemHeapAlloc(0x1000, 0x80);
  const uint32_t arr = mem->SystemHeapAlloc(kFakeMaxSlots * 8 * 2, 0x80);
  const uint32_t recs = mem->SystemHeapAlloc(kFakeMaxSlots * 656, 0x80);
  if (!be || !arr || !recs) return false;
  std::memset(REX_RAW_ADDR(be), 0, 0x1000);
  std::memset(REX_RAW_ADDR(recs), 0, kFakeMaxSlots * 656);
  REX_STORE_U32(be + 0, be + 0x800);              // vtable
  REX_STORE_U32(be + 0x800 + 28, 0x82DAF348u);    // GetCurrentThreadId: harmless leaf
  REX_STORE_U32(be + 4, recs);
  REX_STORE_U32(be + 1588, be + 0x7F0);           // debug flag byte (0)
  g_fake = {be, arr, recs, recs + kFakeMaxSlots * 656};
  return true;
}
}  // namespace

// The JobInstance::Run hook (sk83_job_timeline.cpp) asks first: fake records are not run.
bool Sk83RunSingleJobFakeRun(uint32_t rec) {
  if (rec >= g_fake.recs && rec < g_fake.recs_end && g_fake.recs) {
    t_fake_ran = rec;
    return true;
  }
  return false;
}

namespace {
void NativeRunSingleJob(PPCContext& ctx, uint8_t* base, uint32_t be, uint32_t aff) {
  uint32_t slot = 0, rec = 0;
  uint64_t running = 0, done = 0;
  for (;;) {
    const Pick p = Scan(base, be, aff);
    if (!p.found || p.best < 0) {
      ctx.r3.u64 = 0;
      return;
    }
    slot = REX_LOAD_U32(be + 8) + uint32_t(p.best) * 8u;
    const uint64_t w = REX_LOAD_U64(slot);
    const uint32_t hi = uint32_t(w >> 32), lo = uint32_t(w);
    if ((hi & 0xE0000000u) != 0) continue;
    if ((aff & ~((hi >> 23) & 0x3Fu) & 0xFFu) == 0) continue;
    const uint32_t keep = hi & 0x1FFFFFFFu;
    const uint64_t expect = (uint64_t(keep) << 32) | lo;
    running = (uint64_t(keep | 0x20000000u) << 32) | lo;
    if (!Cas64(base, slot, expect, running)) continue;
    done = (uint64_t(keep | 0x60000000u) << 32) | lo;
    rec = REX_LOAD_U32(be + 4) + uint32_t(p.best) * 656u;
    break;
  }
  ctx.r3.u64 = rec;
  ctx.lr = 0x82ABA750;
  rex_EA_Jobs_LocalBackend_JobInstance_Run_QAAXXZ_82ABA3B8(ctx, base);
  if (REX_LOAD_U8(rec + 41) != 0) {
    ctx.r3.u64 = rec + 624;
    ctx.lr = 0x82ABA774;
    rex_EA_Jobs_Detail_ReferenceCount_Increment_QAA_NXZ_82AB8030(ctx, base);
    Cas64(base, slot, running, done);
    if (REX_LOAD_U8(rec + 41) != 0) {
      ctx.r3.u64 = rec + 628;
      ctx.r4.u64 = 1;
      ctx.lr = 0x82ABA7C8;
      rex_EA_Thread_Semaphore_Post_QAAHH_Z_82C397E0(ctx, base);
    }
    ctx.r3.u64 = rec + 624;
    ctx.lr = 0x82ABA7D0;
    rex_EA_Jobs_Detail_ReferenceCount_Decrement_QAA_NXZ_82AB8098(ctx, base);
  } else if (REX_LOAD_U8(REX_LOAD_U32(be + 1588)) != 0) {
    Cas64(base, slot, running, done);
  } else {
    Cas64(base, slot, running, ~0ull);
  }
  ctx.r3.u64 = be;
  const uint32_t fn = REX_LOAD_U32(REX_LOAD_U32(be + 0) + 28);
  ctx.ctr.u64 = fn;
  ctx.lr = 0x82ABA85C;
  REX_CALL_INDIRECT_FUNC(fn);
  ctx.r3.u64 = 1;
}

void FixtureCheck(PPCContext& ctx, uint8_t* base, uint32_t be, uint32_t aff) {
  std::lock_guard<std::mutex> lock(g_fake_mu);
  if (!FakeInit(base)) return;
  const uint32_t n = REX_LOAD_U32(be + 16);
  if (n == 0 || n > kFakeMaxSlots) return;
  const uint32_t live = REX_LOAD_U32(be + 8);
  // external-writer check: the fake arrays must still hold what the last run left
    static std::vector<uint8_t> s_last;
    static std::atomic<int> s_ext{0};
    if (!s_last.empty() && std::memcmp(REX_RAW_ADDR(g_fake.arr), s_last.data(), s_last.size()) != 0 &&
        s_ext.fetch_add(1) < 5) {
      size_t q = 0;
      while (REX_RAW_ADDR(g_fake.arr)[q] == s_last[q]) ++q;
      REXLOG_WARN("runsinglejob fixture: fake array changed while idle at +{:X}", q);
    }
    struct Saver {
      uint8_t* b; uint32_t a;
      ~Saver() { s_last.assign(b + a, b + a + kFakeMaxSlots * 8 * 2); }
    } saver{base, g_fake.arr};
  const uint32_t a0 = g_fake.arr, a1 = g_fake.arr + kFakeMaxSlots * 8;
  // One snapshot of the (concurrently changing) live array into a host buffer, then both copies
  // from it. Copying a1 from a0 is NOT the same: the compiler may forward memcpy(a1, a0) to
  // memcpy(a1, live), which other threads keep changing (that made the copies differ).
  static thread_local std::vector<uint8_t> snap;
  snap.resize(n * 8);
  std::memcpy(snap.data(), REX_RAW_ADDR(live), n * 8);
  asm volatile("" ::: "memory");
  std::memcpy(REX_RAW_ADDR(a0), snap.data(), n * 8);
  std::memcpy(REX_RAW_ADDR(a1), snap.data(), n * 8);
  REX_STORE_U32(g_fake.be + 16, n);
  static thread_local std::vector<uint64_t> pre;
  pre.assign(reinterpret_cast<const uint64_t*>(REX_RAW_ADDR(a0)), reinterpret_cast<const uint64_t*>(REX_RAW_ADDR(a0)) + n);
  const bool pre_eq = std::memcmp(REX_RAW_ADDR(a0), REX_RAW_ADDR(a1), n * 8) == 0;
  if (!pre_eq) {
    static std::atomic<int> s_pl{0};
    if (s_pl.fetch_add(1) < 6) {
      uint32_t k = 0;
      while (k < n && REX_LOAD_U64(a0 + k * 8) == REX_LOAD_U64(a1 + k * 8)) ++k;
      REXLOG_WARN("runsinglejob fixture: copies differ before running, slot {}: a0 {:016X} a1 {:016X} live {:016X} pre {:016X}",
                  k, REX_LOAD_U64(a0 + k * 8), REX_LOAD_U64(a1 + k * 8), REX_LOAD_U64(live + k * 8),
                  __builtin_bswap64(pre[k]));
    }
  }
  static std::atomic<int> s_addr_log{0};
  if (s_addr_log.fetch_add(1) < 3)
    REXLOG_INFO("runsinglejob fixture: live be {:08X} arr {:08X} n {} recs {:08X} | fake be {:08X} arr {:08X}/{:08X} recs {:08X}",
                be, live, n, REX_LOAD_U32(be + 4), g_fake.be, a0, a1, g_fake.recs);
  PPCContext g = ctx;
  // recompiled on copy 0
  REX_STORE_U32(g_fake.be + 8, a0);
  g.r3.u64 = g_fake.be;
  g.r4.u64 = aff;
  t_fake_ran = 0;
  __imp__rex_EA_1Jobs_LocalBackend_RunSingleJob_UAA_NW4JobAffinity_23_Z_82ABA558(g, base);
  const uint32_t g_ret = g.r3.u32, g_ran = t_fake_ran;
  // native on copy 1
  REX_STORE_U32(g_fake.be + 8, a1);
  PPCContext m = ctx;
  t_fake_ran = 0;
  if (REXCVAR_GET(sk83_native_runsinglejob_selftest)) {
    m.r3.u64 = g_fake.be;
    m.r4.u64 = aff;
    __imp__rex_EA_1Jobs_LocalBackend_RunSingleJob_UAA_NW4JobAffinity_23_Z_82ABA558(m, base);
  } else {
    NativeRunSingleJob(m, base, g_fake.be, aff);
  }
  const uint32_t m_ret = m.r3.u32, m_ran = t_fake_ran;
  const bool same = g_ret == m_ret && g_ran == m_ran &&
                    std::memcmp(REX_RAW_ADDR(a0), REX_RAW_ADDR(a1), n * 8) == 0;
  const uint64_t c = g_fx_runs.fetch_add(1) + 1;
  if (g_ran) g_fx_picks.fetch_add(1);
  if (!same && g_fx_bad.fetch_add(1) < 10) {
    auto changed = [&](uint32_t arr) {
      uint32_t c = 0;
      for (uint32_t q = 0; q < n; ++q) c += std::memcmp(REX_RAW_ADDR(arr + q * 8), &pre[q], 8) != 0;
      return c;
    };
    REXLOG_WARN("runsinglejob fixture: pre-equal {} changed slots: copy0 {} copy1 {}", pre_eq, changed(a0), changed(a1));
    uint32_t k = 0;
    while (k < n && REX_LOAD_U64(a0 + k * 8) == REX_LOAD_U64(a1 + k * 8)) ++k;
    REXLOG_WARN("runsinglejob fixture: DIFF n {} aff {:02X}: guest ret {} rec {:08X} | native ret {} rec {:08X} | slot {} guest {:016X} native {:016X}",
                n, aff, g_ret, g_ran, m_ret, m_ran, k, k < n ? REX_LOAD_U64(a0 + k * 8) : 0,
                k < n ? REX_LOAD_U64(a1 + k * 8) : 0);
  }
  if ((c & 0x3FFF) == 0)
    REXLOG_INFO("runsinglejob fixture: {} runs ({} picked a job), {} differ", c, g_fx_picks.load(),
                g_fx_bad.load());
}
std::atomic<uint64_t> g_rsj_calls{0};
}  // namespace

extern "C" REX_FUNC(rex_EA_1Jobs_LocalBackend_RunSingleJob_UAA_NW4JobAffinity_23_Z_82ABA558) {
  const uint32_t be = ctx.r3.u32;
  const uint32_t aff = ctx.r4.u32 & 0xFFu;
  if (REXCVAR_GET(sk83_native_runsinglejob_verify) && be != g_fake.be &&
      (g_rsj_calls.fetch_add(1, std::memory_order_relaxed) & 63) == 0) {
    FixtureCheck(ctx, base, be, aff);
  }
  if (!REXCVAR_GET(sk83_native_runsinglejob) || REX_LOAD_U8(REX_LOAD_U32(be + 1588)) != 0) {
    __imp__rex_EA_1Jobs_LocalBackend_RunSingleJob_UAA_NW4JobAffinity_23_Z_82ABA558(ctx, base);
    return;
  }
  NativeRunSingleJob(ctx, base, be, aff);
}

// Timeline FN marks (--sk83_job_timeline) for the living-world job body's serial parts.
namespace sk83_job_timeline {
int64_t FnBegin();
void FnEnd(uint32_t addr, int64_t t0);
}  // namespace sk83_job_timeline
#define LW_TIMELINE_HOOK(sym, addr)                    \
  extern "C" REX_FUNC(sym) {                           \
    const int64_t jt0_ = sk83_job_timeline::FnBegin(); \
    __imp__##sym(ctx, base);                           \
    sk83_job_timeline::FnEnd(addr, jt0_);              \
  }
LW_TIMELINE_HOOK(rex_AnimCmdSys_AnimCommandSystem_EndCommandSet_QAAXXZ_82A62628, 0x82A62628u)
LW_TIMELINE_HOOK(rex_AnimCmdSys_AnimCommandSystem_LoadCmdBufferInfo_QAA_NPBD_Z_82A5E030, 0x82A5E030u)
LW_TIMELINE_HOOK(rex_Character_Customizations_DeserializeEmbeddedStamps_AAAXAAVByteStream_Sk8_Z_829464E8, 0x829464E8u)
LW_TIMELINE_HOOK(rex_Sk8_cLivingWorldPresEntityManager_EvalNoSimPedestrians_AAAXXZ_82946878, 0x82946878u)
LW_TIMELINE_HOOK(rex_Sk8_cLivingWorldPresEntityManager_UpdateFullHandFacePedestrians_AAAXXZ_82946620, 0x82946620u)
LW_TIMELINE_HOOK(sub_82A524F8, 0x82A524F8u)
LW_TIMELINE_HOOK(rex_AnimCmdSys_AnimCommandSystem_BeginCommands_QAAXXZ_82A52498, 0x82A52498u)
LW_TIMELINE_HOOK(rex_AnimCmdSys_AnimCommandSystem_EndCommands_QAAIXZ_82A58148, 0x82A58148u)
LW_TIMELINE_HOOK(rex_D3D_CDevice_BlockOnFence_QAAXKW4_D3DBLOCKTYPE_PAUD3DResource_N_Z_82D8D5D8, 0x82D8D5D8u)
