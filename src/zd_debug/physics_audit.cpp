// Read-only diagnostics for the reported zd_debug jump-height difference.
#include "generated/sk83_na_m_init.h"
#include "zd_debug.h"

#include <array>
#include <atomic>
#include <bit>
#include <rex/cvar.h>
#include <rex/logging.h>

REXCVAR_DEFINE_BOOL(zdx_physics_audit, false, "Skate 3 zd debug",
                   "Log jump inputs/results without changing physics (diagnostics)");

namespace {
struct RestoreMath {
  uint32_t csr = PPCFPSCRRegister::Platform::getcsr();
  ~RestoreMath() { PPCFPSCRRegister::Platform::setcsr(csr); }
};
float ReadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(REX_LOAD_U32(address));
}
std::array<float, 4> ReadVector(uint8_t* base, uint32_t address) {
  return {ReadFloat(base,address),ReadFloat(base,address+4),
          ReadFloat(base,address+8),ReadFloat(base,address+12)};
}
}

extern "C" REX_FUNC(rex_Sk8_Physics_Skateboard_Toolkit_CalcGroundJump_QAA_AUSkateboardJumpInfo_23_XZ_82EB20D8) {
  const uint32_t out = ctx.r3.u32, toolkit = ctx.r4.u32;
  __imp__rex_Sk8_Physics_Skateboard_Toolkit_CalcGroundJump_QAA_AUSkateboardJumpInfo_23_XZ_82EB20D8(ctx,base);
  if (!REXCVAR_GET(zdx_physics_audit) || !out) return;
  RestoreMath restore;
  static std::atomic<unsigned> count{0};
  const unsigned n = count.fetch_add(1);
  if (n >= 4096) return;
  const auto value = ReadVector(base,out);
  REXLOG_INFO("zd-physics: ground-jump #{} debug={} toolkit={:08X} out=({:.6f},{:.6f},{:.6f},{:.6f}) f16={:.6f} flag20={} dt={:.9f}",
              n,zd_debug::Enabled(),toolkit,value[0],value[1],value[2],value[3],
              ReadFloat(base,out+16),unsigned(REX_LOAD_U8(out+20)),ReadFloat(base,0x82222D04u));
}

extern "C" REX_FUNC(rex_Sk8_Physics_Skateboard_Toolkit_ClampJumpVelocity_QAA_AVVector3_vpu_math_rw_ABV4567_Z_82EA65A8) {
  const uint32_t out = ctx.r3.u32, toolkit = ctx.r4.u32, input = ctx.r5.u32;
  const bool audit = REXCVAR_GET(zdx_physics_audit) && out && input;
  std::array<float,4> before{};
  if (audit) before = ReadVector(base,input);
  __imp__rex_Sk8_Physics_Skateboard_Toolkit_ClampJumpVelocity_QAA_AVVector3_vpu_math_rw_ABV4567_Z_82EA65A8(ctx,base);
  if (!audit) return;
  RestoreMath restore;
  static std::atomic<unsigned> count{0};
  const unsigned n = count.fetch_add(1);
  if (n >= 4096) return;
  const auto after = ReadVector(base,out);
  REXLOG_INFO("zd-physics: clamp-jump #{} debug={} toolkit={:08X} in=({:.6f},{:.6f},{:.6f}) out=({:.6f},{:.6f},{:.6f})",
              n,zd_debug::Enabled(),toolkit,before[0],before[1],before[2],after[0],after[1],after[2]);
}

extern "C" REX_FUNC(rex_Sk8_Physics_PhysState_PhysicsAir_CalcVelFromJump_AAA_AVVector3_vpu_math_rw_H_Z_8300DB60) {
  const uint32_t out=ctx.r3.u32,self=ctx.r4.u32,index=ctx.r5.u32;
  __imp__rex_Sk8_Physics_PhysState_PhysicsAir_CalcVelFromJump_AAA_AVVector3_vpu_math_rw_H_Z_8300DB60(ctx,base);
  if (!REXCVAR_GET(zdx_physics_audit) || !out) return;
  RestoreMath restore;
  static std::atomic<unsigned> count{0};
  const unsigned n=count.fetch_add(1);
  if(n>=4096)return;
  const auto value=ReadVector(base,out);
  REXLOG_INFO("zd-physics: air-jump #{} debug={} self={:08X} index={} velocity=({:.6f},{:.6f},{:.6f}) dt={:.9f}",
              n,zd_debug::Enabled(),self,index,value[0],value[1],value[2],ReadFloat(base,0x82222D04u));
}
