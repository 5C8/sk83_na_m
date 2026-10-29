// Crash diagnostics: on abort (REX_FATAL, e.g. "Call to invalid or
// unregistered function at guest address 0x00000000") log the host call
// stack as module+offset, so the recompiled function that made the call can
// be found with llvm-symbolizer on the exe. Logs only; abort continues.

#include "zd_debug.h"

#include <rex/logging.h>
#include <rex/logging/assert.h>

#include <csignal>
#include <cstdio>
#include <windows.h>

namespace {

void OnAbort(int) {
  void* frames[48];
  const USHORT n = CaptureStackBackTrace(1, 48, frames, nullptr);
  for (USHORT i = 0; i < n; ++i) {
    HMODULE mod = nullptr;
    char name[MAX_PATH] = "?";
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           static_cast<LPCSTR>(frames[i]), &mod)) {
      GetModuleFileNameA(mod, name, MAX_PATH);
    }
    const char* base = strrchr(name, '\\');
    REXLOG_CRITICAL("abort frame {:2}: {}+0x{:X}", i, base ? base + 1 : name,
                    uint64_t(frames[i]) - uint64_t(mod));
  }
  if (auto l = ::rex::GetLogger()) l->flush();
}

struct Install {
  Install() { std::signal(SIGABRT, OnAbort); }
} g_install;

}  // namespace
