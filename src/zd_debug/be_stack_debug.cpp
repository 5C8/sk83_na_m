// BE state stack on screen (kbEnableBEStateName): zd's sender, transplanted.
//
// Presentation::DisplayBEStateNames (in RenderMan::DebugRender, render_debug.cpp)
// draws Presentation's debug BE-state list. Both builds keep that list and its
// message receivers (cMsgAddDebugBEStateStack / cMsgClearDebugStateStack), but
// only zd sends them: StateManagerFrontEnd's vtable slot 5 (zd 0x836BCDF8),
// after updating the stack, in the front end (mode 6) clears the list and
// re-adds every stack entry. m's slot 5 (0x827530E8) only runs the base update.
// Here m's own function runs (m's return value is kept) and then zd's send
// block, as a fragment (zdx_frag_BEStackDebugSend) with zd's live-in this.

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include "generated/sk83_na_m_init.h"

// Sk8::FE::StateManagerFrontEnd vtable slot 5 (m 0x827530E8).
extern "C" REX_FUNC(sub_827530E8) {
  const uint32_t self = ctx.r3.u32;
  __imp__sub_827530E8(ctx, base);
  // zd sends only when the base update reports a change (r3 & 0xFF). The
  // fragment needs only the pool (GuestCall builds it), not the zd-only
  // cvars, so it also runs for the stack changes during loading.
  if (!zd_debug::Enabled() || !(ctx.r3.u32 & 0xFF)) return;
  zdx::GuestCall call(ctx, base, 0x200);
  call.c.r29.u64 = self;
  call(zdx_frag_BEStackDebugSend);
}
