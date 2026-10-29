// TEMPORARY: what guest work actually runs, ranked by CPU cost.
//
// RunSingleJob is 27.2% of total process CPU -- it is the parent of nearly all
// guest job work. The question worth answering is not how to make the
// dispatcher faster (7.24% self, the queue scan) but how much of the 20% it
// dispatches is work our native renderer throws away.
//
// Run_JobInstance dispatches through a function pointer at job+44:
//     lwz r11,44(r31) ; mtctr r11 ; bctrl
// with arguments at job+56/60/64/68. That pointer is a stable identity for
// "which kind of job", so this hook histograms cost by it.
//
// Reports BOTH exclusive (self) and inclusive time. The distinction matters:
// a job that waits for its children by helping to run them absorbs their time
// and looks like the hot spot when it is only the parent. The first run of
// this probe showed a top entry averaging 4M cycles (~1.3 ms) per call, which
// is far too long for a leaf, so inclusive time alone was not trustworthy.
// Self time is computed by having each nested call report its full span to its
// parent through t_child_cycles and subtracting.
//
// Remaining caveat: nesting is only observed where it passes back through
// Run_JobInstance. A job that blocks in a spin/wait without dispatching still
// bills that wait to itself, so a high self time can mean "waiting", not
// "working". Call counts are exact.

#include "generated/sk83_na_m_init.h"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <vector>

#include <intrin.h>

#include <rex/logging.h>

namespace {

constexpr uint32_t kSlots = 2048;  // power of two, open addressing

struct Slot {
  std::atomic<uint32_t> key{0};
  std::atomic<uint64_t> calls{0};
  std::atomic<uint64_t> cycles{0};  // exclusive
  std::atomic<uint64_t> incl{0};
};

Slot g_slots[kSlots];
std::atomic<uint64_t> g_total_calls{0};
std::atomic<uint64_t> g_total_cycles{0};   // exclusive (self)
std::atomic<uint64_t> g_incl_cycles{0};    // inclusive, for comparison

// Cycles billed by nested Run_JobInstance calls on this thread. A job that
// waits for its children by helping to run them would otherwise absorb their
// time and look like the hot spot when it is only the parent.
thread_local uint64_t t_child_cycles = 0;

inline uint32_t SlotFor(uint32_t addr) {
  uint32_t h = (addr * 2654435761u) >> 16;
  for (uint32_t probe = 0; probe < 64; ++probe) {
    Slot& s = g_slots[(h + probe) & (kSlots - 1)];
    uint32_t k = s.key.load(std::memory_order_acquire);
    if (k == addr) {
      return (h + probe) & (kSlots - 1);
    }
    if (k == 0) {
      uint32_t expected = 0;
      if (s.key.compare_exchange_strong(expected, addr,
                                        std::memory_order_acq_rel)) {
        return (h + probe) & (kSlots - 1);
      }
      if (expected == addr) {
        return (h + probe) & (kSlots - 1);
      }
    }
  }
  return kSlots - 1;  // table full: fold into the last slot
}

void Report() {
  struct Row {
    uint32_t addr;
    uint64_t calls;
    uint64_t cycles;
    uint64_t incl;
  };
  std::vector<Row> rows;
  rows.reserve(128);
  for (uint32_t i = 0; i < kSlots; ++i) {
    const uint32_t k = g_slots[i].key.load(std::memory_order_acquire);
    if (k == 0) {
      continue;
    }
    rows.push_back({k, g_slots[i].calls.load(std::memory_order_relaxed),
                    g_slots[i].cycles.load(std::memory_order_relaxed),
                    g_slots[i].incl.load(std::memory_order_relaxed)});
  }
  std::sort(rows.begin(), rows.end(),
            [](const Row& a, const Row& b) { return a.cycles > b.cycles; });

  const uint64_t total = std::max<uint64_t>(1, g_total_cycles.load());
  const uint64_t incl_total = std::max<uint64_t>(1, g_incl_cycles.load());
  REXLOG_INFO("[jobs] {} entry points, {} calls, self {} Mcyc, incl {} Mcyc "
              "(nesting factor {:.2f})",
              rows.size(), g_total_calls.load(), total / 1000000,
              incl_total / 1000000, double(incl_total) / double(total));
  const size_t shown = std::min<size_t>(rows.size(), 24);
  for (size_t i = 0; i < shown; ++i) {
    REXLOG_INFO("[jobs]   {:2}. fn={:08X} self {:6.2f}%  incl {:6.2f}%  "
                "calls={:<9} self_avg={} cyc",
                i + 1, rows[i].addr,
                100.0 * double(rows[i].cycles) / double(total),
                100.0 * double(rows[i].incl) / double(incl_total), rows[i].calls,
                rows[i].cycles / std::max<uint64_t>(1, rows[i].calls));
  }
}

}  // namespace

extern "C" REX_FUNC(rex_EA_Jobs_LocalBackend_JobInstance_Run_QAAXXZ_82ABA3B8) {
  const uint32_t job = ctx.r3.u32;
  uint32_t fn = job != 0 ? REX_LOAD_U32(job + 44) : 0;
  // 0 is the empty-slot marker, so a null dispatch pointer would accumulate
  // into a slot Report() skips. Map it to a sentinel instead of losing it.
  if (fn == 0) {
    fn = 0xFFFFFFFFu;
  }

  const uint64_t saved_child = t_child_cycles;
  t_child_cycles = 0;

  const uint64_t t0 = __rdtsc();
  __imp__rex_EA_Jobs_LocalBackend_JobInstance_Run_QAAXXZ_82ABA3B8(ctx, base);
  const uint64_t dt = __rdtsc() - t0;

  // Whatever nested calls billed while we were inside is not our own work.
  const uint64_t nested = t_child_cycles;
  const uint64_t self = dt > nested ? dt - nested : 0;
  t_child_cycles = saved_child + dt;  // report our full span to our parent

  const uint32_t slot = SlotFor(fn);
  g_slots[slot].calls.fetch_add(1, std::memory_order_relaxed);
  g_slots[slot].cycles.fetch_add(self, std::memory_order_relaxed);
  g_slots[slot].incl.fetch_add(dt, std::memory_order_relaxed);
  g_total_cycles.fetch_add(self, std::memory_order_relaxed);
  g_incl_cycles.fetch_add(dt, std::memory_order_relaxed);

  const uint64_t n = g_total_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if ((n & 0xFFFFu) == 0) {  // ~65k dispatches; 1M was too rare to ever fire
    Report();
  }
}
