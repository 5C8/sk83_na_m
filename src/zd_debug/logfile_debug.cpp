// zd's log files: PrintChannelMan::Internal opens d:\logs\<MM-DD-YYYY>\output_*.log
// and warnings_*.log (zd PrintLogFile; game\logs on the host, written when the
// runtime allows game-relative writes: --allow_game_relative_writes=true).
//
// In zd every xcore print reached PrintChannelMan::Internal::Print, which also
// appended it to the output log. In m, xcore prints go straight to
// ConsolePrintFunction -> OutputDebugStringA (zd's print-manager pointer is a
// private copy), so the m print hook (skate3_online.cpp) forwards each line
// here, into zd's output log (PrintLogFile::Print + Flush, as Internal::Print
// does; Internal::Print itself would print to OutputDebugStringA again), and to
// the Juice TTY (JuiceBase::PrintTTY, as Internal::Print does).

#include <mutex>

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

namespace {
constexpr uint32_t kPrintChannelMan = 0x84904474u;  // zd ThePrintChannelMan instance
std::mutex g_log_mu;
thread_local bool t_in_log = false;
}  // namespace

namespace zdx {

void OnGamePrint(PPCContext& ctx, uint8_t* base, uint32_t text) {
  if (!zd_debug::Enabled() || !PoolReady(base) || !text || t_in_log) return;
  // Internal::Print first sends the line to the Juice TTY plugin
  // (JuiceBase::PrintTTY: only while Juice is connected); m dropped the TTY
  // sender (TTYPlugin::LogText), so its TTY plugin registered but stayed silent.
  {
    t_in_log = true;
    GuestCall call(ctx, base);
    call.c.r3.u64 = text;
    call(zdx_837A24B0);  // Juice::JuiceBase::PrintTTY(const char*)
    t_in_log = false;
  }
  const uint32_t pcm = REX_LOAD_U32(Map(kPrintChannelMan));
  if (!pcm) return;
  const uint32_t internal = REX_LOAD_U32(pcm + 4);
  if (!internal) return;
  const uint32_t output = REX_LOAD_U32(internal + 4);  // PrintLogFile* (null if the open failed)
  if (!output) return;
  std::lock_guard<std::mutex> lock(g_log_mu);
  t_in_log = true;
  {
    GuestCall call(ctx, base);
    call.c.r3.u64 = output;
    call.c.r4.u64 = text;
    call(zdx_837AA1B0);  // PrintLogFile::Print(const char*)
  }
  {
    GuestCall call(ctx, base);
    call.c.r3.u64 = output;
    call(zdx_837A77D0);  // PrintLogFile::Flush()
  }
  t_in_log = false;
}

}  // namespace zdx
