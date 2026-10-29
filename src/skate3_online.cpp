// Online play (EA Nation / Blaze) on the recomp.
//
// The game's Net config has an entry choosing the secure transport: when set,
// PlasmaSystemPlatform::InitDirtySock starts DirtySock without "-nosecure"
// (the Xbox LIVE encrypted connection that needs an LSP server address), and
// the Blaze hub parameters mark the redirector connection secure (SSL). The
// recomp's sockets are plain IP and the community Blaze server is insecure,
// so a secure setup never connects ("Connecting to EA Nation..." forever).
// The entry is forced to insecure before both readers.

#include "generated/sk83_na_m_init.h"

#include <cstdint>
#include <cstring>
#include <string_view>

#include <rex/logging.h>

namespace {

// Pointer to the config entry; its value byte lives behind the pointer at
// +208, with the default at +205 (read by both functions below).
constexpr uint32_t kSecureTransportEntrySlot = 0x833633C4u;

void ForceInsecureTransport(uint8_t* base) {
  const uint32_t entry = REX_LOAD_U32(kSecureTransportEntrySlot);
  if (entry == 0) {
    return;
  }
  REX_STORE_U8(entry + 205, 0);
  if (const uint32_t value = REX_LOAD_U32(entry + 208)) {
    REX_STORE_U8(value, 0);
  }
}

}  // namespace

// Sk8::Net::PlasmaSystemPlatform::InitDirtySock: XNetStartup + NetConnStartup.
extern "C" REX_FUNC(rex_Sk8_Net_PlasmaSystemPlatform_InitDirtySock_MAAXXZ_828A03B0) {
  ForceInsecureTransport(base);
  __imp__rex_Sk8_Net_PlasmaSystemPlatform_InitDirtySock_MAAXXZ_828A03B0(ctx, base);
}

// Fills the Blaze hub init parameters (service name, secure flag, versions).
extern "C" REX_FUNC(sub_828A0250) {
  ForceInsecureTransport(base);
  __imp__sub_828A0250(ctx, base);
}

// --- TEMPORARY matchmaking diagnostics (remove once the local player resolves) ---
// Game::InitGame dereferences the Blaze game's local player (+508), which stays
// null when no player in the lobby is recognized as the signed-in user.

namespace {
const char* GuestCString(uint8_t* base, uint32_t ea) {
  if (ea < 0x40000000u || ea >= 0x90000000u) {
    return "(null)";
  }
  return reinterpret_cast<const char*>(base + ea);
}
}  // namespace

// Blaze UserManager acquireUser(manager, blazeId, name, externalId, ...)
extern "C" REX_FUNC(sub_82BCB290) {
  const uint32_t blaze_id = ctx.r4.u32;
  const uint32_t name_ea = ctx.r5.u32;
  const uint64_t r6 = ctx.r6.u64, r7 = ctx.r7.u64, r8 = ctx.r8.u64, r9 = ctx.r9.u64;
  const uint32_t caller = uint32_t(ctx.lr);
  __imp__sub_82BCB290(ctx, base);
  const uint32_t user = ctx.r3.u32;
  REXLOG_INFO("[mm-diag] acquireUser blazeId={} name='{}' r6={:X} r7={:X} r8={:X} r9={:X} caller={:08X} "
              "-> user={:08X} user.blazeId(+20)={} localMask(+26)={:04X}",
              blaze_id, GuestCString(base, name_ea), r6, r7, r8, r9, caller, user,
              user ? REX_LOAD_U32(user + 20) : 0, user ? REX_LOAD_U16(user + 26) : 0);
}

// Blaze local user registration (login session -> user object)
extern "C" REX_FUNC(sub_82BCB558) {
  const uint32_t manager = ctx.r3.u32;
  REXLOG_INFO("[mm-diag] registerLocalUser manager={:08X} r4={:X} r5={:X} r6={:X} r7={:X} caller={:08X}",
              manager, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64, uint32_t(ctx.lr));
  __imp__sub_82BCB558(ctx, base);
}

namespace zdx {
void OnGamePrint(PPCContext& ctx, uint8_t* base, uint32_t text);
}

// Game debug output. xcore::ConsolePrintFunction formats into a buffer and
// hands it to this OutputDebugStringA wrapper, which goes to the console's
// debug channel (not the XAM export the SDK logs), so the text was lost.
extern "C" REX_FUNC(rex_OutputDebugStringA_82DAE610) {
  const char* text = GuestCString(base, ctx.r3.u32);
  size_t length = std::strlen(text);
  while (length > 0 && (text[length - 1] == '\n' || text[length - 1] == '\r')) {
    --length;
  }
  if (length > 0) {
    REXLOG_INFO("[game] {}", std::string_view(text, length));
  }
#ifdef SK83_ZD_DEBUG
  zdx::OnGamePrint(ctx, base, ctx.r3.u32);
#endif
  __imp__rex_OutputDebugStringA_82DAE610(ctx, base);
}

// BlazeSDK print: formats the message, then calls the logger pointer set at
// BlazeHub::initialize (null in this build, so every [CANA]/[GAME]/[GMGR]
// line was dropped). Point it at the OutputDebugStringA wrapper above.
constexpr uint32_t kBlazeLoggerSlot = 0x832DB5BCu;
constexpr uint32_t kGameOutputDebugStringA = 0x82DAE610u;

extern "C" REX_FUNC(sub_82B91DF8) {
  if (REX_LOAD_U32(kBlazeLoggerSlot) == 0) {
    REX_STORE_U32(kBlazeLoggerSlot, kGameOutputDebugStringA);
  }
  __imp__sub_82B91DF8(ctx, base);
}

// Blaze Game add player (creates the player, stores it at +508 when local)
extern "C" REX_FUNC(rex_unknown_82BAD438) {
  const uint32_t game = ctx.r3.u32;
  const uint32_t player_data = ctx.r4.u32;
  const uint32_t pid = player_data ? REX_LOAD_U32(player_data + 12) : 0;
  __imp__rex_unknown_82BAD438(ctx, base);
  const uint32_t player = ctx.r3.u32;
  REXLOG_INFO("[mm-diag] addPlayer game={:08X} pid={} -> player={:08X} user={:08X} localPlayer(+508)={:08X}",
              game, pid, player, player ? REX_LOAD_U32(player + 16) : 0,
              game ? REX_LOAD_U32(game + 508) : 0);
}

// ---------------------------------------------------------------------------
// XStringVerify (sub_82E08840) - LIVE's profanity check on typed text.
//
//   DWORD XStringVerify(DWORD dwFlags, const CHAR* szLocale, DWORD dwNumStrings,
//                       const STRING_DATA* pStringData, DWORD cbResults,
//                       STRING_VERIFY_RESPONSE* pResults, XOVERLAPPED* pOverlapped);
//   STRING_VERIFY_RESPONSE { WORD wNumStrings; HRESULT* pStringResult; }  (packed)
//
// Same function as retail's sub_82F39B60 (identical code: XLIVEBASE message 0x5000C,
// 40-byte request), hooked the same way as in skate3recomp's skate3_online.cpp.
// The in-game browser runs it on text typed into a page (Create Team's team name
// and motto, Post Message, ...). The runtime's XLiveBaseApp has no case for
// 0x5000C ("Unimplemented XLIVEBASE message ... msg=0005000C" in the log), so
// pResults was never written and the browser froze / crashed right after the
// keyboard closed. There is no LIVE string service to ask, so every string
// passes: the packed response with its S_OK array placed right after the header
// inside the caller's cbResults buffer, as LIVE's reply would be unpacked.
// ---------------------------------------------------------------------------
#include <algorithm>
#include <rex/system/kernel_state.h>
#include <rex/system/xtypes.h>

extern "C" REX_FUNC(sub_82E08840) {
  using namespace rex;
  const uint32_t num_strings = ctx.r5.u32, cb_results = ctx.r7.u32;
  const uint32_t results = ctx.r8.u32, overlapped = ctx.r9.u32;
  constexpr uint32_t kHeader = 6;  // WORD wNumStrings + HRESULT* pStringResult
  uint32_t fit = 0;
  if (results && cb_results >= kHeader) {
    fit = std::min(num_strings, (cb_results - kHeader) / 4);
    REX_STORE_U16(results + 0, uint16_t(fit));
    REX_STORE_U32(results + 2, results + kHeader);
    for (uint32_t i = 0; i < fit; ++i) REX_STORE_U32(results + kHeader + i * 4, 0);  // S_OK
  }
  REXLOG_INFO("[live-gate] XStringVerify({} string(s), cb={}) -> all passed", num_strings, cb_results);
  uint32_t result = X_ERROR_SUCCESS;
  if (overlapped) {
    REX_KERNEL_STATE()->CompleteOverlappedImmediate(overlapped, X_ERROR_SUCCESS);
    result = X_ERROR_IO_PENDING;
  }
  ctx.r3.u64 = result;
}
