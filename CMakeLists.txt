# sk83_na_m - ReXGlue Recompiled Project
#
# This file is yours to edit. 'rexglue migrate' will NOT overwrite it.
# SDK integration lives in cmake/ReXGlue.cmake (independent of codegen).

cmake_minimum_required(VERSION 3.25)
project(sk83_na_m VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force default build type to Release if non-debug build type is omitted
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

include(cmake/ReXGlue.cmake)

# EAWebkit.xex guest module (declared by generated/dll_targets.cmake, which
# rexglue.cmake includes above). The SDK's module helper leaves the target on
# the bare CMAKE_CXX_FLAGS, and CMAKE_CXX_FLAGS_RELEASE is empty in a rexglue
# tree -- so without this the 16k recompiled PPC functions build at -O0.
# Deliberately NOT -flto=thin: the module is loaded through LoadLibrary at
# runtime, nothing cross-module inlines into it, and thin-LTO over that many
# translation units costs minutes of link time for nothing.
if(TARGET sk83_na_m_EAWebkit)
    # codegen emits <project>_register.cpp -- the exported ReXModule_Register
    # that hands the guest->host function mappings to the loader -- but its
    # sources.cmake template never lists it (see resources/templates/codegen/
    # sources_cmake.inja: init.cpp, module_registry.cpp and the recomp files
    # only). Without it the DLL exports nothing, and LoadUserModule fails with
    # "ReXModule_Register not found in 'sk83_na_m_EAWebkit'" -- the module
    # loads but no function is ever registered.
    target_sources(sk83_na_m_EAWebkit PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/generated/eawebkit/sk83_na_m_register.cpp)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(sk83_na_m_EAWebkit PRIVATE
            -march=x86-64-v3
            -fomit-frame-pointer
            $<$<CONFIG:Release>:-O3 -DNDEBUG>
            $<$<CONFIG:RelWithDebInfo>:-O3 -DNDEBUG -g -Xclang -gcodeview>
        )
    elseif(MSVC)
        target_compile_options(sk83_na_m_EAWebkit PRIVATE
            /arch:AVX2 /Oy
            $<$<CONFIG:Release>:/O2 /Ob2 /Oi /Ot /DNDEBUG>
            $<$<CONFIG:RelWithDebInfo>:/O2 /Ob2 /Oi /Ot /DNDEBUG /Zi>
        )
    endif()
endif()

# Sources
set(SK83NAM_SOURCES
    src/main.cpp
    src/skate3_fullscreen_toggle.cpp
    src/skate3_guest_aspect.cpp
    src/skate3_apt_spread.cpp
    src/skate3_online.cpp
    src/skate3_controller_port.cpp
    src/skate3_deck_shadow.cpp
    src/skate3_frustum_cull.cpp
    src/skate3_anim_twoconsec.cpp
    src/skate3_safe_callstack.cpp
    src/skate3_localizer_formatters.cpp
    # Diagnostic-only hook overrides: left out of Release (the recompiled
    # functions they wrap are weak, so the game's own bodies link instead).
    $<$<NOT:$<CONFIG:Release>>:src/skate3_navmesh_diag.cpp>
    $<$<NOT:$<CONFIG:Release>>:src/skate3_shadow_gate_diag.cpp>
    $<$<NOT:$<CONFIG:Release>>:src/skate3_trickguide_diag.cpp>
    src/skate3_cas_camera_fixes.cpp
    src/skate3_scene_rig_fix.cpp
    src/skate3_pc_renderer.cpp
    src/skate3_shader_translation.cpp
    # --- native (D3D12/Vulkan) renderer, ported from skate3recomp -----------
    # src/skate3_demo_path.cpp     # TODO(sk83_na_m): 4 retail addresses, none
                                    # resolved yet (0x82639400, 0x826FDD70,
                                    # 0x826FE1D8, 0x82D0AFA0) -- won't link.
    # src/skate3_draw_distance.cpp # TODO(sk83_na_m): 9 retail addresses, none
                                    # resolved yet (0x8247AA88, 0x8247AC48,
                                    # 0x8247B3A0, 0x8247BB50, 0x8247BD28,
                                    # 0x8247DCF0, 0x82792900, 0x827E1AD8,
                                    # 0x8288DC58) -- won't link.
    src/native/skate3_native_diagnostics.cpp
    src/native/skate3_native_entity.cpp
    src/native/skate3_native_guest_read.cpp
    src/native/skate3_native_lw.cpp
    src/native/skate3_native_palette.cpp
    src/skate3_guest_console.cpp
    src/skate3_native_debug_dialog.cpp
    src/skate3_native_render.cpp
    src/generated/skate3_native_font_data.cpp
    src/skate3_native_font_atlas.cpp
    src/skate3_native_scene.cpp
    src/skate3_native_scene_gpu.cpp
    src/skate3_native_scene_post.cpp
    src/skate3_screenshot.cpp
    src/skate3_shader_disasm.cpp
    src/skate3_draw_distance_cvars.cpp
    src/skate3_render_resolution.cpp
    src/skate3_settings_menu.cpp
    src/skate3_profiles.cpp
    src/skate3_splitscreen.cpp
    src/skate3_vr.cpp
    src/skate3_window_icon.cpp
    src/skate3_job_watch.cpp
    # exception_compat.cpp intentionally NOT included yet -- see
    # NATIVE_RENDERER_PORT_NOTES.md, it hooks by compile-time symbol name
    # (sub_82F44E40) which hasn't been resolved for this build.
)

if(WIN32)
    add_executable(sk83_na_m WIN32 ${SK83NAM_SOURCES})
else()
    add_executable(sk83_na_m ${SK83NAM_SOURCES})
endif()

# mimalloc as the process-wide allocator (Windows, dynamic CRT): mimalloc.dll
# built with MI_OVERRIDE plus the bundled mimalloc-redirect.dll patch the
# CRT's malloc/free for every module (exe, rexruntime.dll, drivers' CRT
# use), so no cross-module new/delete mismatch is possible. The renderer's
# per-frame containers and item copies make heap calls a measurable share
# of the guest render thread. Linked FIRST so mimalloc.dll heads the import
# table (mimalloc's load-order requirement); /INCLUDE keeps the otherwise
# unreferenced import. Build the DLL once with:
#   cmake -S _ext/mimalloc -B _ext/mimalloc/out/dll-release -G Ninja
#     -DCMAKE_BUILD_TYPE=Release -DMI_BUILD_SHARED=ON -DMI_BUILD_STATIC=OFF
#     -DMI_BUILD_OBJECT=OFF -DMI_BUILD_TESTS=OFF -DMI_OVERRIDE=ON
set(SK83_MIMALLOC_DIR ""
    CACHE PATH "Directory holding mimalloc.lib/.dll and mimalloc-redirect.dll")
option(SK83_MIMALLOC "Use mimalloc (process-wide malloc redirect) on Windows" ON)
if(WIN32 AND SK83_MIMALLOC
   AND EXISTS "${SK83_MIMALLOC_DIR}/mimalloc.lib")
    target_link_libraries(sk83_na_m PRIVATE "${SK83_MIMALLOC_DIR}/mimalloc.lib")
    target_link_options(sk83_na_m PRIVATE "LINKER:/INCLUDE:mi_version")
    add_custom_command(TARGET sk83_na_m POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SK83_MIMALLOC_DIR}/mimalloc.dll"
            "${SK83_MIMALLOC_DIR}/mimalloc-redirect.dll"
            "$<TARGET_FILE_DIR:sk83_na_m>"
        VERBATIM)
    message(STATUS "sk83_na_m: mimalloc from ${SK83_MIMALLOC_DIR}")
elseif(WIN32 AND SK83_MIMALLOC)
    message(STATUS "sk83_na_m: mimalloc not found in ${SK83_MIMALLOC_DIR}; using the CRT heap")
endif()

rexglue_setup_target(sk83_na_m)

# Debug features ported from the sk83_na_zd build (src/zd_debug/README.md).
# Self-contained: delete src/zd_debug/ and this block to remove them, or set
# -DSK83_ZD_DEBUG=OFF. At runtime the `zd_debug` cvar switches them all off.
option(SK83_ZD_DEBUG "Build the debug features ported from sk83_na_zd" ON)
if(SK83_ZD_DEBUG AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/zd_debug)
    file(GLOB SK83_ZD_DEBUG_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/zd_debug/*.cpp)
    target_sources(sk83_na_m PRIVATE ${SK83_ZD_DEBUG_SOURCES})
    target_compile_definitions(sk83_na_m PRIVATE SK83_ZD_DEBUG=1)
endif()

# Release strips the investigation probes (per-draw pass recording, the
# motion-blur stats, logging-only hooks); see SK83_RELEASE_BUILD in src/.
target_compile_definitions(sk83_na_m PRIVATE $<$<CONFIG:Release>:SK83_RELEASE_BUILD>)

# Window title "[v<version>-<config>]", the same form skate3recomp uses
# (without it the SDK default shows its own rexglue git version). The exe's
# VERSIONINFO in app.rc carries the same number; keep the two in step.
target_compile_definitions(sk83_na_m PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:SK83_VERSION_STRING="${PROJECT_VERSION}">
    $<$<COMPILE_LANGUAGE:CXX>:SK83_BUILD_CONFIG="$<CONFIG>">)

# The recompiler lowers the guest's multiply-add instructions as unfused
# add(mul(a,b), c) -- two roundings. Letting the compiler contract our native
# reimplementations into FMA (one rounding) puts them a few ulp off the guest,
# which breaks the differential harnesses. Match the guest.
set_source_files_properties(
    src/skate3_anim_twoconsec.cpp
    PROPERTIES COMPILE_OPTIONS "-ffp-contract=off")

# icon setup
if(WIN32)
  enable_language(RC)   # skip if already enabled
  target_sources(sk83_na_m PRIVATE app.rc)
endif()



# ---------------------------------------------------------------------------
# MAXIMUM OPTIMIZATION SETTINGS
# ---------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(sk83_na_m PRIVATE
        -march=x86-64-v3
        -fomit-frame-pointer
        -mprefer-vector-width=256
        $<$<CONFIG:Release>:-O3 -DNDEBUG -funroll-loops -mllvm -inline-threshold=1000>
        $<$<CONFIG:RelWithDebInfo>:-O3 -DNDEBUG -funroll-loops -g -Xclang -gcodeview>
    )
elseif(MSVC)
    target_compile_options(sk83_na_m PRIVATE
        /arch:AVX2
        /Oy
        /Qvec
        /Gw
        $<$<CONFIG:Release>:/O2 /Ob2 /Oi /Ot /DNDEBUG>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Ob2 /Oi /Ot /DNDEBUG /Zi>
    )
endif()

# Enable Link-Time Optimization (IPO/LTO) across all split recompiled units
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(ipo_supported)
    set_property(TARGET sk83_na_m PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set_property(TARGET sk83_na_m PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
endif()
# ---------------------------------------------------------------------------

# --- Native renderer: embed HLSL sources into a generated header ----------
set(SKATE3_NATIVE_SHADER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/scene.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/shadow_blur.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/resolve.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/blur.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/outline.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/overlay2d.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/spline.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/photo_fx.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/ssao.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/hbao.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/hdr.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/ssr.hlsl
)
file(GLOB SKATE3_NATIVE_SHADER_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/*.hlsli)
set(SKATE3_NATIVE_SHADER_HEADER
    ${CMAKE_CURRENT_BINARY_DIR}/include/skate3_native_shaders.h)
add_custom_command(
    OUTPUT ${SKATE3_NATIVE_SHADER_HEADER}
    COMMAND ${CMAKE_COMMAND}
        -DSHADER_DIR=${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders
        -DOUT=${SKATE3_NATIVE_SHADER_HEADER}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedShaders.cmake
    DEPENDS ${SKATE3_NATIVE_SHADER_SOURCES}
        ${SKATE3_NATIVE_SHADER_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedShaders.cmake
    COMMENT "Embedding native renderer shaders"
    VERBATIM)
target_sources(sk83_na_m PRIVATE ${SKATE3_NATIVE_SHADER_HEADER})

# Vulkan SPIR-V of the native renderer shaders (tools/gen_native_spirv.py).
# The D3D12 backend compiles the HLSL at runtime; the Vulkan backend runs the
# offline-compiled SPIR-V in src/native/shaders/spirv. With Python and a
# SPIR-V-capable DXC (the Windows SDK's dxc.exe lacks SPIR-V codegen; use the
# GitHub release) the build regenerates it whenever a shader, a shared
# include or the generator changes; without them the checked-in header is
# used as is. `regen_native_spirv` forces a rebuild, e.g. after the renderer
# starts using a new shader entry point.
set(SK83_DXC ""
    CACHE FILEPATH "SPIR-V-capable dxc.exe for the native renderer's Vulkan shaders")
set(SK83_NATIVE_SPIRV_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/src/native/shaders/spirv/skate3_native_shaders_spirv.h)
find_package(Python3 COMPONENTS Interpreter)
if(EXISTS "${SK83_DXC}" AND Python3_Interpreter_FOUND)
    set(SK83_NATIVE_SPIRV_COMMAND
        ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_native_spirv.py
        --src ${CMAKE_CURRENT_SOURCE_DIR}/src --dxc ${SK83_DXC})
    add_custom_command(
        OUTPUT ${SK83_NATIVE_SPIRV_HEADER}
        COMMAND ${SK83_NATIVE_SPIRV_COMMAND}
        DEPENDS ${SKATE3_NATIVE_SHADER_SOURCES}
            ${SKATE3_NATIVE_SHADER_INCLUDES}
            ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_native_spirv.py
        COMMENT "Compiling native renderer shaders to SPIR-V (Vulkan)"
        VERBATIM)
    add_custom_target(regen_native_spirv
        COMMAND ${SK83_NATIVE_SPIRV_COMMAND}
        COMMENT "Recompiling all native renderer shaders to SPIR-V (Vulkan)"
        VERBATIM)
    target_sources(sk83_na_m PRIVATE ${SK83_NATIVE_SPIRV_HEADER})
    message(STATUS "sk83_na_m: Vulkan SPIR-V regenerated from HLSL with ${SK83_DXC}")
else()
    message(STATUS "sk83_na_m: no SPIR-V-capable DXC at ${SK83_DXC}; using the checked-in "
                   "Vulkan SPIR-V (stale once the HLSL changes)")
endif()
target_include_directories(sk83_na_m PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)

target_link_libraries(sk83_na_m PRIVATE d3d11 dxgi dxguid gdiplus)

# SteamVR output (src/skate3_vr.cpp): the OpenVR C API header, and the
# runtime DLL staged next to the exe (loaded on demand; without it, or
# without SteamVR, the game simply runs flat).
target_include_directories(sk83_na_m PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/openvr/openvr_api.dll")
    add_custom_command(TARGET sk83_na_m POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/openvr/openvr_api.dll"
            "$<TARGET_FILE_DIR:sk83_na_m>/openvr_api.dll"
        VERBATIM)
endif()

# Profile-guided optimisation (clang IR PGO), as in skate3recomp: see the
# notes there. GEN = instrumented build dumping SK83_PGO_PROFILE_DIR/
# sk83-<pid>.profraw every few seconds; USE = optimise with SK83_PGO_PROFILE;
# AUTO = USE when the profile exists; OFF = never. Kept last so the
# profile dependency covers every source added above.
set(SK83_PGO "AUTO" CACHE STRING "Profile-guided optimisation: AUTO, GEN, USE or OFF")
set_property(CACHE SK83_PGO PROPERTY STRINGS AUTO GEN USE OFF)
set(SK83_PGO_PROFILE "${CMAKE_CURRENT_SOURCE_DIR}/pgo/sk83_na_m.profdata"
    CACHE FILEPATH "Merged PGO profile used by SK83_PGO=USE/AUTO")
set(SK83_PGO_PROFILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/pgo-raw"
    CACHE PATH "Where a SK83_PGO=GEN build writes its .profraw files")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(SK83_PGO STREQUAL "GEN")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-resource-dir
                        OUTPUT_VARIABLE SK83_CLANG_RESOURCE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
        find_file(SK83_CLANG_PROFILE_RT NAMES clang_rt.profile-x86_64.lib clang_rt.profile.lib
                  PATHS "${SK83_CLANG_RESOURCE_DIR}/lib/windows"
                        "${SK83_CLANG_RESOURCE_DIR}/lib/x86_64-pc-windows-msvc"
                  NO_DEFAULT_PATH)
        if(NOT SK83_CLANG_PROFILE_RT)
            message(FATAL_ERROR "SK83_PGO=GEN: clang profile runtime not found under ${SK83_CLANG_RESOURCE_DIR}")
        endif()
        file(MAKE_DIRECTORY "${SK83_PGO_PROFILE_DIR}")
        # Instrument the PLAYED config (RelWithDebInfo: the debug front end only
        # auto-advances there) but without debug info: an instrumented PDB and
        # debug bitcode would not fit next to the normal builds. -g0 comes after
        # the config's -g, so it wins.
        target_compile_options(sk83_na_m PRIVATE -fprofile-generate $<$<CONFIG:RelWithDebInfo>:-g0>)
        target_sources(sk83_na_m PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/skate3_pgo.cpp)
        target_compile_definitions(sk83_na_m PRIVATE
            SKATE3_PGO_GEN "SKATE3_PGO_RAW_DIR=\"${SK83_PGO_PROFILE_DIR}\""
            "SKATE3_PGO_RAW_PREFIX=\"sk83\"")
        # The link runs with -nostdlib, so the driver does not add the runtime.
        target_link_libraries(sk83_na_m PRIVATE "${SK83_CLANG_PROFILE_RT}")
        target_link_options(sk83_na_m PRIVATE "LINKER:/INCLUDE:__llvm_profile_runtime")
        message(STATUS "sk83_na_m: PGO instrumented build (raw profiles -> ${SK83_PGO_PROFILE_DIR})")
    elseif(SK83_PGO STREQUAL "USE" OR (SK83_PGO STREQUAL "AUTO" AND EXISTS "${SK83_PGO_PROFILE}"))
        if(NOT EXISTS "${SK83_PGO_PROFILE}")
            message(FATAL_ERROR "SK83_PGO=USE: ${SK83_PGO_PROFILE} does not exist")
        endif()
        target_compile_options(sk83_na_m PRIVATE
            "-fprofile-use=${SK83_PGO_PROFILE}"
            -Wno-profile-instr-unprofiled -Wno-profile-instr-out-of-date -Wno-profile-instr-missing
            -Wno-backend-plugin)
        # Rebuild everything when the profile changes.
        get_target_property(SK83_ALL_SOURCES sk83_na_m SOURCES)
        set_property(SOURCE ${SK83_ALL_SOURCES} APPEND PROPERTY OBJECT_DEPENDS "${SK83_PGO_PROFILE}")
        message(STATUS "sk83_na_m: PGO using ${SK83_PGO_PROFILE}")
    endif()
endif()
