// zd_debug: debug features from the Aug 2009 sk83_na_zd build that the
// Dec 2009 sk83_na_m build had compiled out, re-added as hooks.
//
// Everything lives in this folder. To remove it: delete src/zd_debug/ and the
// SK83_ZD_DEBUG block in CMakeLists.txt. At runtime the master cvar
// `zd_debug` (default on) turns every feature off; each feature also has its
// own cvar (see README.md for the list and their status).

#pragma once

#include <cstdint>

#include <rex/cvar.h>

REXCVAR_DECLARE(bool, zd_debug);

namespace zd_debug {

// True when the master switch is on. Every hook checks this first and falls
// straight through to the game's own code when it is off.
inline bool Enabled() { return REXCVAR_GET(zd_debug); }

// A feature is on when both the master switch and its own cvar are on.
inline bool On(bool feature_cvar) { return Enabled() && feature_cvar; }

// Per-game-frame callbacks (run from Sk8::Presentation::Update, the point
// where zd ran DebugOverlay::Update). Features register with a static
//   static zd_debug::FrameHook hook(&MyFeatureFrame);
// and draw their debug output through zd_debug::overlay from it.
// Value of one of the GAME's own debug cvars (the options its debug menus
// and Config Variables show), read the way the game's code reads them:
//   var = *(obj + 32); p = *(var + 208); value = p ? *p : *(var + 205)
// `obj` is the cvar object the game registered (m addresses; see README).
bool GameCvarBool(uint8_t* base, uint32_t obj);
float GameCvarFloat(uint8_t* base, uint32_t obj);
int32_t GameCvarInt(uint8_t* base, uint32_t obj);

using FrameFn = void (*)(uint8_t* base);
struct FrameHook {
  explicit FrameHook(FrameFn fn);
};
void OnFrame(uint8_t* base);

}  // namespace zd_debug
