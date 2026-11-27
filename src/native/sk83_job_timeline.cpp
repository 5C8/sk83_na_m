// DIAGNOSTIC: per-thread begin/end timeline of the living-world presentation job chain and every
// EA::Jobs job run, for a short window (cvar sk83_job_timeline = frames to record, after
// sk83_job_timeline_delay seconds). Written to logs/jobtimeline_<ts>.txt as raw events:
//   <thread> <kind> <tag hex> <t0 us> <t1 us>
// kinds: LWADD  render thread starts the living-world job (sub_82943BD0)
//        LWWAIT render thread waits for it (sub_82943AD8)
//        LWJOB  the job body (sub_829478C0)
//        SYNC   AnimCommandSystem::Sync inside it
//        ADDTREE JobScheduler::AddTree (scheduling)
//        JOB    LocalBackend::JobInstance::Run, tag = the job's entry point (job+44)
//        RFRAME sub_82928208 (render frame: LW wait + crowd SyncAnim)
#include "generated/sk83_na_m_init.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

REXCVAR_DEFINE_INT32(sk83_job_timeline, 0, "Skate 3",
                     "DIAGNOSTIC: record a job timeline for this many render frames "
                     "(logs/jobtimeline_<ts>.txt)");
REXCVAR_DEFINE_DOUBLE(sk83_job_timeline_delay, 60.0, "Skate 3",
                      "DIAGNOSTIC: seconds after start before the job timeline records");

namespace {

enum Kind : uint8_t { kLwAdd, kLwWait, kLwJob, kSync, kAddTree, kJob, kRFrame, kFetch, kProc, kFn };
const char* const kKindNames[] = {"LWADD", "LWWAIT", "LWJOB", "SYNC", "ADDTREE", "JOB", "RFRAME", "FETCH", "PROC", "FN"};

struct Event {
  uint32_t tid;
  uint8_t kind;
  uint32_t tag;
  int64_t t0, t1;
};

constexpr size_t kMax = 1 << 20;
std::vector<Event> g_events;
std::atomic<size_t> g_n{0};
std::atomic<int> g_state{0};  // 0 idle, 1 recording, 2 done
std::atomic<int> g_frames{0};
const auto g_start = std::chrono::steady_clock::now();

int64_t NowUs() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::steady_clock::now() - g_start)
      .count();
}

void Dump() {
  const size_t n = std::min(g_n.load(), kMax);
  char path[256];
  std::snprintf(path, sizeof(path), "logs/jobtimeline_%lld.txt", (long long)NowUs());
  if (FILE* f = std::fopen(path, "w")) {
    for (size_t i = 0; i < n; ++i) {
      const Event& e = g_events[i];
      std::fprintf(f, "%u %s %08x %lld %lld\n", e.tid, kKindNames[e.kind], e.tag, (long long)e.t0,
                   (long long)e.t1);
    }
    std::fclose(f);
  }
  REXLOG_INFO("job timeline: {} events -> {}", n, path);
}

// Render-frame marker drives the window.
void OnFrame() {
  const int st = g_state.load(std::memory_order_relaxed);
  if (st == 2) return;
  const int frames = REXCVAR_GET(sk83_job_timeline);
  if (frames <= 0) return;
  if (st == 0) {
    if (NowUs() < int64_t(REXCVAR_GET(sk83_job_timeline_delay) * 1e6)) return;
    g_events.resize(kMax);
    g_state.store(1, std::memory_order_release);
    return;
  }
  if (g_frames.fetch_add(1) + 1 >= frames) {
    g_state.store(2, std::memory_order_release);
    Sleep(5);  // let in-flight events land
    Dump();
  }
}

struct Scope {
  uint8_t kind;
  uint32_t tag;
  int64_t t0;
  bool on;
  Scope(uint8_t k, uint32_t t) : kind(k), tag(t), on(g_state.load(std::memory_order_acquire) == 1) {
    if (on) t0 = NowUs();
  }
  ~Scope() {
    if (!on || g_state.load(std::memory_order_acquire) != 1) return;
    const size_t i = g_n.fetch_add(1, std::memory_order_relaxed);
    if (i < kMax) g_events[i] = {GetCurrentThreadId(), kind, tag, t0, NowUs()};
  }
};

}  // namespace

// FN events (tag = guest address) for hooks owned elsewhere, e.g. the render frame parts.
namespace sk83_job_timeline {
int64_t FnBegin() { return g_state.load(std::memory_order_acquire) == 1 ? NowUs() : -1; }
void FnEnd(uint32_t addr, int64_t t0) {
  if (t0 < 0 || g_state.load(std::memory_order_acquire) != 1) return;
  const size_t i = g_n.fetch_add(1, std::memory_order_relaxed);
  if (i < kMax) g_events[i] = {GetCurrentThreadId(), kFn, addr, t0, NowUs()};
}
}  // namespace sk83_job_timeline

extern "C" REX_FUNC(sub_82928208) {
  OnFrame();
  Scope s(kRFrame, 0);
  __imp__sub_82928208(ctx, base);
}
extern "C" REX_FUNC(sub_82943BD0) {
  Scope s(kLwAdd, 0);
  __imp__sub_82943BD0(ctx, base);
}
extern "C" REX_FUNC(sub_82943AD8) {
  Scope s(kLwWait, 0);
  __imp__sub_82943AD8(ctx, base);
}
extern "C" REX_FUNC(sub_829478C0) {
  Scope s(kLwJob, 0);
  __imp__sub_829478C0(ctx, base);
}
extern "C" REX_FUNC(rex_AnimCmdSys_AnimCommandSystem_Sync_QAAXXZ_82A5F628) {
  Scope s(kSync, ctx.r3.u32);
  __imp__rex_AnimCmdSys_AnimCommandSystem_Sync_QAAXXZ_82A5F628(ctx, base);
}
extern "C" REX_FUNC(rex_EA_Jobs_JobScheduler_AddTree_QAAXAAVJob_23_Z_82AB90E8) {
  Scope s(kAddTree, 0);
  __imp__rex_EA_Jobs_JobScheduler_AddTree_QAAXAAVJob_23_Z_82AB90E8(ctx, base);
}
bool Sk83RunSingleJobFakeRun(uint32_t rec);  // sk83_lw_jobs.cpp: verify fixture records
extern "C" REX_FUNC(rex_EA_Jobs_LocalBackend_JobInstance_Run_QAAXXZ_82ABA3B8) {
  if (Sk83RunSingleJobFakeRun(ctx.r3.u32)) return;
  // tag: entry point low 24 bits | the running guest thread's processor number << 24
  Scope s(kJob, (REX_LOAD_U32(ctx.r3.u32 + 44) & 0xffffffu) |
                    (uint32_t(REX_LOAD_U8(ctx.r13.u32 + 268)) << 24));
  __imp__rex_EA_Jobs_LocalBackend_JobInstance_Run_QAAXXZ_82ABA3B8(ctx, base);
}

// The Andale fetch job thunk (sub_82fec170 -> Vector_ExtractPackedNVBR(types=r4, count=r6,
// decompressor=r5)). FETCH tag = count | distinct sqtBuff << 16; PROC = the calling guest
// thread's processor number (PCR+268, the codec's work-buffer slot), one per call.
extern "C" REX_FUNC(sub_82FEC170) {
  uint32_t tag = 0;
  const bool on = g_state.load(std::memory_order_acquire) == 1;
  if (on) {
    const uint32_t types = ctx.r4.u32, n = ctx.r6.u32;
    uint32_t bufs[512];
    uint32_t nb = 0;
    for (uint32_t i = 0; i < n && i < 512; ++i) {
      const uint32_t b = REX_LOAD_U32(types + i * 0x40 + 0x20);
      bool seen = false;
      for (uint32_t k = 0; k < nb && !seen; ++k) seen = bufs[k] == b;
      if (!seen) bufs[nb++] = b;
    }
    tag = (n & 0xffff) | (nb << 16);
    const size_t i = g_n.fetch_add(1, std::memory_order_relaxed);
    const int64_t t = NowUs();
    if (i < kMax) g_events[i] = {GetCurrentThreadId(), kProc, uint32_t(REX_LOAD_U8(ctx.r13.u32 + 268)), t, t};
  }
  Scope s(kFetch, tag);
  __imp__sub_82FEC170(ctx, base);
}
