
// sk83_na_m - ReXGlue Recompiled Project
//
// This file is yours to edit. 'rexglue migrate' will NOT overwrite it.
// Customize your app by overriding virtual hooks from rex::ReXApp.

#pragma once

#include <cstdlib>
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <optional>
#include <memory>

#include <imgui.h>
#include <rex/rex_app.h>
#include <rex/graphics/native_guest_renderer.h>
#include <rex/ui/window.h>

#include "sk83_na_m_init.h"

#include "skate3_pc_renderer.h"
#include "skate3_shader_translation.h"
#include "skate3_fullscreen_toggle.h"
#include "skate3_guest_aspect.h"
#include "skate3_render_resolution.h"
#include "skate3_settings_menu.h"
#include "skate3_profiles.h"
#include "skate3_splitscreen.h"
#include "skate3_vr.h"
#include "skate3_window_icon.h"
#include <rex/input/input_system.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xam/user_profile.h>
#include "skate3_native_render.h"
#include "skate3_native_scene.h"  // ToggleSceneEnabled (F5)
#include "skate3_native_debug_dialog.h"
#include "skate3_guest_console.h"
#include "skate3_screenshot.h"

#include <rex/ui/keybinds.h>
#include "skate3_shader_disasm.h"
#include <stdlib.h>
void Sk83InstallSoftAsserts();
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <rex/filesystem/devices/host_path_device.h>
#include <rex/ui/overlay/settings_overlay.h>
#ifdef SK83_ZD_DEBUG
#include "zd_debug/overlay.h"  // sk83_na_zd debug features (removable)
#endif
// demo_path.h intentionally not included yet -- skate3_demo_path.cpp is
// still excluded from the build (4 unresolved retail addresses), so
// skate3::demo_path::InstallHooks() isn't available to call.

class Sk83NaMApp : public rex::ReXApp {
 public:
  using rex::ReXApp::ReXApp;

  static std::unique_ptr<rex::ui::WindowedApp> Create(
      rex::ui::WindowedAppContext& ctx) {
    return std::unique_ptr<Sk83NaMApp>(new Sk83NaMApp(ctx, "sk83_na_m",
        sk83_na_m_PPCImageConfig));
  }

  // "sk83_na_m [v1.0.0-Release]", matching skate3recomp's title form; the
  // SDK default would show the rexglue SDK's git version instead.
  std::string_view GetBuildTitle() const override {
    return "[v" SK83_VERSION_STRING "-" SK83_BUILD_CONFIG "]";
  }

  // Default game_data_root to "<cwd>/game" when the user hasn't set it
  // via --game_data_root or the config TOML. Users can override the
  // default by launching from a different directory or passing the
  // flag; a non-empty value from the CLI/config wins.
  void OnConfigurePaths(rex::PathConfig& paths) override {
    if (paths.game_data_root.empty()) {
      paths.game_data_root = std::filesystem::current_path() / "game";
    }
  }

  // UI font: the same as skate3recomp (Skate3BaseApp::OnConfigureFonts) --
  // Helvetica, else Arial, at 16 px, with MS Gothic merged in for Japanese --
  // so the profile picker, settings menu and overlays look the same in both
  // builds (without an override the SDK's 12 px default font is used).
  void OnConfigureFonts(ImFontAtlas* atlas) override {
    if (!atlas) {
      return;
    }
    atlas->Clear();
    ImFontConfig font_config;
    font_config.OversampleH = 2;
    font_config.OversampleV = 2;
    font_config.PixelSnapH = false;
    const char* base_font_path = nullptr;
    for (const char* path : {"C:\\Windows\\Fonts\\Helvetica.ttf", "C:\\Windows\\Fonts\\helvetica.ttf",
                             "C:\\Windows\\Fonts\\HelveticaNeue.ttf",
                             "C:\\Windows\\Fonts\\arial.ttf"}) {
      if (std::filesystem::exists(path)) {
        base_font_path = path;
        break;
      }
    }
    if (base_font_path) {
      atlas->AddFontFromFileTTF(base_font_path, 16.0f, &font_config,
                                atlas->GetGlyphRangesDefault());
    } else {
      atlas->AddFontDefault();
    }
    const char* jp_font_path = "C:\\Windows\\Fonts\\msgothic.ttc";
    if (std::filesystem::exists(jp_font_path)) {
      ImFontConfig jp_font_config;
      jp_font_config.MergeMode = true;
      jp_font_config.OversampleH = jp_font_config.OversampleV = 1;
      jp_font_config.PixelSnapH = true;
      jp_font_config.FontNo = 0;
      atlas->AddFontFromFileTTF(jp_font_path, 16.0f, &jp_font_config,
                                atlas->GetGlyphRangesJapanese());
    }
  }

  // Profile sign-in before the runtime exists (skate3_profiles): the last
  // profile signs in automatically, or the profile picker opens (auto
  // sign-in off / nothing to sign in). The kernel's user profile reads the
  // chosen identity from the profile cvars when the runtime is constructed.
  std::optional<rex::PathConfig> OnFinalizePaths(
      const rex::PathConfig& defaults, std::function<void(rex::PathConfig)> resume) override {
    (void)resume;
    // Caption/taskbar icon from the embedded MAINICON, before the profile
    // picker opens (see skate3_window_icon.h).
    skate3::ApplyEmbeddedWindowIcon(window() ? window()->GetNativeWindowHandle() : nullptr);
    profiles_path_ = skate3::profiles::StorePath(defaults.user_data_root);
    // --splitscreen (skate3_splitscreen): this process only shows the seat
    // screen and starts one game window per player.
    if (skate3::splitscreen::IsLauncher()) {
      skate3::splitscreen::RunLauncher(app_context(), window(), imgui_drawer());
      app_context().QuitFromUIThread();
      return std::nullopt;
    }
    if (const int player = skate3::splitscreen::PlayerNumber(); player > 0 && window()) {
      window()->SetTitle(window()->GetTitle() + " - Player " + std::to_string(player));
      skate3::splitscreen::KeepAboveTaskbar(window()->GetNativeWindowHandle());
    }
    if (!skate3::profiles::SignInAtBoot(app_context(), window(), imgui_drawer(),
                                        profiles_path_)) {
      app_context().QuitFromUIThread();
      return std::nullopt;
    }
    return defaults;
  }

  // Runtime is fully initialized here (kernel + memory are ready).
  //   * Skate3InitFullscreenToggle: subclasses the window so a double
  //     click on the client area (or Alt+Enter) toggles borderless
  //     fullscreen on the current monitor.
  //
  // The OLD PC renderer (Skate3InitPcRenderer / Skate3RegisterPcRendererHooks)
  // is DISABLED. Per skate3_pc_renderer.h it stands up its own D3D11 device
  // and swapchain and translates every guest D3DDevice_* call itself, fully
  // bypassing the SDK's GPU/CommandProcessor pipeline -- and its own header
  // says the app must then keep the SDK's GPU backend from spinning up at
  // all. skate3::native_render::Install() needs exactly that backend running,
  // so with both enabled the process carried TWO complete GPU stacks over the
  // same guest draw stream. skate3recomp never calls these at all.
  //
  // They were kept alive only while the native path could not yet produce a
  // correct frame. It now does (world, characters, shadows, 2D/HUD and the
  // debug overlay all render), so the old path is pure cost.
  //
  // Re-enable only to A/B against the native renderer, and expect the SDK
  // graphics config to need changing to match.
  static bool IsBigPathChar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '.' ||
           c == '/' || c == '\\' || c == ' ';
  }

  // Collect the directory part of every "data/..." or "data\..." path string
  // embedded in the .big archives under the game data root (EB archives name
  // folders with '/'; BIGF archives such as marquee.big carry full backslash
  // paths, e.g. data\content\recipe\marquee\*.recipe, which the marquee
  // viewer lists). Follows directory junctions.
  static std::set<std::string> DiscoverBigDirectories(
      const std::filesystem::path& data_root) {
    static constexpr size_t kScanBytes = 8 * 1024 * 1024;
    std::set<std::string> dirs;
    std::error_code ec;
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(
             data_root, std::filesystem::directory_options::follow_directory_symlink, ec)) {
      if (ec) {
        break;
      }
      std::error_code fec;
      if (!entry.is_regular_file(fec) || fec) {
        continue;
      }
      if (entry.path().extension() != ".big") {
        continue;
      }
      std::ifstream stream(entry.path(), std::ios::binary);
      if (!stream) {
        continue;
      }
      std::string buffer;
      buffer.resize(kScanBytes);
      stream.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
      buffer.resize(static_cast<size_t>(stream.gcount()));
      for (std::string_view prefix : {std::string_view("data/"), std::string_view("data\\")}) {
        size_t pos = 0;
        while ((pos = buffer.find(prefix, pos)) != std::string::npos) {
          size_t end = pos;
          while (end < buffer.size() && IsBigPathChar(buffer[end])) {
            ++end;
          }
          std::string path = buffer.substr(pos, end - pos);
          pos = end > pos ? end : pos + 1;
          std::replace(path.begin(), path.end(), '\\', '/');
          const size_t slash = path.find_last_of('/');
          if (slash == std::string::npos || slash <= prefix.size()) {
            continue;
          }
          path.resize(slash);
          if (path.size() > 250 || path.find("..") != std::string::npos) {
            continue;
          }
          dirs.insert(path);
        }
      }
    }
    return dirs;
  }

  // EAWebKit content paths.
  //
  // The browser asks for its fonts at d:\data\webkit\<name>.ttf, but the only
  // copies in the dump sit under data\big\data\webkit -- and the BIG overlay
  // above deliberately skips data\webkit, because that directory DOES exist on
  // disc (it holds module\sk8-xbox2-na-milestone\EAWebkit.xex). So every font
  // open failed with 0xc000000f. With no font face loaded EAWebKit lays out
  // and paints nothing, which is the blank white page.
  //
  // It also wants a writable cache (webcache\cookies.tmp, webcache\FileCache
  // .ini), asked for BOTH as d:\webcache\ and as a bare "webcache\" carrying
  // no device prefix at all -- the latter logged "[no device]" every time.
  //
  // Fonts are linked per FILE rather than by redirecting the directory, so the
  // real data\webkit\module subtree holding the xex is never shadowed.
  void InstallWebkitOverlay() {
    auto* fs = runtime() ? runtime()->file_system() : nullptr;
    if (fs == nullptr) {
      return;
    }
    const auto game_root = runtime()->game_data_root();
    std::error_code ec;

    const auto fonts = game_root / "data" / "big" / "data" / "webkit";
    if (std::filesystem::exists(fonts, ec)) {
      auto dev = std::make_unique<rex::filesystem::HostPathDevice>(
          "sk83webkit:", fonts, true);
      if (dev->Initialize()) {
        fs->RegisterDevice(std::move(dev));
        size_t n = 0;
        for (const auto& e : std::filesystem::directory_iterator(fonts, ec)) {
          if (ec || !e.is_regular_file()) {
            continue;
          }
          const std::string name = e.path().filename().string();
          fs->RegisterSymbolicLink(
              "\\Device\\Harddisk0\\Partition1\\data\\webkit\\" + name,
              "sk83webkit:\\" + name);
          ++n;
        }
        REXLOG_INFO("webkit overlay: {} font(s) linked from {}", n, fonts.string());
      } else {
        REXLOG_WARN("webkit overlay: font device init failed for {}", fonts.string());
      }
    } else {
      REXLOG_WARN("webkit overlay: no fonts under {}", fonts.string());
    }

    const auto cache = cache_root() / "webcache";
    std::filesystem::create_directories(cache, ec);
    if (ec) {
      REXLOG_WARN("webkit overlay: cannot create {}: {}", cache.string(), ec.message());
      return;
    }
    auto cdev = std::make_unique<rex::filesystem::HostPathDevice>(
        "sk83webcache:", cache, false);
    if (!cdev->Initialize()) {
      REXLOG_WARN("webkit overlay: cache device init failed for {}", cache.string());
      return;
    }
    fs->RegisterDevice(std::move(cdev));
    // Both spellings the module was observed using. Symlink matching is a
    // case-insensitive PREFIX match, so these cover the subpaths too.
    fs->RegisterSymbolicLink("\\Device\\Harddisk0\\Partition1\\webcache",
                             "sk83webcache:");
    fs->RegisterSymbolicLink("webcache", "sk83webcache:");
    REXLOG_INFO("webkit overlay: webcache at {}", cache.string());
  }

  void InstallBigDirectoryOverlay() {
    auto* fs = runtime() ? runtime()->file_system() : nullptr;
    if (fs == nullptr) {
      return;
    }
    const auto game_root = runtime()->game_data_root();
    const auto data_root = game_root / "data";
    std::error_code ec;
    if (!std::filesystem::exists(data_root, ec)) {
      REXLOG_WARN("BIG-directory overlay: data root not found: {}", data_root.string());
      return;
    }
    const auto dirs = DiscoverBigDirectories(data_root);
    if (dirs.empty()) {
      REXLOG_WARN("BIG-directory overlay: no paths discovered under {}", data_root.string());
      return;
    }
    const auto overlay_root = cache_root() / "vfs_big_directory_aliases";
    std::filesystem::create_directories(overlay_root, ec);
    if (ec) {
      REXLOG_WARN("BIG-directory overlay: cannot create {}: {}", overlay_root.string(),
                  ec.message());
      return;
    }
    size_t created = 0;
    for (const auto& dir : dirs) {
      std::filesystem::path path = overlay_root;
      size_t start = 0;
      while (start < dir.size()) {
        const size_t slash = dir.find('/', start);
        const size_t end = slash == std::string::npos ? dir.size() : slash;
        if (end > start) {
          path /= dir.substr(start, end - start);
        }
        if (slash == std::string::npos) {
          break;
        }
        start = slash + 1;
      }
      std::error_code dec;
      std::filesystem::create_directories(path, dec);
      if (!dec) {
        ++created;
      }
    }
    auto device = std::make_unique<rex::filesystem::HostPathDevice>(
        "sk83bigdirs:", overlay_root, true);
    if (!device->Initialize()) {
      REXLOG_WARN("BIG-directory overlay: host device init failed for {}",
                  overlay_root.string());
      return;
    }
    fs->RegisterDevice(std::move(device));
    // Only link subtrees absent from the real disc layout, so the overlay can
    // never shadow content that is actually present (data\content\world and
    // data\audio exist on disk and are deliberately left alone).
    // Link every DISCOVERED directory that is absent from the real disc
    // layout, not just whole top-level roots. datae exists on disk but
    // dataeonts does not, so the hardcoded-root version left the real
    // font assets unresolved (futuraheavy.bmpFont / .rx2 both failed to
    // open) -- the game then falls back to a cruder built-in font, and the
    // same gap crashed RenderStats earlier. Deepest-first, skipping any
    // directory already covered by a linked ancestor, so a link can never
    // shadow content that is present.
    std::vector<std::string> want;
    for (const auto& dir : dirs) {
      std::string guest = dir;  // "data/x/y"
      for (char& c : guest) {
        if (c == '/') c = '\\';
      }
      if (guest.rfind("data\\", 0) != 0 || guest.size() <= 5) {
        continue;
      }
      if (std::filesystem::exists(game_root / guest)) {
        continue;  // real content wins
      }
      want.push_back(guest);
    }
    std::sort(want.begin(), want.end());
    size_t linked = 0;
    std::vector<std::string> linked_roots;
    for (const std::string& guest : want) {
      bool covered = false;
      for (const std::string& r : linked_roots) {
        if (guest.size() > r.size() && guest.compare(0, r.size(), r) == 0 &&
            guest[r.size()] == '\\') {
          covered = true;
          break;
        }
      }
      if (covered) {
        continue;
      }
      fs->RegisterSymbolicLink("\\Device\\Harddisk0\\Partition1\\" + guest,
                               "sk83bigdirs:\\" + guest);
      linked_roots.push_back(guest);
      ++linked;
    }
    REXLOG_INFO("BIG-directory overlay: {} dirs discovered, {} created, {} roots linked ({})",
                dirs.size(), created, linked, overlay_root.string());
  }

  void OnPostSetup() override {
    // Native renderer output = the window's physical client size at ANY
    // aspect, so native frames map 1:1 onto screen pixels and the window is
    // freely resizable (the renderer extends the view Hor+/Vert+ and keeps
    // the 2D stream in a centered 16:9 band). Only consulted while native
    // frames are being served (see the SDK's ApplyNativeGuestOutputSize);
    // emulated frames keep the frontbuffer path and stay 16:9, letterboxed
    // by the presenter (present_letterbox).
    rex::graphics::SetNativeGuestOutputSizeProvider(
        [](uint32_t& width, uint32_t& height, void* user_data) -> bool {
          const auto* app = static_cast<Sk83NaMApp*>(user_data);
          const rex::ui::Window* win = app != nullptr ? app->window() : nullptr;
          if (win == nullptr) {
            return false;
          }
          // Window pixels unless a resolution/aspect is forced
          // (skate3_render_width/height/aspect); a forced size is scaled
          // to fit and letterboxed by the presenter.
          // VR: every native frame is a headset eye image.
          if (skate3::vr::OutputSize(width, height)) {
            return true;
          }
          return skate3::render_resolution::ComputeOutputSize(
              win->GetActualPhysicalWidth(), win->GetActualPhysicalHeight(),
              width, height);
        },
        this);
    // SteamVR output + VR-controller pad (idle unless skate3_vr is on).
    skate3::vr::Install(
        runtime() ? static_cast<rex::input::InputSystem*>(runtime()->input_system()) : nullptr);
    skate3::vr::SetMenuHooks(
        [this]() { return settings_menu_ && settings_menu_->visible(); },
        [this]() { app_context().CallInUIThreadDeferred([this]() { ToggleSettingsMenu(); }); });
    // Route CRT assertion failures to stderr instead of a modal dialog.
    // The SDK's bit_stream assert fires on the audio thread; its dialog is
    // modal, so it BLOCKS Dac::XenonThread, which is the only thing that
    // drains the audio command queue (System::ExecuteCommands). The queue
    // then overflows its 256000-byte buffer and corrupts its own owner
    // struct, which is the access violation that follows every one of these
    // dialogs. With the dialog gone the thread keeps running and the assert
    // is recorded rather than fatal. This changes only how the shared CRT
    // reports asserts -- the SDK itself is untouched.
    _set_error_mode(_OUT_TO_STDERR);
    Sk83InstallSoftAsserts();
    // Dev: disassemble any *.bin in skate3_disasm_ucode_dir. The file was
    // compiled into this project but never called (retail calls it from
    // Skate3BaseApp), so the cvar silently did nothing.
    skate3::shader_disasm::RunIfRequested();

    // Skate3InitPcRenderer();
    // Skate3RegisterPcRendererHooks(runtime()->function_dispatcher());
    // NOTE: the generated RegisterRecompiledModules (guest module -> host
    // shared library, from the manifest's [[modules]] list) needs no call
    // from here -- sk83_na_m_init.cpp already hands it to the runtime as
    // PPCImageConfig::register_modules. Calling it again here just logged
    // "RegisterRecompiledModule: duplicate guest_path".
    skate3::native_render::Install();
    // A splitscreen window covers only part of its monitor; this toggle
    // would take a double click there for "windowed" and fill the monitor.
    if (!skate3::splitscreen::IsPlayer()) {
      Skate3InitFullscreenToggle();
    }
    Skate3InitGuestAspect(runtime() ? runtime()->memory() : nullptr);

    // Route Skate 3's "big:"/"dlcbig:" world-content-probe schemes to the
    // same mounted title device so paths like big:\data\content\world\stream
    // resolve instead of failing "device not found" -- this is routing to
    // existing content, not a separate archive; retail calls the equivalent
    // (Skate3BaseApp::InstallBigDeviceAliases) from OnPostSetup too.
    if (auto* fs = runtime() ? runtime()->file_system() : nullptr) {
      fs->RegisterSymbolicLink("big:", "\\Device\\Harddisk0\\Partition1");
      fs->RegisterSymbolicLink("dlcbig:", "\\Device\\Harddisk0\\Partition1");
    }

    // BIG-directory VFS overlay: most content lives inside .big
    // archives, but the game still probes the loose directory tree
    // before reading, and those directories are absent from disk. Every
    // probe failed, so the subsystems depending on them came up
    // half-initialised -- which is what left the audio command queue
    // running away and a GrainPlayer with a garbage vtable.
    InstallBigDirectoryOverlay();
    InstallWebkitOverlay();

    // F12: native-render debug menu (live cvar controls, per-subsystem
    // toggles). Lazily constructs native_debug_dialog_ on first press --
    // retail's Skate3BaseApp::ToggleNativeDebug() also calls
    // ApplySettingsCursorMode()/ApplyGameplayCursorMode() around this to
    // switch the cursor between gameplay-capture and free-mouse modes;
    // Sk83NaMApp doesn't have those helpers, so the cursor won't change
    // mode when the dialog opens/closes. Cosmetic gap, not a blocker --
    // add the equivalent if this app grows a settings-menu cursor mode.
    // F5: native <-> emulated renderer. skate3::native_scene provides
    // ToggleSceneEnabled() and retail binds it here, but this project never
    // registered the bind, so the key did nothing.
    // Remembered handle: the F11 paired A/B parity capture (native +
    // emulated screenshots + gsnap, sequenced from the guest frame loop)
    // needs the window without an app pointer. Never registered before, so
    // every F11 capture ran without screenshots.
    skate3::screenshot::RememberWindow(window() ? window()->GetNativeWindowHandle()
                                                : nullptr);
    rex::ui::RegisterBind("bind_skate3_screenshot", "F6",
                          "Save screenshot to screenshots/", [this] {
                            skate3::screenshot::CaptureWindow(
                                window() ? window()->GetNativeWindowHandle() : nullptr);
                          });
    rex::ui::RegisterBind("bind_skate3_native_render_toggle", "F5",
                          "Toggle native/emulated renderer", [] {
                            skate3::native_scene::ToggleSceneEnabled();
                          });

    // The guest debug-variable console (the game's own cVar registry, ~2500
    // variables, edited live), on the key games conventionally use.
    //
    // TWO binds, because the Win32 backend identifies keys by VIRTUAL-KEY
    // code (window_win.cpp HandleKeyboard passes wParam straight through),
    // and VK codes are LAYOUT-dependent -- the scancode is available in
    // lParam but unused. The key left of "1" is therefore VK_OEM_3
    // ("Backtick") on US/UK but VK_OEM_5 ("Backslash") on the Nordic
    // layouts, where it prints as the paragraph sign. Binding both makes
    // the console open on that physical key on either layout; each stays
    // rebindable in the settings overlay.
    // OPEN only. Closing is handled inside the console itself, which watches
    // the same keys: the console captures no input, so this bind would also
    // fire on the press that is meant to close it and the two would cancel
    // out (open -> closed again in the same press).
    const auto toggle_console = [this] {
      if (!guest_console_) {
        guest_console_ =
            std::make_unique<skate3::GuestConsoleDialog>(imgui_drawer());
      }
      if (!guest_console_->visible()) {
        guest_console_->Show();
      }
    };
#ifdef SK83_ZD_DEBUG
    zd_debug::overlay::Create(imgui_drawer());  // src/zd_debug (removable)
#endif
    rex::ui::RegisterBind("bind_skate3_guest_console", "Backtick",
                          "Guest debug variable console", toggle_console);
    rex::ui::RegisterBind("bind_skate3_guest_console_alt", "Backslash",
                          "Guest debug variable console (Nordic layouts)",
                          toggle_console);

    rex::ui::RegisterBind("bind_skate3_native_debug", "F12",
                          "Native render debug menu", [this] {
                            if (!native_debug_dialog_) {
                              native_debug_dialog_ =
                                  std::make_unique<skate3::NativeDebugDialog>(
                                      imgui_drawer());
                            }
                            native_debug_dialog_->Toggle();
                          });

    // Escape / F1 / the controller menu chord: the player-facing settings
    // screen (skate3_settings_menu.cpp). Escape backs out level by level
    // (rows -> category rail -> closed) and opens the menu when closed.
    skate3::CaptureSettingsBootValues();
    rex::ui::RegisterBind("bind_skate3_menu", "Escape", "Settings", [this] {
      if (settings_menu_ && settings_menu_->visible()) {
        settings_menu_->NavigateBack();
      } else {
        ToggleSettingsMenu();
      }
    });
    rex::ui::RegisterBind("bind_skate3_menu_alt", "F1", "Settings (alternate)",
                          [this] { ToggleSettingsMenu(); });
    if (auto* input_system = runtime() ? static_cast<rex::input::InputSystem*>(
                                             runtime()->input_system())
                                       : nullptr) {
      // Keep controller/keyboard input away from the game while the menu
      // is open (it navigates with the same buttons).
      input_system->SetActiveCallback(
          [this]() { return !(settings_menu_ && settings_menu_->visible()); });
      input_system->SetMenuChordCallback([this]() {
        app_context().CallInUIThreadDeferred([this]() { ToggleSettingsMenu(); });
      });
    }

    // Ctrl+F1: the SDK's generic cvar overlay -- every registered cvar,
    // grouped by category with search and "Save to config", plus keybind
    // rebinding (developer view; the player settings live on Escape/F1).
    //
    // rex::ui::SettingsDialog has no Show/Hide of its own (unlike
    // NativeDebugDialog): an ImGuiDialog attaches to the drawer when it is
    // constructed and detaches when destroyed, and is_draw_active_ starts
    // true -- so constructing shows it and resetting the pointer closes it.
    rex::ui::RegisterBind("bind_rex_settings", "Ctrl+F1", "All cvars (developer)", [this] {
      if (settings_dialog_) {
        settings_dialog_.reset();
        return;
      }
      settings_dialog_ = std::make_unique<rex::ui::SettingsDialog>(
          imgui_drawer(), std::filesystem::current_path() / "sk83_na_m.toml");
    });
  }

  // Load the xex under its original name. The SDK defaults to
  // "game:\default.xex" (rex_app.cpp), which forced the image to be renamed.
  // Keeping the real name also makes the guest's own module path correct,
  // which is what EA::Callstack appends ".map" to when it loads symbols for
  // assert callstacks -- so sk83_na_m.map next to the xex resolves frames to
  // function names (generate it with tools/gen_map.py).
  // Always sk83_na_m.xex (there is no default.xex for this build).
  void OnLoadXexImage(std::string& xex_image) override {
    xex_image = "game:\\sk83_na_m.xex";
    if (!std::filesystem::exists(game_data_root() / "sk83_na_m.xex")) {
      REXLOG_ERROR("sk83_na_m.xex not found in {}", game_data_root().string());
    }
    REXLOG_INFO("entrypoint xex: {}", xex_image);
  }

  // Override virtual hooks for customization:
  // void OnPostInitLogging() override {}
  // void OnPreSetup(rex::RuntimeConfig& config) override {}
  // void OnCreateDialogs(rex::ui::ImGuiDrawer* drawer) override {}
  // void OnShutdown() override {}

 private:
  void ToggleSettingsMenu() {
    if (settings_menu_) {
      if (settings_menu_->visible()) {
        settings_menu_->Hide();
      } else {
        OpenSettingsMenu();
      }
      return;
    }
    auto close_settings = [this]() {
      // Fires on every Hide (Esc/B, Close Settings, Close Game, Restart).
      skate3::native_scene::SetSettingsMenuBlur(false);
      if (window()) {
        window()->SetCursorVisibility(rex::ui::Window::CursorVisibility::kAutoHidden);
      }
    };
    auto close_game = [this]() {
      app_context().CallInUIThreadDeferred([this]() {
        if (window()) {
          window()->RequestClose();
        } else {
          app_context().QuitFromUIThread();
        }
      });
    };
    auto restart_game = [this, close_game]() {
      if (skate3::RelaunchGame()) {
        close_game();
      }
    };
    // Raw pad poll for menu navigation: bypasses the active-callback gate
    // that zeroes guest-facing input while the menu is open.
    auto poll_gamepad = [this]() {
      rex::ui::SimpleSettingsGamepad pad;
      auto* rt = runtime();
      auto* input_system =
          rt ? static_cast<rex::input::InputSystem*>(rt->input_system()) : nullptr;
      if (input_system) {
        rex::input::X_INPUT_GAMEPAD state;
        if (input_system->GetUiGamepadState(&state)) {
          pad.connected = true;
          pad.buttons = state.buttons;
          pad.thumb_lx = state.thumb_lx;
          pad.thumb_ly = state.thumb_ly;
        }
      }
      return pad;
    };
    skate3::SettingsMenuEnv env;
    env.server_hosts = {"gosredirector.stest.ea.com", "gosredirector.sdev.ea.com",
                        "gosredirector.scert.ea.com", "skate3_web"};
    env.profiles_path = profiles_path_;
    env.is_fullscreen = [] { return Skate3IsFullscreen(); };
    env.set_fullscreen = [](bool on) { Skate3SetFullscreen(on); };
    // A rename reaches the running kernel profile right away (the server
    // sees it from the next connection).
    env.profile_changed = [this] {
      auto* rt = runtime();
      if (rt && rt->kernel_state() && rt->kernel_state()->user_profile()) {
        rt->kernel_state()->user_profile()->SetIdentity(
            std::strtoull(rex::cvar::Query<std::string>("user_profile_xuid").c_str(), nullptr,
                          16),
            rex::cvar::Query<std::string>("user_profile_name"));
      }
    };
    settings_menu_ = std::make_unique<skate3::SettingsMenu>(
        imgui_drawer(), std::filesystem::current_path() / "sk83_na_m.toml",
        std::move(close_settings), std::move(close_game), std::move(restart_game),
        std::move(poll_gamepad), std::move(env));
    OpenSettingsMenu();
  }

  void OpenSettingsMenu() {
    if (window()) {
      window()->SetCursorVisibility(rex::ui::Window::CursorVisibility::kVisible);
    }
    skate3::native_scene::SetSettingsMenuBlur(true);
    settings_menu_->Show();
  }

  std::unique_ptr<skate3::SettingsMenu> settings_menu_;
  std::filesystem::path profiles_path_;
  std::unique_ptr<skate3::NativeDebugDialog> native_debug_dialog_;
  std::unique_ptr<skate3::GuestConsoleDialog> guest_console_;
  std::unique_ptr<rex::ui::SettingsDialog> settings_dialog_;
};
