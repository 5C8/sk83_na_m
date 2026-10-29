// Localizer custom-argument formatters that retail Skate 3 registers and this
// development build lacks.
//
// Localized texts use {N:TYPE} tokens resolved by LocalizerManager's custom
// argument handlers (RegisterCustomArgInfo). This build registers only
// "points" (FEBackEnd::Init) and "d"; its data already uses retail tokens,
// e.g. the photo mission's sign-up poster: "Get some promotional material for
// {0:TEAMMATE}. ...". The unregistered type fired Localizer.cpp(1642).
//
// Ported from retail (sk83 default.xex):
//  - FEBackEnd::Init registers RegisterCustomArgInfo("TEAMMATE", handler).
//  - handler(out, size, index): index 1..count-1 names teammate record
//    index-1 of the career teammate list (name at record + 5); anything else
//    prints "ID_COMMON_UNKNOWN".
//  - SignUpPosterCmdTranslator::GetSignUpDescription passes the mission's
//    teammate index (attribute 0xF18EAB9D, one byte) as the custom-argument
//    string "%d" to FormatStringCustom. This build's older poster passes no
//    argument string (r5 = 0); it is supplied here the same way.
// This build's teammate list is the one its own teammate-select screen shows:
// *(*(*(0x8336CFAC) + 184) + 68) + 8, count = sub_82951188(list), record k =
// sub_82951258(list, k).

#include "generated/sk83_na_m_init.h"

#include <rex/hook.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/system/function_dispatcher.h>
#include <rex/system/kernel_state.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <stdlib.h>

namespace {

uint32_t LoadU32(uint8_t* base, uint32_t addr) {
  uint32_t v;
  std::memcpy(&v, REX_RAW_ADDR(addr), 4);
  return _byteswap_ulong(v);
}

// Guest memory for the strings handed to guest code (the formatter name and
// the poster's argument string), allocated once from the system heap.
uint32_t g_guest_scratch = 0;
constexpr uint32_t kNameOffset = 0;    // "TEAMMATE"
constexpr uint32_t kArgOffset = 32;    // poster argument string
constexpr uint32_t kScratchSize = 64;

uint32_t GuestScratch() {
  if (g_guest_scratch == 0) {
    g_guest_scratch = rex::system::kernel_memory()->SystemHeapAlloc(kScratchSize);
  }
  return g_guest_scratch;
}

// The career teammate list (see the file comment); 0 when unavailable.
uint32_t TeammateList(uint8_t* base) {
  const uint32_t sim = LoadU32(base, 0x8336CFAC);
  if (sim == 0) {
    return 0;
  }
  const uint32_t career = LoadU32(base, sim + 184);
  if (career == 0) {
    return 0;
  }
  const uint32_t progression = LoadU32(base, career + 68);
  return progression != 0 ? progression + 8 : 0;
}

// TEAMMATE handler: r3 = out, r4 = out size, r5 = teammate index. Returns the
// written length in r3 (retail: Snprintf's return).
void TeammateFormatter(PPCContext& ctx, uint8_t* base) {
  const uint32_t out = ctx.r3.u32;
  const uint32_t size = ctx.r4.u32;
  const int32_t index = ctx.r5.s32;
  const char* text = "ID_COMMON_UNKNOWN";
  char name[64] = {};
  const uint32_t list = TeammateList(base);
  if (list != 0 && index >= 1) {
    PPCContext call = ctx;
    call.r3.u64 = list;
    sub_82951188(call, base);
    const uint32_t count = call.r3.u32;
    if (uint32_t(index) < count) {
      call = ctx;
      call.r3.u64 = list;
      call.r4.u64 = uint32_t(index - 1);
      sub_82951258(call, base);
      const uint32_t record = call.r3.u32;
      if (record != 0) {
        const char* src = reinterpret_cast<const char*>(REX_RAW_ADDR(record + 5));
        size_t n = 0;
        while (n + 1 < sizeof(name) && src[n] != 0) {
          name[n] = src[n];
          ++n;
        }
        text = name;
      }
    }
  }
  size_t len = std::strlen(text);
  if (size == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  if (len >= size) {
    len = size - 1;
  }
  char* dst = reinterpret_cast<char*>(REX_RAW_ADDR(out));
  std::memcpy(dst, text, len);
  dst[len] = 0;
  ctx.r3.u64 = uint32_t(len);
}

// Poster formatting state (guest thread running GetSignUpDescription).
thread_local bool tl_in_poster = false;
thread_local int32_t tl_poster_teammate = -1;

}  // namespace

// FEBackEnd::Init: after the build's own "points" registration, register the
// retail TEAMMATE formatter once.
extern "C" REX_FUNC(rex_Sk8_BE_FEBackEnd_Init_UAAXXZ_8267EA40) {
  __imp__rex_Sk8_BE_FEBackEnd_Init_UAAXXZ_8267EA40(ctx, base);
  static bool s_registered = false;
  if (s_registered) {
    return;
  }
  const uint32_t scratch = GuestScratch();
  rex::Runtime* runtime = rex::Runtime::instance();
  if (scratch == 0 || runtime == nullptr || runtime->function_dispatcher() == nullptr) {
    REXLOG_WARN("localizer: TEAMMATE formatter not registered (no guest memory/dispatcher)");
    return;
  }
  const uint32_t handler = runtime->function_dispatcher()->AllocateThunk(&TeammateFormatter, 0);
  if (handler == 0) {
    REXLOG_WARN("localizer: TEAMMATE formatter not registered (thunk allocation failed)");
    return;
  }
  std::memcpy(REX_RAW_ADDR(scratch + kNameOffset), "TEAMMATE", 9);
  PPCContext call = ctx;
  call.r3.u64 = scratch + kNameOffset;
  call.r4.u64 = handler;
  rex_EA_Localizer_LocalizerManager_RegisterCustomArgInfo_SAXPBDP6AI0PADI_Z_Z_82ABB0A8(call, base);
  s_registered = true;
  REXLOG_INFO("localizer: registered retail TEAMMATE formatter (handler thunk 0x{:08X})", handler);
}

// SignUpPosterCmdTranslator::GetSignUpDescription: mark the poster's scope.
extern "C" REX_FUNC(
    rex_Sk8_FE_SignUpPosterCmdTranslator_sMethod_GetSignUpDescription_SAPAVAptValue_PAV4_H_Z_82717E90) {
  tl_in_poster = true;
  tl_poster_teammate = -1;
  __imp__rex_Sk8_FE_SignUpPosterCmdTranslator_sMethod_GetSignUpDescription_SAPAVAptValue_PAV4_H_Z_82717E90(
      ctx, base);
  tl_in_poster = false;
}

// Attrib::Instance::Get(uint64 key): when the poster reads the mission's
// description (key low word 0xE508B742), also read its teammate index from
// the same instance (key 0x0AF3F024'F18EAB9D, one byte), as retail does.
extern "C" REX_FUNC(rex_Attrib_Instance_GetAttributePointer_IBAPBX_KI_Z_82D77458) {
  if (tl_in_poster && uint32_t(ctx.r4.u64) == 0xE508B742u) {
    PPCContext call = ctx;
    call.r4.u64 = 0x0AF3F024F18EAB9Dull;
    __imp__rex_Attrib_Instance_GetAttributePointer_IBAPBX_KI_Z_82D77458(call, base);
    const uint32_t value = call.r3.u32;
    if (value != 0) {
      tl_poster_teammate = *reinterpret_cast<const uint8_t*>(REX_RAW_ADDR(value));
    }
  }
  __imp__rex_Attrib_Instance_GetAttributePointer_IBAPBX_KI_Z_82D77458(ctx, base);
}

// FormatStringCustom(out, id, args): inside the poster, supply the teammate
// index argument string the build's poster omits.
extern "C" REX_FUNC(rex_unknown_82ABCF48) {
  if (tl_in_poster && ctx.r5.u32 == 0 && tl_poster_teammate >= 0) {
    const uint32_t scratch = GuestScratch();
    if (scratch != 0) {
      char* arg = reinterpret_cast<char*>(REX_RAW_ADDR(scratch + kArgOffset));
      std::snprintf(arg, kScratchSize - kArgOffset, "%d", tl_poster_teammate);
      ctx.r5.u64 = scratch + kArgOffset;
    }
  }
  __imp__rex_unknown_82ABCF48(ctx, base);
}
