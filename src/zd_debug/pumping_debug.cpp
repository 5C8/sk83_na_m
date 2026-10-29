// Pumping debug graphs (kDrawNewPump, kScaleGraph1/2).
//
// zd's Physics::Pumping ctor also builds four DebugGraph widgets
// (+80/+116/+152/+188) that Pumping::Update feeds and Pumping::DebugDraw
// draws. m's Pumping has no graphs (96 bytes vs zd's 240). After m's ctor the
// graph part of zd's ctor runs on m's object (zdx FRAGMENTS "PumpingGraphs";
// the graphs live in the Pumping extension block, config EXT 14).

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include "generated/sk83_na_m_init.h"

// Sk8::Physics::Pumping::Pumping() (m 0x8305F540).
extern "C" REX_FUNC(sub_8305F540) {
  const uint32_t self = ctx.r3.u32;
  __imp__sub_8305F540(ctx, base);
  if (!zdx::PoolReady(base)) return;  // prepare the graphs for later enable
  zdx::GuestCall call(ctx, base, 0x100);
  call.c.r3.u64 = self;
  call(zdx_frag_PumpingGraphs);
}
