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

#include <rex/logging.h>

namespace {

constexpr uint32_t kMaxSlots = 1024;
constexpr uint32_t kRing = 64;  // per slot, power of two
constexpr double kStuckSeconds = 3.0;

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

// Every 500 ms: any slot in state 2 with the same id for kStuckSeconds gets
// its history dumped once, together with the slots of the dependencies that
// were attached to it (from its ADDEVENT entries).
void WatchThread() {
  struct Seen {
    uint64_t word = 0;
    std::chrono::steady_clock::time_point since;
    bool dumped = false;
  };
  static Seen seen[kMaxSlots];
  for (;;) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
      if (sn.dumped ||
          std::chrono::duration<double>(now - sn.since).count() < kStuckSeconds) {
        continue;
      }
      sn.dumped = true;
      const uint32_t counter = Load32(base, l.records + i * 656);
      REXLOG_WARN(
          "[job-watch] STUCK: slot {} job {:X} not released for {:.0f}s "
          "(state word {:016X}, enabler counter {})",
          i, (uint64_t(hi & 0x7FFF) << 32) | lo, kStuckSeconds, word, counter);
      DumpSlot(i, "stuck job");
      // The dependencies attached to it.
      SlotLog& s = g_log[i];
      const uint32_t n = s.next.load(std::memory_order_relaxed);
      const uint32_t first = n > kRing ? n - kRing : 0;
      for (uint32_t k = first; k < n; ++k) {
        const Entry& e = s.e[k & (kRing - 1)];
        if (e.op == kAddEvent && (e.a >> 16) < kMaxSlots && (e.a >> 16) != i) {
          DumpSlot(e.a >> 16, "dependency of the stuck job");
        }
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
  __imp__rex_EA_1Jobs_LocalBackend_AddEvent_UAAXVJobInstanceHandle_23_ABVEvent_23_W4When_523_Z_82ABA230(ctx, base);
  const uint32_t after = counter != 0 ? REX_LOAD_U32(counter) : 0xFFFFFFFFu;
  Log(slot, kAddEvent, (dep_slot << 16) | (when & 0xFFFF), dep_id, before, after);
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
  __imp__rex_EA_Jobs_LocalBackend_JobInstance_PlayEventList_AAAXW4When_Event_34_Z_82AB9A28(ctx, base);
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
