// Diagnostic members are prepared at construction even when their output
// is disabled, so the master switch can enable the complete overlay later.
#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"
#include "generated/sk83_na_m_init.h"

extern "C" REX_FUNC(rex_f0_PresentationObj_VSimPresentation_Sk8_Sk8_QAA_AAVIDataSysClient_1_W4PresentationSource_1_PBVSimulation_1_Z_8292AA50) {
  const uint32_t self=ctx.r3.u32;
  __imp__rex_f0_PresentationObj_VSimPresentation_Sk8_Sk8_QAA_AAVIDataSysClient_1_W4PresentationSource_1_PBVSimulation_1_Z_8292AA50(ctx,base);
  if (!self || !zdx::PoolReady(base)) return;
  zdx::GuestCall call(ctx,base,0x100);
  call.c.r3.u64=zdx::Ext(self+20,6); // SimPresentation's extra DebugStringRef
  call.c.r4.u64=zdx::Map(0x823387E4u); // "Time: "
  call(zdx_839ACA10);
}
