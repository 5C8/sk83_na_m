# sk83_na_m

Skate 3 milestone recompilation with the native renderer, PC integration and full ZD debugging features.

This is the `5C8` publication snapshot of the working projects, based on
[mchughalex/skate3recomp](https://github.com/mchughalex/skate3recomp) and the
[Skate 3 ReXGlue fork](https://github.com/mchughalex/rexglue-skate3).
It uses the companion [5C8 SDK](https://github.com/5C8/rexglue-skate3).

## Download and play

[**Windows x64 releases**](https://github.com/5C8/sk83_na_m/releases/latest)

1. Download and fully extract **sk83_na_m-Windows-x64.zip** from Releases.
2. Put your matching milestone game tree in `game/` beside `sk83_na_m.exe`. Its
   root must contain `sk83_na_m.xex`, and its original `data/` tree must be intact.
   This build needs milestone data; a retail ISO is not a substitute.
3. Start from the extracted release folder. To use a different game directory,
   run `sk83_na_m.exe --game_data_root=C:/Games/sk83_na_m`.

The release contains the application and its runtime dependencies. Provide your
own matching game files. Windows x64 with an x86-64-v3 CPU (AVX2/BMI2), a
DirectX 12 or Vulkan GPU, and the Visual C++ x64 runtime is required. Linux/ARM
and macOS binaries are not published in this snapshot.

## Included features

- Custom/native DirectX 12 and Vulkan rendering, enabled by default; **F5**
  switches to/from the emulated renderer.
- Video enhancements, enhanced fonts, ultrawide HUD/world-marker corrections,
  display settings, controller/keyboard input, and SteamVR integration.
- Separate player profiles and saves, compatible community-server online play,
  and a Windows multi-window split-screen launcher with controller assignment.
- Full ZD menus, debug text, graphs, physics/skeleton visualization and other
  diagnostic views, with a saved **System → ZD debugging features** checkbox.

Open Settings with **Escape/F1** or **RB + Start**. To launch split screen, run
`sk83_na_m.exe --splitscreen`, assign connected controllers to screens, then select
different profiles and host/join a compatible online session.

## Guides

- [Features, controls, online play, split screen, VR and troubleshooting](docs/FEATURES.md)
- [Build from source](docs/BUILDING.md)
- [All project configuration options and defaults](docs/CONFIGURATION.md)
- [What is included/excluded in this clean source snapshot](docs/SOURCE_CONTENTS.md)
- [Credits and dependency notices](THIRD_PARTY_NOTICES.md)
- [Detailed ZD debug-feature reference](src/zd_debug/README.md)

The generated main-game implementation, original game executables/data,
decompiler dumps, user data, backups, build caches and unused investigation tools
are excluded. The enhanced embedded font is retained. See the source-contents
guide for the intentional ZD debug-source inclusion in the milestone project.
