// Master switch and per-frame dispatch for the debug features ported from
// sk83_na_zd (see zd_debug.h).

#include "zd_debug.h"
#include "zdx.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#include <rex/logging.h>

#include "generated/sk83_na_m_init.h"

REXCVAR_DEFINE_BOOL(zd_debug, true, "Skate 3 zd debug",
                    "Enable the debug features ported from the sk83_na_zd build (master switch)");
REXCVAR_DEFINE_STRING(zd_debug_set, "", "Skate 3 zd debug",
                      "Game debug options to set at startup, e.g. "
                      "\"kShowAnimationSkeleton=1;kDrawCentreOfMass=1\" (same as the debug menu)");

namespace zd_debug {
namespace {

std::vector<FrameFn>& Hooks() {
  static std::vector<FrameFn> hooks;  // filled during static initialization
  return hooks;
}

}  // namespace

FrameHook::FrameHook(FrameFn fn) { Hooks().push_back(fn); }

// Game cvar objects (both builds use the same layout):
//   bool:  var = *(obj+32); override = *(var+208); value = override ? *override : *(u8*)(var+205)
//   float/int: var = *(obj+40); value at var+216
bool GameCvarBool(uint8_t* base, uint32_t obj) {
  const uint32_t var = REX_LOAD_U32(obj + 32);
  if (var == 0) return false;
  const uint32_t ovr = REX_LOAD_U32(var + 208);
  return (ovr ? REX_LOAD_U8(ovr) : REX_LOAD_U8(var + 205)) != 0;
}
float GameCvarFloat(uint8_t* base, uint32_t obj) {
  const uint32_t var = REX_LOAD_U32(obj + 40);
  if (var == 0) return 0.0f;
  const uint32_t bits = REX_LOAD_U32(var + 216);
  float f;
  std::memcpy(&f, &bits, 4);
  return f;
}
int32_t GameCvarInt(uint8_t* base, uint32_t obj) {
  const uint32_t var = REX_LOAD_U32(obj + 40);
  return var ? int32_t(REX_LOAD_U32(var + 216)) : 0;
}

namespace {

// zd_debug_set: applied once each option's variable exists.
void ApplyStartupOptions(uint8_t* base) {
  static std::vector<std::pair<std::string, std::string>> pending = [] {
    std::vector<std::pair<std::string, std::string>> v;
    std::string s = REXCVAR_GET(zd_debug_set);
    size_t i = 0;
    while (i < s.size()) {
      size_t j = s.find_first_of(";,", i);
      if (j == std::string::npos) j = s.size();
      const std::string item = s.substr(i, j - i);
      const size_t eq = item.find('=');
      if (eq != std::string::npos) v.emplace_back(item.substr(0, eq), item.substr(eq + 1));
      i = j + 1;
    }
    return v;
  }();
  for (auto it = pending.begin(); it != pending.end();) {
    const uint32_t obj = zdx::CvarObject(it->first.c_str());
    if (!obj) {
      REXLOG_WARN("zd_debug_set: unknown option {}", it->first);
      it = pending.erase(it);
      continue;
    }
    // The value block pointer sits at +32 (bool), +40 (int/float) or +36
    // (the 83D91738/82D8A658 kind) of the object, by registration kind.
    const uint32_t off = zdx::CvarVarOffset(it->first.c_str());
    const uint32_t var = off ? REX_LOAD_U32(obj + off) : 0;
    if (!var) { ++it; continue; }  // not created yet
    const double v = std::atof(it->second.c_str());
    if (off == 32) {
      // bool: value byte +205, or the override byte +208 points at
      const uint32_t ovr = REX_LOAD_U32(var + 208);
      REX_STORE_U8(ovr ? ovr : var + 205, v != 0);
    } else if (off == 36) {
      REX_STORE_U32(var + 208, uint32_t(int32_t(v)));  // int value (read by zd at +208)
    } else if (it->second.find('.') != std::string::npos) {
      const float f = float(v);
      uint32_t bits;
      std::memcpy(&bits, &f, 4);
      REX_STORE_U32(var + 216, bits);
    } else {
      REX_STORE_U32(var + 216, uint32_t(int32_t(v)));
    }
    REXLOG_INFO("zd_debug_set: {} = {}", it->first, it->second);
    it = pending.erase(it);
  }
}

}  // namespace

void OnFrame(uint8_t* base) {
  if (!Enabled()) return;
  ApplyStartupOptions(base);
  for (FrameFn fn : Hooks()) fn(base);
}

}  // namespace zd_debug
