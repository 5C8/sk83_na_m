// Cvar storage for the draw-distance/LOD sliders in the native debug dialog.
//
// Split out of skate3_draw_distance.cpp on purpose: that file's actual
// hooks (the guest-address rescaling logic these cvars feed) are still
// excluded pending sk83_na_m address resolution (see
// NATIVE_RENDERER_PORT_NOTES.md), but skate3_native_debug_dialog.cpp
// REXCVAR_DECLAREs and reads/writes these three regardless, so the
// storage needs to exist and be linked in even while the hooks
// themselves are dark. Once skate3_draw_distance.cpp's addresses are
// resolved and it's added back into the build, these definitions should
// move back into that file and this one can go away -- don't duplicate
// the REXCVAR_DEFINE lines in both places, that's an ODR violation.

#include <rex/cvar.h>

REXCVAR_DEFINE_DOUBLE(skate3_draw_distance_scale, 2.0, "Skate 3",
                      "Scale the distance at which small world meshes "
                      "(foliage, props, street furniture) stop being drawn. "
                      "1 = original console behavior. Larger values draw more "
                      "of the world and cost proportionally more GPU/CPU. "
                      "NOTE: not yet wired to any hook on sk83_na_m -- the "
                      "slider exists but changing it currently does nothing "
                      "until skate3_draw_distance.cpp's addresses are resolved.")
    .range(0.25, 16.0)
    .lifecycle(rex::cvar::Lifecycle::kHotReload);
REXCVAR_DEFINE_DOUBLE(skate3_lod_distance_scale, 2.0, "Skate 3",
                      "Scale the distances at which skaters, pedestrians and "
                      "vehicles switch to lower-detail models. 1 = original "
                      "console behavior. NOTE: not yet wired to any hook on "
                      "sk83_na_m.")
    .range(0.25, 16.0)
    .lifecycle(rex::cvar::Lifecycle::kHotReload);
REXCVAR_DEFINE_DOUBLE(skate3_draw_distance_stream_probe, 0.0, "Skate 3",
                      "Pre-stream world detail cells this many metres ahead "
                      "of the camera focus. 0 = original console behavior. "
                      "NOTE: not yet wired to any hook on sk83_na_m.")
    .range(0.0, 1000.0)
    .lifecycle(rex::cvar::Lifecycle::kHotReload);
