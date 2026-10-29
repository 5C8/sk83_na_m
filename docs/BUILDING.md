# Building the milestone project

This target is for the **sk83_na_m** milestone executable, not the retail disc.
It cannot be built or run by renaming a retail `default.xex`.

Use the [matching SDK release](https://github.com/5C8/rexglue-skate3/releases),
Clang 23.1.0 (the compiler used by the initial Windows snapshot), CMake 3.25+,
Ninja, Python 3, Visual Studio C++ build tools and a Windows SDK. Run in a
developer shell with `clang` and `clang++` on PATH.

For code generation, put your own matching executables at:

```text
assets/sk83_na_m.xex
assets/EAWebkit.xex
```

The manifest describes the game's EAWebkit guest-module path. No executable or
main-game generated source is included in this repository.

```powershell
cmake --preset win-amd64-release -DCMAKE_PREFIX_PATH=C:/Dev/rex-sdk
cmake --build --preset win-amd64-release --target sk83_na_m_codegen --parallel 4
cmake --preset win-amd64-release -DCMAKE_PREFIX_PATH=C:/Dev/rex-sdk
cmake --build --preset win-amd64-release --parallel 4
```

The SDK integration is committed as `cmake/ReXGlue.cmake`, so initial configure
does not depend on an already-populated `generated/` folder. Reconfigure after
generation. Output is in `out/build/win-amd64-release/`.

ZD is compiled by default (`SK83_ZD_DEBUG=ON`). Its complete debug implementation
and initialization snapshot are included under `src/zd_debug`; generating the
main game from your own milestone XEX is still required. Use
`-DSK83_ZD_DEBUG=OFF` to build without those features. The in-game master checkbox
exists only when ZD is compiled in. Historical regeneration commands in the
detailed ZD notes refer to research tools outside this source snapshot; the
checked-in C++ needs none of those tools to compile.

PGO defaults to AUTO and uses no private profile when none exists. You can pass
`-DSK83_PGO=OFF`. Optional mimalloc uses `SK83_MIMALLOC_DIR`; without it, the CRT
heap is used. Shader SPIR-V is checked in; set `SK83_DXC` to a SPIR-V-capable DXC
to regenerate after HLSL changes. Run `python tools/fetch_openvr.py` for SteamVR,
or copy `openvr_api.dll` from this release into `thirdparty/openvr/`.

Windows x64 is the supported packaged target. Other presets are inherited and
are not validated for this milestone integration; several files call Windows
APIs directly. Do not assume that a Linux/ARM preset implies a working port.
