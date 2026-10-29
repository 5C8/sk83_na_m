// BodySpin debug graphs (kDebugDisplayBodySpinGraphs).
//
// zd's BodySpin is a separate object whose ctor also builds four DebugGraph
// widgets (+176/+212/+248/+284). In m BodySpin is the start of Reckoning
// (same field offsets, no graphs). After m's Reckoning ctor the graph part of
// zd's BodySpin ctor runs on the Reckoning (zdx FRAGMENTS "BodySpinGraphs";
// the graphs live in the BodySpin extension block, config EXT 12).

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include "generated/sk83_na_m_init.h"

// Sk8::Physics::Reckoning::Reckoning(ProcessedPhysIn const*) (m 0x8306EEC0).
extern "C" REX_FUNC(sub_8306EEC0) {
  const uint32_t self = ctx.r3.u32;
  __imp__sub_8306EEC0(ctx, base);
  if (!zdx::PoolReady(base)) return;  // prepare the graphs for later enable
  zdx::GuestCall call(ctx, base, 0x100);
  call.c.r3.u64 = self;
  call(zdx_frag_BodySpinGraphs);
}
