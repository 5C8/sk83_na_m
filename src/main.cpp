#include <string>
#include <cstring>
#include <atomic>
#include <mutex>
#include <vector>

// sk83_na_m - ReXGlue Recompiled Project
//
// This file is yours to edit. 'rexglue migrate' will NOT overwrite it.

#include "generated/sk83_na_m_init.h"

#include "sk83_na_m_app.h"

#include <cstdio>

// Windows APIs used by the TLS/Rtl* thunks below.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/system/xthread.h>

REX_DEFINE_APP(sk83_na_m, Sk83NaMApp::Create)

// --- Stubs for functions we don't care about during boot ---
//
// IMPORTANT: names must be the actual `__imp__` symbol the generated
// code calls (see sk83_na_m_init.h: `DECLARE_REX_FUNC(__imp__XXX);`).
// A `_XXX` name relies on MSVC's automatic dllimport-thunk fixup and
// is flaky. When a stub does need to return a value in r3, an empty
// REX_STUB is not enough -- see the TLS impls below.
REX_STUB(__imp__XSetLaunchData);
REX_STUB(__imp__XGetLaunchData);
REX_STUB(__imp__XGetLaunchDataSize);
REX_STUB(__imp__XamLaunchNewImage);
REX_STUB(__imp__XamTerminateTitle);
REX_STUB(__imp__VdVerifyMEInitCommand);

// --- __imp__CreateThread_0 -- really KeInitializeDpc ---
//
// The recompiler named the import thunk at guest 0x832A89D8
// "CreateThread_0", but it is the xboxkrnl KeInitializeDpc import. Its only
// caller, the D3D device's GPU-interrupt setup (bl at 0x82DA5370), passes
// r3 = &KDPC, r4 = DPC routine (0x82DA4150), r5 = context -- then creates
// the real worker thread through rex_CreateThread_82DB16B8 right after.
//
// Forwarding it to CreateThread made that call a thread create with
// stack size = 0x82DA4150 (larger than the whole guest stack range) and
// start address = a heap pointer: "BaseHeap::Alloc page count too big" ->
// "Thread creation failed: C0000017" at every boot, and the DPC was never
// initialised. Initialise it exactly as the kernel export does.
extern "C" void __imp__CreateThread_0(PPCContext& ctx, uint8_t* base) {
    auto* dpc = reinterpret_cast<rex::system::XDPC*>(REX_RAW_ADDR(ctx.r3.u32));
    dpc->Initialize(ctx.r4.u32, ctx.r5.u32);
}

// --- TLS implementations ---
//
// Empty stubs for these break the MSVC CRT: _mtinit calls TlsAlloc,
// stores the returned slot into a global, then later reads it back via
// TlsGetValue. If the stub doesn't set ctx.r3, whatever garbage was in
// r3 becomes the "TLS slot" and every downstream call goes wrong.
//
// Xbox 360's TlsAlloc/Set/Get/Free have the same semantics as Windows',
// so we just wire straight through. TLS values in the guest are 32-bit
// guest addresses; Windows TLS stores void*, which is wider on x64,
// but we consistently write/read the low 32 bits so the round-trip is
// lossless.

extern "C" void __imp__TlsAlloc(PPCContext& ctx, uint8_t* base) {
    (void)base;
    ctx.r3.u64 = static_cast<uint32_t>(::TlsAlloc());
}

extern "C" void __imp__TlsSetValue(PPCContext& ctx, uint8_t* base) {
    (void)base;
    // r3 = slot index, r4 = value to store
    BOOL ok = ::TlsSetValue(
        static_cast<DWORD>(ctx.r3.u32),
        reinterpret_cast<LPVOID>(static_cast<uintptr_t>(ctx.r4.u32)));
    ctx.r3.u64 = ok ? 1u : 0u;
}

extern "C" void __imp__TlsGetValue(PPCContext& ctx, uint8_t* base) {
    (void)base;
    // r3 = slot index; return in r3
    LPVOID v = ::TlsGetValue(static_cast<DWORD>(ctx.r3.u32));
    ctx.r3.u64 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(v));
}

extern "C" void __imp__TlsFree(PPCContext& ctx, uint8_t* base) {
    (void)base;
    // r3 = slot index
    BOOL ok = ::TlsFree(static_cast<DWORD>(ctx.r3.u32));
    ctx.r3.u64 = ok ? 1u : 0u;
}

// --- Rtl string formatting implementations ---
//
// RtlSprintf / RtlSnprintf / RtlVsnprintf are the NT-flavoured names
// for sprintf / _snprintf / _vsnprintf. The Xbox CRT calls them during
// early init. Route through the host CRT so anything they format ends
// up in guest memory correctly.
//
// Argument marshalling: guest ABI puts the first varargs in r5,r6,r7,r8
// (RtlSprintf: r3=dst, r4=fmt, r5..=args). We can't reconstruct a full
// va_list here without more work, so we just pull four ints and treat
// the format string as taking at most four %-args. That's enough for
// the CRT's own usage during boot; if the game itself calls these with
// more args, we'll extend it then.

namespace {
constexpr size_t kRtlFormatMaxLen = 1024;

int rex_format_from_guest(char* dst, size_t cap, const char* fmt,
                          uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {
    // vsnprintf-style call, but with our fixed 4-int shim.
    return std::snprintf(dst, cap, fmt, a0, a1, a2, a3);
}
}  // namespace

extern "C" void __imp__RtlSprintf(PPCContext& ctx, uint8_t* base) {
    // r3 = dst (guest), r4 = fmt (guest), r5..r8 = args
    char* dst = reinterpret_cast<char*>(REX_RAW_ADDR(ctx.r3.u32));
    const char* fmt =
        reinterpret_cast<const char*>(REX_RAW_ADDR(ctx.r4.u32));
    int n = rex_format_from_guest(dst, kRtlFormatMaxLen, fmt,
                                  ctx.r5.u32, ctx.r6.u32,
                                  ctx.r7.u32, ctx.r8.u32);
    ctx.r3.u64 = static_cast<uint32_t>(n < 0 ? 0 : n);
}

extern "C" void __imp__RtlSnprintf(PPCContext& ctx, uint8_t* base) {
    // r3 = dst, r4 = size, r5 = fmt, r6..r9 = args
    char* dst = reinterpret_cast<char*>(REX_RAW_ADDR(ctx.r3.u32));
    size_t cap = ctx.r4.u32;
    const char* fmt =
        reinterpret_cast<const char*>(REX_RAW_ADDR(ctx.r5.u32));
    int n = rex_format_from_guest(dst, cap, fmt,
                                  ctx.r6.u32, ctx.r7.u32,
                                  ctx.r8.u32, ctx.r9.u32);
    ctx.r3.u64 = static_cast<uint32_t>(n < 0 ? 0 : n);
}

extern "C" void __imp__RtlVsnprintf(PPCContext& ctx, uint8_t* base) {
    // r3 = dst, r4 = size, r5 = fmt, r6 = va_list ptr (guest)
    // We can't decode a guest va_list portably; pull four ints
    // directly from the pointed-to memory as a best-effort.
    char* dst = reinterpret_cast<char*>(REX_RAW_ADDR(ctx.r3.u32));
    size_t cap = ctx.r4.u32;
    const char* fmt =
        reinterpret_cast<const char*>(REX_RAW_ADDR(ctx.r5.u32));
    uint32_t* va = ctx.r6.u32
        ? reinterpret_cast<uint32_t*>(REX_RAW_ADDR(ctx.r6.u32))
        : nullptr;
    uint32_t a0 = va ? va[0] : 0;
    uint32_t a1 = va ? va[1] : 0;
    uint32_t a2 = va ? va[2] : 0;
    uint32_t a3 = va ? va[3] : 0;
    int n = rex_format_from_guest(dst, cap, fmt, a0, a1, a2, a3);
    ctx.r3.u64 = static_cast<uint32_t>(n < 0 ? 0 : n);
}

// --- xcore::FailAssert: log the assert site ---
//
// The game's assert text only goes to its console print hook (not the log),
// so record the arguments before the game builds its callstack report.
// r3/r4 carry the message/file strings, r5 the line.
static const char* GuestStringOrNull(uint8_t* base, uint32_t ea) {
    if (ea < 0x82000000u || ea >= 0x83400000u) {
        return "(null)";
    }
    return reinterpret_cast<const char*>(base + ea);
}

// DIAGNOSTIC: the Localizer's format-token parser (sub_82ABB480, r4 = the
// localized text). Its Localizer.cpp(1642) assert fires on a {N:type} token
// whose type is not registered; remember the text so the assert log below
// names it.
static thread_local uint32_t tl_localizer_text = 0;
extern "C" REX_FUNC(sub_82ABB480) {
    tl_localizer_text = ctx.r4.u32;
    __imp__sub_82ABB480(ctx, base);
    tl_localizer_text = 0;
}

extern "C" REX_FUNC(rex_xcore_FailAssert_YAXPBD0H_Z_82B58838) {
    REXLOG_ERROR("[assert] FailAssert r3='{}' r4='{}' r5={} (return 0x{:08X})",
                 GuestStringOrNull(base, ctx.r3.u32), GuestStringOrNull(base, ctx.r4.u32),
                 ctx.r5.s32, uint32_t(ctx.lr));
    if (ctx.r5.s32 == 1642 && tl_localizer_text >= 0x10000) {
        char text[257] = {};
        const char* src = reinterpret_cast<const char*>(REX_RAW_ADDR(tl_localizer_text));
        for (int i = 0; i < 256 && src[i] != 0; ++i) {
            text[i] = src[i];
        }
        REXLOG_ERROR("[assert] Localizer: unregistered format type in text '{}'", text);
    }
    rex::FlushLogging();
    __imp__rex_xcore_FailAssert_YAXPBD0H_Z_82B58838(ctx, base);
}


// DIAGNOSTIC: the CRT's _purecall. A load-time crash (2026-11-17) was a pure
// virtual call from the app Render (sub_8283B3B0, call at 0x8283B6D4) on an
// object fetched via AppMan[+64]->vfunc24. r3 still holds that object here,
// so log it with its vtable (named via the .map) before the CRT aborts.
extern "C" REX_FUNC(rex_purecall_83190B90) {
    uint32_t obj = ctx.r3.u32;
    uint32_t vt = (obj >= 0x10000u) ? REX_LOAD_U32(obj) : 0u;
    uint32_t slot[8] = {};
    if (vt >= 0x82000000u && vt < 0x84000000u) {
        for (int i = 0; i < 8; ++i) slot[i] = REX_LOAD_U32(vt + 4u * i);
    }
    uint32_t app = REX_LOAD_U32(0x83367290u);
    uint32_t app64 = app >= 0x10000u ? REX_LOAD_U32(app + 64u) : 0u;
    uint32_t app64vt = app64 >= 0x10000u ? REX_LOAD_U32(app64) : 0u;
    REXLOG_ERROR("[purecall] obj=0x{:08X} vtable=0x{:08X} lr=0x{:08X} r31=0x{:08X} | slots {:08X} {:08X} {:08X} {:08X} "
                 "{:08X} {:08X} {:08X} {:08X} | app=0x{:08X} app+64=0x{:08X} (vt 0x{:08X})",
                 obj, vt, uint32_t(ctx.lr), ctx.r31.u32, slot[0], slot[1], slot[2], slot[3], slot[4], slot[5],
                 slot[6], slot[7], app, app64, app64vt);
    rex::FlushLogging();
    __imp__rex_purecall_83190B90(ctx, base);
}

// --- DataSysMan::SetStreamerController: deferred delete (game race) ---
//
// The game deletes the old streamer controller *before* publishing the new
// one, with no sync against the render thread, which does
// DataSysMan->m_streamer(+36)->UpdateStreamer() every frame (app Render,
// sub_8283B3B0 at 0x8283B6D4). When the flow thread swaps controllers during
// load, Render can fetch the old pointer and call into a dying object: pure
// virtual call (vtable reset to the base) or an access violation once freed.
// Rare on the 360; on PC under CPU load ~2% of loads crashed (2026-11-17).
// Fix: publish the new pointer first, and run the old controller's deleting
// destructor later, on the same thread that swapped it, after kDelay. No
// render frame holds the pointer for anywhere near that long.
REXCVAR_DEFINE_BOOL(sk83_streamer_swap_fix, true, "sk83",
                    "Defer deleting a replaced streamer controller (fixes a load-time race crash)");
namespace {
struct RetiredStreamer {
  uint32_t obj;
  DWORD thread;
  ULONGLONG when_ms;
};
std::mutex g_retired_mu;
std::vector<RetiredStreamer> g_retired;
std::atomic<uint32_t> g_retired_n{0};
constexpr ULONGLONG kRetireDelayMs = 2000;

void DrainRetiredStreamers(PPCContext& ctx, uint8_t* base, bool force) {
  if (g_retired_n.load(std::memory_order_relaxed) == 0) return;
  const DWORD me = GetCurrentThreadId();
  const ULONGLONG now = GetTickCount64();
  std::vector<uint32_t> due;
  {
    std::lock_guard<std::mutex> lock(g_retired_mu);
    for (size_t i = 0; i < g_retired.size();) {
      const ULONGLONG age = now - g_retired[i].when_ms;
      // the swapping thread deletes after kRetireDelayMs; any thread after 5x
      // (the first swap comes from a short-lived startup thread)
      if (force || (g_retired[i].thread == me ? age >= kRetireDelayMs : age >= 5 * kRetireDelayMs)) {
        due.push_back(g_retired[i].obj);
        g_retired[i] = g_retired.back();
        g_retired.pop_back();
      } else {
        ++i;
      }
    }
    g_retired_n.store(uint32_t(g_retired.size()), std::memory_order_relaxed);
  }
  if (due.empty()) return;
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, lr = ctx.lr;
  for (uint32_t obj : due) {
    REXLOG_INFO("streamer swap fix: deleting retired controller 0x{:08X}", obj);
    ctx.r3.u64 = obj;
    ctx.r4.u64 = 1;  // deleting destructor: free the memory too
    REX_CALL_INDIRECT_FUNC(REX_LOAD_U32(REX_LOAD_U32(obj)));
  }
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.lr = lr;
}
}  // namespace

extern "C" REX_FUNC(rex_Sk8_DataSysMan_SetStreamerController_UAAXPAVIStreamerController_2_Z_8289CD48) {
  if (!REXCVAR_GET(sk83_streamer_swap_fix)) {
    __imp__rex_Sk8_DataSysMan_SetStreamerController_UAAXPAVIStreamerController_2_Z_8289CD48(ctx, base);
    return;
  }
  const uint32_t self = ctx.r3.u32;
  const uint32_t next = ctx.r4.u32;
  const uint32_t old = REX_LOAD_U32(self + 36u);
  if (next == old) return;
  std::atomic_thread_fence(std::memory_order_seq_cst);
  REX_STORE_U32(self + 36u, next);  // publish first
  std::atomic_thread_fence(std::memory_order_seq_cst);
  if (old != 0) {
    std::lock_guard<std::mutex> lock(g_retired_mu);
    g_retired.push_back({old, GetCurrentThreadId(), GetTickCount64()});
    REXLOG_INFO("streamer swap fix: 0x{:08X} -> 0x{:08X}, old one retired", old, next);
    g_retired_n.store(uint32_t(g_retired.size()), std::memory_order_relaxed);
  }
}

// The flow thread that swaps controllers calls GetDataSysMan regularly, so
// retired controllers are deleted from here once their delay has passed.
extern "C" REX_FUNC(rex_Sk8_Sk8AppMan_GetDataSysMan_QBAAAVDataSysMan_2_XZ_82832528) {
  __imp__rex_Sk8_Sk8AppMan_GetDataSysMan_QBAAAVDataSysMan_2_XZ_82832528(ctx, base);
  if (g_retired_n.load(std::memory_order_relaxed) != 0) {
    const uint32_t r3 = ctx.r3.u32;
    DrainRetiredStreamers(ctx, base, false);
    ctx.r3.u64 = r3;
  }
}

// --- rex_DebugBreak_82DAD568: log-and-halt ---
//
// Previously this hook was a pure no-op so the game could continue
// past assertions. Downside: an assertion means the game itself has
// detected an invariant violation, so *continuing* past it runs code
// with guaranteed-bad state, and we crash a few instructions later
// on a bogus pointer read. That's exactly what happened on the
// filesystem-worker crash in rex_assman_cAssetLoader_AssetClosedFailedCallback_CAXPAVAsyncOp_filesys_core_rw_Z_82FCE1C0 -- FailAssert(0, _, 388)
// fired, DebugBreak was no-op'd, execution fell through to
// loc_82FCE228, and the very next chained load dereferenced garbage.
//
// The current strategy: log everything useful we can extract from
// the PPC context, then actually halt the process (via host
// __debugbreak). We lose "run further into gameplay" but we gain
// diagnosability: every assertion prints its guest return address
// so we can trace back to the exact code site (and, from there, the
// FailAssert argument constants -- msg pointer, file pointer, line
// number) that caught the invariant violation.
//
// The two args that matter most here:
//   ctx.lr       -- return address of the bl to DebugBreak; the
//                   instruction after that in the generated source
//                   is where the game asserted.
//   ctx.r3..r6   -- whatever FailAssert left in the arg registers
//                   just before DebugBreak. Often r3=0 (null msg),
//                   r4=file-string pointer, r5=line number.
//
// To go back to run-through-asserts mode: comment out
// `::DebugBreak();` in the hook body -- the log stays, the halt
// disappears, and downstream corruption resumes.
REX_HOOK_RAW(rex_DebugBreak_82DAD568) {
    (void)base;

    // Try to read the file-name string that FailAssert usually passes
    // in r4. It's a guest pointer, so translate through the memory
    // base. Bail cleanly if the pointer looks obviously bogus.
    const char* file = "?";
    uint32_t file_ea = ctx.r4.u32;
    if (file_ea >= 0x82000000u && file_ea < 0x83400000u) {
        file = reinterpret_cast<const char*>(REX_RAW_ADDR(file_ea));
    }

    char buf[512];
    std::snprintf(buf, sizeof(buf),
        "[assert] guest DebugBreak fired\n"
        "         caller LR (guest) = 0x%08X\n"
        "         r3=%08X  r4=%08X  r5=%d  r6=%08X\n"
        "         file (guessed from r4) = %s\n",
        uint32_t(ctx.lr),
        ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r6.u32,
        file);
    ::OutputDebugStringA(buf);
    REXLOG_ERROR("{}", buf);

    // Actually halt so we don't run into corrupted downstream state.
    // Comment this out (leaving only the log) to trade correctness
    // for progress -- useful when you know a particular assert is
    // benign but want to see any others that come after.
    //::DebugBreak();
}

// --- Non-fatal CRT asserts for the SDK runtime -----------------------------
//
// rexruntimerd.dll is a RelWithDebInfo build, so its assert()s are live. One
// of them is reached in normal operation:
//
//   XmaContext::DecodeOldFrame()
//     BitStream stream(current_input_buffer, current_input_size * 8);
//     stream.SetOffset(data->input_buffer_read_offset);   // assert_false(...)
//     if (data->input_buffer_read_offset > current_input_size * 8) {
//       REXAPU_ERROR("input offset {} exceeds buffer size {}", ...);
//       SwapInputBuffer(data);                            // recovery
//       return;
//     }
//
// The SDK already knows that offset can exceed the buffer and handles it --
// but the assert fires one line BEFORE the check, so the process dies before
// the recovery runs. A Release SDK build (asserts compiled out, which is what
// the stock prebuilt runtime was) reaches the recovery and keeps playing.
//
// Rather than modify the SDK, patch the DLL's import of ucrtbase's _wassert
// in memory so it logs and returns instead of aborting. That reproduces
// Release behaviour for every SDK assert while leaving the binary on disk
// untouched.
namespace {

std::atomic<uint64_t> g_softened_asserts{0};

void __cdecl SoftAssert(const wchar_t* expr, const wchar_t* file, unsigned line) {
  const uint64_t n = g_softened_asserts.fetch_add(1, std::memory_order_relaxed);
  if (n < 64 || (n % 1000) == 0) {
    auto narrow = [](const wchar_t* w) {
      std::string out;
      for (; w != nullptr && *w != 0; ++w) {
        out.push_back(*w < 128 ? char(*w) : '?');
      }
      return out;
    };
    REXLOG_WARN("SDK assert (non-fatal, #{}): {} at {}:{}", n + 1, narrow(expr),
                narrow(file), line);
  }
}

bool PatchImportedWassert(HMODULE module) {
  auto* bytes = reinterpret_cast<uint8_t*>(module);
  auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(bytes);
  if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
    return false;
  }
  auto* nt = reinterpret_cast<IMAGE_NT_HEADERS*>(bytes + dos->e_lfanew);
  if (nt->Signature != IMAGE_NT_SIGNATURE) {
    return false;
  }
  const auto& dir =
      nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
  if (dir.VirtualAddress == 0) {
    return false;
  }
  auto* desc = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(bytes + dir.VirtualAddress);
  bool patched = false;
  for (; desc->Name != 0; ++desc) {
    if (desc->OriginalFirstThunk == 0 || desc->FirstThunk == 0) {
      continue;
    }
    auto* orig = reinterpret_cast<IMAGE_THUNK_DATA*>(bytes + desc->OriginalFirstThunk);
    auto* iat = reinterpret_cast<IMAGE_THUNK_DATA*>(bytes + desc->FirstThunk);
    for (; orig->u1.AddressOfData != 0; ++orig, ++iat) {
      if (IMAGE_SNAP_BY_ORDINAL(orig->u1.Ordinal)) {
        continue;
      }
      auto* import =
          reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(bytes + orig->u1.AddressOfData);
      if (std::strcmp(import->Name, "_wassert") != 0) {
        continue;
      }
      DWORD old = 0;
      if (!VirtualProtect(&iat->u1.Function, sizeof(iat->u1.Function),
                          PAGE_READWRITE, &old)) {
        continue;
      }
      iat->u1.Function = reinterpret_cast<ULONGLONG>(&SoftAssert);
      VirtualProtect(&iat->u1.Function, sizeof(iat->u1.Function), old, &old);
      patched = true;
    }
  }
  return patched;
}

}  // namespace

void Sk83InstallSoftAsserts() {
  static bool done = false;
  if (done) {
    return;
  }
  done = true;
  HMODULE m = GetModuleHandleA("rexruntimerd.dll");
  if (m == nullptr) {
    m = GetModuleHandleA("rexruntime.dll");
  }
  if (m == nullptr) {
    REXLOG_WARN("Soft asserts: rexruntime module not found");
    return;
  }
  // Breakpoint diagnostics: the SDK ends guest DbgBreakPoint / RtlRaiseException /
  // KeBugCheck in rex::debug::Break(); without a debugger that kills the process with
  // nothing in the (buffered) log. Log the host return-address chain (module+offset,
  // resolve with llvm-symbolizer and the PDBs) and flush first; the crash proceeds.
  struct Chain {
    static std::string Of(const void* fault) {
      void* frames[48] = {};
      const USHORT n = RtlCaptureStackBackTrace(0, 48, frames, nullptr);
      std::string chain = " fault=" + Name(fault) + " |";
      for (USHORT i = 0; i < n; ++i) chain += " " + Name(frames[i]);
      return chain;
    }
    static std::string Name(const void* p) {
      HMODULE mod = nullptr;
      char name[MAX_PATH] = "?";
      if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                 GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                             reinterpret_cast<LPCSTR>(p), &mod) &&
          mod != nullptr) {
        GetModuleFileNameA(mod, name, MAX_PATH);
      }
      const char* leaf = std::strrchr(name, 0x5C);  // last path separator
      char item[160];
      std::snprintf(item, sizeof(item), "%s+0x%llX", leaf ? leaf + 1 : name,
                    (unsigned long long)(reinterpret_cast<uintptr_t>(p) - reinterpret_cast<uintptr_t>(mod)));
      return item;
    }
  };
  AddVectoredExceptionHandler(1, [](EXCEPTION_POINTERS* ep) -> LONG {
    if (ep->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT) {
      return EXCEPTION_CONTINUE_SEARCH;
    }
    REXLOG_CRITICAL("[breakpoint] host break at {} | chain:{}", ep->ExceptionRecord->ExceptionAddress,
                    Chain::Of(ep->ExceptionRecord->ExceptionAddress));
    rex::FlushLogging();
    return EXCEPTION_CONTINUE_SEARCH;
  });
  // Unhandled crashes (access violations etc.) died silently: first-chance AVs
  // can be legitimate (the runtime handles some), so log only at the
  // unhandled-filter stage, then hand on to whatever filter was there.
  static LPTOP_LEVEL_EXCEPTION_FILTER s_prev_filter = nullptr;
  s_prev_filter = SetUnhandledExceptionFilter([](EXCEPTION_POINTERS* ep) -> LONG {
    const EXCEPTION_RECORD* er = ep->ExceptionRecord;
    REXLOG_CRITICAL("[crash] unhandled 0x{:08X} at {} (access {} 0x{:X}) | chain:{}",
                    uint32_t(er->ExceptionCode), er->ExceptionAddress,
                    er->NumberParameters >= 2 ? er->ExceptionInformation[0] : 0,
                    er->NumberParameters >= 2 ? er->ExceptionInformation[1] : 0,
                    Chain::Of(er->ExceptionAddress));
    rex::FlushLogging();
    return s_prev_filter ? s_prev_filter(ep) : EXCEPTION_CONTINUE_SEARCH;
  });
  REXLOG_INFO("Soft asserts: {}",
              PatchImportedWassert(m)
                  ? "SDK assert() is now non-fatal (logged, execution continues)"
                  : "no _wassert import found; asserts remain fatal");
}
