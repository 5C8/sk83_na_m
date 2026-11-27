// Job-release watchdog: find out why a not-ready EA::Jobs job is never
// released (the "freeze" -- main thread and a job worker spinning in
// VTrajectoryBatch::SyncJobs forever).
//
// What a frozen process showed (twice, both in VTrajectoryBatch batches):
// the job a SyncJobs waits on sits in state 2 ("not ready") with its
// enabler counter (first word of its 656-byte JobInstance record) stuck at 1,
// while every other job in the backend is done. The release protocol, read
// from the guest code:
//   Job::INTERNAL_AddNotReady  slot allocated in state 2, then AddBarrier for
//                              the job's own guard event        counter = 1
//   INTERNAL_SubmitEventsAndDeps  per dependency: AddBarrier    counter += 1
//                              and AddEvent on the dependency (appended to
//                              its end-event list, or run at once when the
//                              dependency is already done/played)
//   then Event::Run(guard)                                       counter -= 1
//   each dependency's PlayEventList(end) runs its events         counter -= 1
//   the decrement that reaches 0 stores the enabler -> state 0 (ready).
// One decrement is lost and static reading of every step found no hole, so
// this records the steps as they happen, per job slot, and dumps a slot's
// history once its job has been stuck in state 2 for kStuckSeconds.
//
// Cost: a few relaxed atomics per hooked call; these run a few thousand
// times a frame. Remove once the freeze is understood.

#include "generated/sk83_na_m_init.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <thread>

#include <intrin.h>

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>

REXCVAR_DEFINE_INT32(sk83_job_watch_inject, 0, "Skate 3",
                     "TEST ONLY: N > 0 = once, N seconds after the watch arms, make one job lose a "
                     "dependency release (an extra count on its enabler counter at AddEvent) to "
                     "exercise the diagnosis + repair path");
REXCVAR_DEFINE_BOOL(sk83_job_addevent_fix, true, "Skate 3",
                    "Fix the EA::Jobs lost-release race at its source: LocalBackend::AddEvent "
                    "treated only state 3 (done) as finished, not state 4 (the garbage collector's "
                    "'collecting', which keeps the job id and runs JobInstance::Clear -- resetting "
                    "the played flags and event lists -- under the list lock). An event registered "
                    "then was appended to a dead list and never played (the freeze). Native "
                    "AddEvent, identical except that 3 and 4 both count as finished");
REXCVAR_DEFINE_BOOL(sk83_job_release_repair, true, "Skate 3",
                    "Safety net for the EA::Jobs lost-release freeze: a job stuck not-ready for 1 s "
                    "with its enabler counter at 1 while every dependency it registered on has "
                    "finished gets the release its lost event would have done (logged as "
                    "'[job-watch] REPAIRED')");

namespace {

constexpr uint32_t kMaxSlots = 1024;
constexpr uint32_t kRing = 64;  // per slot, power of two
constexpr double kStuckSeconds = 3.0;
constexpr double kRepairSeconds = 1.0;

enum Op : uint8_t {
  kAlloc = 1,     // a=id lo, b=counter after, c=state hi
  kBarrier,       // a=id lo, b=counter after
  kAddEvent,      // a=dep slot<<16 | when, b=dep id lo, c=counter before, d=after
  kRun,           // Event::Run on this slot's event: c=counter before, d=after
  kPlayed,        // dependency's PlayEventList ran an event releasing this slot:
                  //   a=player slot<<16 | when, b=player id lo, c=counter before
  kPlayAfter,     // after that PlayEventList: d=counter after
  kEnable,        // JobInstanceHandle::Enable: a=id lo
};

struct Entry {
  uint64_t tsc;
  uint32_t tid;
  uint8_t op;
  uint32_t a, b, c, d;
};

struct SlotLog {
  std::atomic<uint32_t> next{0};
  Entry e[kRing];
};

SlotLog g_log[kMaxSlots];

// The dependencies the CURRENT job of a slot registered a release event on
// (AddEvent that appended, i.e. did not run at once). Reset at ALLOC. Written
// by the allocating thread, read by the watch thread (diagnostic snapshots).
struct PendingDeps {
  std::atomic<uint32_t> id_lo{0};
  std::atomic<uint32_t> n{0};
  struct Dep {
    uint32_t slot, id_lo;
    uint64_t tsc;
  } d[8];
};
PendingDeps g_pending[kMaxSlots];

// Every end-event-list play (PlayEventList(1)): who, when, how many events.
struct PlayRec {
  uint64_t t0, t1;
  uint32_t slot, id_lo, count;
};
constexpr uint32_t kPlayRing = 1u << 20;
PlayRec* g_plays = new PlayRec[kPlayRing]();
std::atomic<uint64_t> g_play_next{0};
std::atomic<uint64_t> g_addevent_saves{0};  // see AddEventFixed
std::atomic<uint8_t*> g_base{nullptr};
std::atomic<uint32_t> g_backend{0};  // the LocalBackend (all jobs seen share one)
std::atomic<bool> g_watch_started{false};

// Small per-thread id for the history (cheaper than an OS call per entry and
// keeps <windows.h> out of a file that includes the generated headers).
std::atomic<uint32_t> g_next_tid{1};
thread_local uint32_t t_tid = 0;
inline uint32_t ThreadTag() {
  if (t_tid == 0) {
    t_tid = g_next_tid.fetch_add(1, std::memory_order_relaxed);
  }
  return t_tid;
}

inline uint32_t Load32(uint8_t* base, uint32_t a) {
  uint32_t v;
  std::memcpy(&v, base + a, 4);
  return _byteswap_ulong(v);
}

// Backend layout (LocalBackend): +4 JobInstance records (656 bytes each),
// +8 state words (u64 each), +16 slot count.
struct Layout {
  uint32_t records = 0, states = 0, count = 0;
};
Layout GetLayout(uint8_t* base) {
  Layout l;
  const uint32_t be = g_backend.load(std::memory_order_relaxed);
  if (be == 0) {
    return l;
  }
  l.records = Load32(base, be + 4);
  l.states = Load32(base, be + 8);
  l.count = Load32(base, be + 16);
  if (l.count > kMaxSlots) {
    l.count = kMaxSlots;
  }
  return l;
}

// Slot of an event's target (a state word's high half) or counter (a
// record's first word); kMaxSlots when it is neither.
uint32_t SlotOfTarget(const Layout& l, uint32_t target) {
  if (l.states == 0 || target < l.states || target >= l.states + l.count * 8 ||
      (target - l.states) % 8 != 0) {
    return kMaxSlots;
  }
  return (target - l.states) / 8;
}
uint32_t SlotOfRecord(const Layout& l, uint32_t rec) {
  if (l.records == 0 || rec < l.records || rec >= l.records + l.count * 656 ||
      (rec - l.records) % 656 != 0) {
    return kMaxSlots;
  }
  return (rec - l.records) / 656;
}

void Log(uint32_t slot, Op op, uint32_t a, uint32_t b = 0, uint32_t c = 0,
         uint32_t d = 0) {
  if (slot >= kMaxSlots) {
    return;
  }
  SlotLog& s = g_log[slot];
  const uint32_t i = s.next.fetch_add(1, std::memory_order_relaxed) & (kRing - 1);
  s.e[i] = {__rdtsc(), ThreadTag(), op, a, b, c, d};
}

const char* OpName(uint8_t op) {
  switch (op) {
    case kAlloc: return "ALLOC   ";
    case kBarrier: return "BARRIER ";
    case kAddEvent: return "ADDEVENT";
    case kRun: return "RUN     ";
    case kPlayed: return "PLAYED  ";
    case kPlayAfter: return "PLAYDONE";
    case kEnable: return "ENABLE  ";
  }
  return "?       ";
}

void DumpSlot(uint32_t slot, const char* why) {
  SlotLog& s = g_log[slot];
  const uint32_t n = s.next.load(std::memory_order_relaxed);
  const uint32_t first = n > kRing ? n - kRing : 0;
  REXLOG_WARN("[job-watch] slot {} history ({}; {} entries, oldest first):", slot,
              why, n - first);
  uint64_t t0 = 0;
  for (uint32_t k = first; k < n; ++k) {
    const Entry& e = s.e[k & (kRing - 1)];
    if (t0 == 0) {
      t0 = e.tsc;
    }
    REXLOG_WARN("[job-watch]   +{:>12} tid {:>5} {} a={:08X} b={:08X} c={:08X} d={:08X}",
                e.tsc - t0, e.tid, OpName(e.op), e.a, e.b, e.c, e.d);
  }
}

// For a stuck slot: each dependency its current job registered on, with the
// registration time and every end-list play of that dependency job found in
// the play ring (times relative to the registration, in TSC ticks). A play
// that STARTED before the registration and has no later play is the race:
// the event was appended to a list that had already been (or was being)
// played. Returns true when every registered dependency has finished (its
// slot is done or reused), i.e. nothing can release the job any more.
bool DiagnoseDeps(uint8_t* base, const Layout& l, uint32_t slot, uint32_t id_lo) {
  PendingDeps& pd = g_pending[slot];
  if (pd.id_lo.load(std::memory_order_acquire) != id_lo) {
    REXLOG_WARN("[job-watch]   no dependency record for this job id");
    return false;
  }
  const uint32_t n = std::min<uint32_t>(pd.n.load(std::memory_order_acquire), 8);
  bool all_done = n > 0;
  const uint64_t plays = g_play_next.load(std::memory_order_relaxed);
  for (uint32_t k = 0; k < n; ++k) {
    const PendingDeps::Dep& d = pd.d[k];
    const uint32_t dhi = d.slot < l.count ? Load32(base, l.states + d.slot * 8) : 0;
    const uint32_t dlo = d.slot < l.count ? Load32(base, l.states + d.slot * 8 + 4) : 0;
    const bool finished = dlo != d.id_lo || (dhi >> 29) == 3 || (dhi >> 29) == 7;
    all_done = all_done && finished;
    REXLOG_WARN("[job-watch]   dep {}: slot {} job lo {:08X} registered at tsc {} | slot now "
                "{:08X}{:08X} ({})",
                k, d.slot, d.id_lo, d.tsc, dhi, dlo, finished ? "finished" : "still pending");
    // Its end-list plays still in the ring (newest first, up to 4).
    int shown = 0;
    for (uint64_t i = plays; i-- > 0 && plays - i <= kPlayRing && shown < 4;) {
      const PlayRec& r = g_plays[i & (kPlayRing - 1)];
      if (r.slot == d.slot && r.id_lo == d.id_lo) {
        REXLOG_WARN("[job-watch]     played end list: start {:+} end {:+} ticks vs registration, "
                    "{} events read before the play",
                    int64_t(r.t0 - d.tsc), int64_t(r.t1 - d.tsc), r.count);
        ++shown;
      }
    }
    if (shown == 0) {
      REXLOG_WARN("[job-watch]     no end-list play of that job in the ring");
    }
  }
  return all_done;
}

// Every 100 ms: any slot in state 2 with the same id gets, after
// kRepairSeconds, the dependency diagnosis and (sk83_job_release_repair) the
// lost release; after kStuckSeconds its history is dumped once.
void WatchThread() {
  struct Seen {
    uint64_t word = 0;
    std::chrono::steady_clock::time_point since;
    bool dumped = false;
    bool repaired = false;
  };
  static Seen seen[kMaxSlots];
  auto last_report = std::chrono::steady_clock::now();
  for (;;) {
    if (std::chrono::steady_clock::now() - last_report > std::chrono::seconds(60)) {
      last_report = std::chrono::steady_clock::now();
      const uint64_t saves = g_addevent_saves.exchange(0, std::memory_order_relaxed);
      if (saves != 0) {
        REXLOG_INFO("[job-watch] AddEvent fix: {} release events run at once for a dependency "
                    "being garbage collected (would have been lost) in the last minute", saves);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    uint8_t* base = g_base.load(std::memory_order_relaxed);
    if (base == nullptr) {
      continue;
    }
    const Layout l = GetLayout(base);
    const auto now = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < l.count; ++i) {
      const uint32_t hi = Load32(base, l.states + i * 8);
      const uint32_t lo = Load32(base, l.states + i * 8 + 4);
      const uint64_t word = (uint64_t(hi) << 32) | lo;
      Seen& sn = seen[i];
      if ((hi >> 29) != 2) {
        sn = Seen{};
        continue;
      }
      if (sn.word != word) {
        sn = Seen{word, now, false};
        continue;
      }
      const double stuck_s = std::chrono::duration<double>(now - sn.since).count();
      if (!sn.repaired && stuck_s >= kRepairSeconds) {
        sn.repaired = true;  // one attempt per stuck job
        const uint32_t counter_addr = l.records + i * 656;
        const uint32_t counter = Load32(base, counter_addr);
        REXLOG_WARN("[job-watch] slot {} job {:X} not released for {:.1f}s (state {:016X}, "
                    "enabler counter {}):",
                    i, (uint64_t(hi & 0x7FFF) << 32) | lo, stuck_s, word, counter);
        const bool deps_done = DiagnoseDeps(base, l, i, lo);
        // Counter accounting from this job's own history: exactly the guard
        // barrier + one per registered dependency.
        // Then a counter of 1 can only be a lost dependency event.
        uint32_t barriers = 0, guard_runs = 0;
        {
          SlotLog& sl = g_log[i];
          const uint32_t ne = sl.next.load(std::memory_order_relaxed);
          const uint32_t fe = ne > kRing ? ne - kRing : 0;
          bool after_alloc = false;
          for (uint32_t k = fe; k < ne; ++k) {
            const Entry& e = sl.e[k & (kRing - 1)];
            if (e.op == kBarrier && e.a == lo) ++barriers;
            if (e.op == kAlloc && e.a == lo) after_alloc = true;
            if (after_alloc && e.op == kRun) ++guard_runs;
          }
        }
        const uint32_t ndeps =
            g_pending[i].id_lo.load(std::memory_order_acquire) == lo
                ? std::min<uint32_t>(g_pending[i].n.load(std::memory_order_acquire), 8)
                : 0;
        // (The guard is released synchronously by the submitting thread right
        // after it registers the dependencies, not always through the hooked
        // Event::Run; after kRepairSeconds it has run, so guard_runs is only
        // reported.)
        const bool accounted = ndeps > 0 && barriers == 1 + ndeps;
        REXLOG_WARN("[job-watch]   barriers {} (expected {}), guard runs {}", barriers, 1 + ndeps,
                    guard_runs);
        if (counter == 1 && deps_done && accounted && REXCVAR_GET(sk83_job_release_repair)) {
          // What the lost event's Event::Run would have done: counter 1 -> 0,
          // then the enabler: the state word's high half with the state bits
          // cleared (0 = ready; the affinity bits stay). Re-check the word
          // first: it must still be this job, not-ready.
          const uint32_t hi2 = Load32(base, l.states + i * 8);
          const uint32_t lo2 = Load32(base, l.states + i * 8 + 4);
          if (hi2 == hi && lo2 == lo && Load32(base, counter_addr) == 1) {
            const uint32_t zero = 0;
            std::memcpy(base + counter_addr, &zero, 4);
            std::atomic_thread_fence(std::memory_order_seq_cst);
            const uint32_t ready = _byteswap_ulong(hi & 0x1FFFFFFFu);
            std::memcpy(base + l.states + i * 8, &ready, 4);
            std::atomic_thread_fence(std::memory_order_seq_cst);
            REXLOG_WARN("[job-watch] REPAIRED: slot {} job {:X} released (lost dependency "
                        "event; state hi {:08X} -> {:08X})",
                        i, (uint64_t(hi & 0x7FFF) << 32) | lo, hi, hi & 0x1FFFFFFFu);
            DumpSlot(i, "repaired job");
            sn = Seen{};
            continue;
          }
        } else {
          REXLOG_WARN("[job-watch]   not repaired ({})",
                      counter != 1      ? "counter is not 1"
                      : !deps_done      ? "a dependency is still pending"
                      : !accounted      ? "counter not fully accounted for"
                                        : "repair disabled");
        }
      }
      if (sn.dumped || stuck_s < kStuckSeconds) {
        continue;
      }
      sn.dumped = true;
      const uint32_t counter = Load32(base, l.records + i * 656);
      REXLOG_WARN(
          "[job-watch] STUCK: slot {} job {:X} not released for {:.0f}s "
          "(state word {:016X}, enabler counter {})",
          i, (uint64_t(hi & 0x7FFF) << 32) | lo, kStuckSeconds, word, counter);
      DumpSlot(i, "stuck job");
      // The dependencies THIS job registered on: only ADDEVENTs after its
      // own ALLOC (the ring also holds earlier occupants of the slot, whose
      // dependencies used to be dumped as if they were this job's), once each.
      SlotLog& s = g_log[i];
      const uint32_t n = s.next.load(std::memory_order_relaxed);
      const uint32_t first = n > kRing ? n - kRing : 0;
      uint32_t from = first;
      for (uint32_t k = first; k < n; ++k) {
        const Entry& e = s.e[k & (kRing - 1)];
        if (e.op == kAlloc && e.a == lo) from = k;
      }
      uint32_t dumped_slots[8];
      uint32_t nd = 0;
      for (uint32_t k = from; k < n; ++k) {
        const Entry& e = s.e[k & (kRing - 1)];
        const uint32_t ds = e.a >> 16;
        if (e.op != kAddEvent || ds >= kMaxSlots || ds == i) continue;
        bool dup = false;
        for (uint32_t q = 0; q < nd; ++q) dup = dup || dumped_slots[q] == ds;
        if (dup || nd >= 8) continue;
        dumped_slots[nd++] = ds;
        DumpSlot(ds, "dependency of the stuck job (slot reused since if it finished)");
      }
    }
  }
}

void Touch(uint8_t* base, uint32_t backend) {
  g_base.store(base, std::memory_order_relaxed);
  if (backend != 0) {
    g_backend.store(backend, std::memory_order_relaxed);
  }
  bool expected = false;
  if (g_watch_started.compare_exchange_strong(expected, true)) {
    std::thread(WatchThread).detach();
    REXLOG_INFO("[job-watch] armed (dumps any job stuck not-ready for {}s)",
                kStuckSeconds);
  }
}

}  // namespace

// Job::INTERNAL_AddNotReady(Job*, JobScheduler&): allocates the slot (handle
// lands at job+64 id / +72 backend / +76 slot) and adds the guard barrier.
extern "C" REX_FUNC(rex_EA_Jobs_Job_INTERNAL_AddNotReady_QAAXAAVJobScheduler_23_Z_82AB7EC0) {
  const uint32_t job = ctx.r3.u32;
  __imp__rex_EA_Jobs_Job_INTERNAL_AddNotReady_QAAXAAVJobScheduler_23_Z_82AB7EC0(ctx, base);
  const uint32_t backend = REX_LOAD_U32(job + 72);
  Touch(base, backend);
  const Layout l = GetLayout(base);
  const uint32_t slot = REX_LOAD_U16(job + 76);
  if (slot < l.count) {
    Log(slot, kAlloc, REX_LOAD_U32(job + 68), REX_LOAD_U32(l.records + slot * 656),
        REX_LOAD_U32(l.states + slot * 8));
    if (slot < kMaxSlots) {
      g_pending[slot].n.store(0, std::memory_order_relaxed);
      g_pending[slot].id_lo.store(REX_LOAD_U32(job + 68), std::memory_order_release);
    }
  }
}

// LocalBackend::AddBarrier(Event* out, id, handle): counter += 1.
extern "C" REX_FUNC(rex_EA_1Jobs_LocalBackend_AddBarrier_UAA_AVEvent_23_VJobInstanceHandle_23_Z_82AB84D8) {
  const uint32_t ev = ctx.r3.u32;
  const uint32_t id_lo = ctx.r5.u32;
  __imp__rex_EA_1Jobs_LocalBackend_AddBarrier_UAA_AVEvent_23_VJobInstanceHandle_23_Z_82AB84D8(ctx, base);
  const Layout l = GetLayout(base);
  const uint32_t counter = REX_LOAD_U32(ev + 12);
  const uint32_t slot = SlotOfRecord(l, counter);
  Log(slot, kBarrier, id_lo, counter != 0 ? REX_LOAD_U32(counter) : 0xFFFFFFFFu);
}

// Native LocalBackend::AddEvent (0x82ABA230) with the state-4 fix, step for step
// as the recompiled code: r3 = backend, r4 = dependency id (64-bit), r5 = its
// handle (slot = bits 16..31), r6 = Event*, r7 = When.
//   unlocked: id must match, record must exist, not finished -> else run now
//   loop: id match + not played, try the list lock (+644, 0 -> 1); on failure
//         spin, re-read, finished -> run now
//   locked: finished / id changed / played -> run the event, then unlock;
//           else append to the list (+80 + when*272), unlock.
// "Finished" = state 3 (done) or 4 (being garbage collected). The original
// only tested 3. Returns true when the state-4 test changed the outcome.
namespace {
uint32_t AtomicCas32(uint8_t* base, uint32_t addr, uint32_t expect, uint32_t desired) {
  auto* p = reinterpret_cast<volatile long*>(base + addr);
  const long e = long(_byteswap_ulong(expect));
  const long d = long(_byteswap_ulong(desired));
  return _byteswap_ulong(uint32_t(_InterlockedCompareExchange(p, d, e)));
}
void AddEventFixed(PPCContext& ctx, uint8_t* base) {
  const uint32_t be = ctx.r3.u32;
  const uint64_t dep_id = ctx.r4.u64;
  const uint32_t slot = uint32_t(ctx.r5.u64 >> 16) & 0xFFFFu;
  const uint32_t ev = ctx.r6.u32;
  const uint32_t when = ctx.r7.u32;
  const uint32_t sw = Load32(base, be + 8) + slot * 8;
  const auto id_of = [&](uint32_t hi) {
    return (uint64_t(hi & 0x7FFFu) << 32) | Load32(base, sw + 4);
  };
  const auto state_of = [](uint32_t hi) { return hi >> 29; };
  bool saved = false;  // state 4 seen where the original tested only 3
  const auto finished = [&](uint32_t hi) {
    const uint32_t st = state_of(hi);
    if (st == 4) saved = true;
    return st == 3 || st == 4;
  };
  const auto run_now = [&]() {
    ctx.r3.u64 = ev;
    rex_EA_Jobs_Event_Run_QBAXXZ_82AB7128(ctx, base);
  };
  const uint32_t rec_base = Load32(base, be + 4);
  const uint32_t rec = rec_base + slot * 656;
  uint32_t hi = Load32(base, sw);
  if (id_of(hi) != dep_id || rec == 0 || finished(hi)) {
    run_now();
  } else {
    for (;;) {
      if (id_of(hi) != dep_id || base[rec + 648 + when] != 0) {
        run_now();
        break;
      }
      if (AtomicCas32(base, rec + 644, 0, 1) == 0) {
        // Locked: re-check under the lock.
        hi = Load32(base, sw);
        if (finished(hi) || id_of(hi) != dep_id || base[rec + 648 + when] != 0) {
          run_now();
        } else {
          ctx.r3.u64 = rec + 80 + when * 272;
          ctx.r4.u64 = ev;
          rex_EA_Jobs_VEvent_BucketListNode_Add_0BA_Detail_Jobs_EA_QAAXABVEvent_34_Z_82AB8A50(ctx, base);
        }
        AtomicCas32(base, rec + 644, 1, 0);  // AutoTryLockEventList's release
        break;
      }
      _mm_pause();  // db16cyc
      hi = Load32(base, sw);
      if (finished(hi)) {
        run_now();
        break;
      }
    }
  }
  if (saved) g_addevent_saves.fetch_add(1, std::memory_order_relaxed);
}
}  // namespace

// LocalBackend::AddEvent(dep id, dep handle, Event&, When): attach the event
// to the dependency, or run it now when the dependency is done/played.
extern "C" REX_FUNC(rex_EA_1Jobs_LocalBackend_AddEvent_UAAXVJobInstanceHandle_23_ABVEvent_23_W4When_523_Z_82ABA230) {
  const uint32_t dep_id = ctx.r4.u32;
  const uint32_t dep_slot = uint32_t(ctx.r5.u64 >> 16) & 0xFFFF;
  const uint32_t ev = ctx.r6.u32;
  const uint32_t when = ctx.r7.u32;
  const Layout l = GetLayout(base);
  const uint32_t target = REX_LOAD_U32(ev + 8);
  const uint32_t counter = REX_LOAD_U32(ev + 12);
  const uint32_t slot = SlotOfTarget(l, target);
  const uint32_t before = counter != 0 ? REX_LOAD_U32(counter) : 0xFFFFFFFFu;
  const uint64_t add_t0 = __rdtsc();
  if (REXCVAR_GET(sk83_job_addevent_fix)) {
    AddEventFixed(ctx, base);
  } else {
    __imp__rex_EA_1Jobs_LocalBackend_AddEvent_UAAXVJobInstanceHandle_23_ABVEvent_23_W4When_523_Z_82ABA230(ctx, base);
  }
  const uint32_t after = counter != 0 ? REX_LOAD_U32(counter) : 0xFFFFFFFFu;
  Log(slot, kAddEvent, (dep_slot << 16) | (when & 0xFFFF), dep_id, before, after);
  if (slot < kMaxSlots && dep_slot < kMaxSlots && when == 1 && before == after) {
    // Test injection (sk83_job_watch_inject): one extra count = one "lost" release.
    static std::atomic<bool> s_injected{false};
    static const auto s_t0 = std::chrono::steady_clock::now();
    const int inject_s = REXCVAR_GET(sk83_job_watch_inject);
    if (inject_s > 0 && counter != 0 && !s_injected.load(std::memory_order_relaxed) &&
        std::chrono::steady_clock::now() - s_t0 > std::chrono::seconds(inject_s) &&
        !s_injected.exchange(true)) {
      REX_STORE_U32(counter, REX_LOAD_U32(counter) + 1);
      REXLOG_WARN("[job-watch] TEST INJECT: slot {} job lo {:08X} will miss one release (dep slot {})",
                  slot, REX_LOAD_U32(l.states + slot * 8 + 4), dep_slot);
    }
    PendingDeps& pd = g_pending[slot];
    const uint32_t k = pd.n.load(std::memory_order_relaxed);
    if (k < 8) {
      pd.d[k] = {dep_slot, dep_id, add_t0};
      pd.n.store(k + 1, std::memory_order_release);
    }
  }
}

// Event::Run(Event*): the guard's decrement (and AddEvent's run-now path,
// which is logged inside ADDEVENT as the counter change as well).
extern "C" REX_FUNC(rex_EA_Jobs_Event_Run_QBAXXZ_82AB7128) {
  const uint32_t ev = ctx.r3.u32;
  const uint32_t type = REX_LOAD_U32(ev + 0);
  const uint32_t target = REX_LOAD_U32(ev + 8);
  const uint32_t counter = REX_LOAD_U32(ev + 12);
  const Layout l = GetLayout(base);
  const uint32_t slot = (type == 1 || type == 3) ? SlotOfTarget(l, target) : kMaxSlots;
  const uint32_t before = counter != 0 ? REX_LOAD_U32(counter) : 0xFFFFFFFFu;
  __imp__rex_EA_Jobs_Event_Run_QBAXXZ_82AB7128(ctx, base);
  const uint32_t after = counter != 0 ? REX_LOAD_U32(counter) : 0xFFFFFFFFu;
  Log(slot, kRun, type, REX_LOAD_U32(ev + 4), before, after);
}

// JobInstance::PlayEventList(When): a job's start (0) / end (1) events. It
// runs them inline (no Event::Run), so the events it is about to play are
// read from the list first and logged on the slots they release.
extern "C" REX_FUNC(rex_EA_Jobs_LocalBackend_JobInstance_PlayEventList_AAAXW4When_Event_34_Z_82AB9A28) {
  const uint32_t rec = ctx.r3.u32;
  const uint32_t when = ctx.r4.u32;
  const Layout l = GetLayout(base);
  const uint32_t player = SlotOfRecord(l, rec);
  const uint32_t player_id =
      player < l.count ? REX_LOAD_U32(l.states + player * 8 + 4) : 0;
  constexpr uint32_t kMaxPlayed = 64;
  uint32_t targets[kMaxPlayed];
  uint32_t counters[kMaxPlayed];
  uint32_t n = 0;
  // Event list for `when`: record + 80 + when*272; 16 events of 16 bytes,
  // then +256 next node, +260 count.
  uint32_t node = rec + 80 + when * 272;
  for (int hops = 0; node != 0 && hops < 8 && n < kMaxPlayed; ++hops) {
    const uint32_t count = REX_LOAD_U32(node + 260);
    for (uint32_t k = 0; k < count && k < 16 && n < kMaxPlayed; ++k) {
      const uint32_t e = node + k * 16;
      const uint32_t type = REX_LOAD_U32(e + 0);
      if (type != 1 && type != 3) {
        continue;
      }
      const uint32_t target = REX_LOAD_U32(e + 8);
      const uint32_t counter = REX_LOAD_U32(e + 12);
      const uint32_t slot = SlotOfTarget(l, target);
      if (slot >= kMaxSlots) {
        continue;
      }
      targets[n] = slot;
      counters[n] = counter;
      Log(slot, kPlayed, (player << 16) | (when & 0xFFFF), player_id,
          counter != 0 ? REX_LOAD_U32(counter) : 0xFFFFFFFFu);
      ++n;
    }
    node = REX_LOAD_U32(node + 256);
  }
  const uint64_t play_t0 = __rdtsc();
  __imp__rex_EA_Jobs_LocalBackend_JobInstance_PlayEventList_AAAXW4When_Event_34_Z_82AB9A28(ctx, base);
  if (when == 1 && player < kMaxSlots) {
    const uint64_t i = g_play_next.fetch_add(1, std::memory_order_relaxed);
    g_plays[i & (kPlayRing - 1)] = {play_t0, __rdtsc(), player, player_id, n};
  }
  for (uint32_t k = 0; k < n; ++k) {
    Log(targets[k], kPlayAfter, (player << 16) | (when & 0xFFFF), player_id, 0,
        counters[k] != 0 ? REX_LOAD_U32(counters[k]) : 0xFFFFFFFFu);
  }
}

// JobInstanceHandle::Enable(): direct release (no counter).
extern "C" REX_FUNC(rex_EA_Jobs_JobInstanceHandle_Enable_QAAXXZ_82AB7348) {
  const uint32_t h = ctx.r3.u32;
  const uint32_t slot = REX_LOAD_U16(h + 12);
  const uint32_t id_lo = REX_LOAD_U32(h + 4);
  __imp__rex_EA_Jobs_JobInstanceHandle_Enable_QAAXXZ_82AB7348(ctx, base);
  Log(slot, kEnable, id_lo);
}
