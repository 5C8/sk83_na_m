// zd-only options at boot.
//
// zd's app init (zd 0x837BC320) creates ThePrintChannelMan -- whose channels
// register the kbDebugPrint_<channel> options -- and its static initializers
// register every zd-only option before it reads startup.ini (the same call,
// insn 730). m reads startup.ini in Sk8AppMan::InitBootDebug right after it
// constructs the cvar manager, so the zd-only registration runs as soon as
// the manager exists: startup.ini values (e.g. kbDebugPrint_Default=1) then
// reach those options, and the guest console's cvar list has them from the
// start.

#include "zd_debug.h"
#include "zdx.h"

#include "generated/sk83_na_m_init.h"

// cVarMan::cVarMan (m 0x82D8A820; sets TheCVarMan, links the queued refs),
// called from Sk8AppMan::InitBootDebug just before the startup.ini read.
extern "C" REX_FUNC(sub_82D8A820) {
  __imp__sub_82D8A820(ctx, base);
  // Prepare services before startup.ini is read; zd_debug gates their output.
  zdx::RegisterZdOnly(ctx, base);
}
