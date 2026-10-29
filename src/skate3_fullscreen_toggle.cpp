// Double-click and Alt+Enter to toggle windowed / borderless fullscreen.
//
// We subclass the game window's WndProc once it appears. Double-clicking
// the client area, or pressing Alt+Enter, removes the border decorations
// and resizes the window to the monitor's full work area. Toggling again
// restores the original window rect and style.
//
// We never touch DXGI's exclusive fullscreen state -- this is borderless
// only, which the recomp's swapchain handles transparently.

#include "skate3_fullscreen_toggle.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <windowsx.h>

namespace {

void DbgLog(const char* fmt, ...) {
  char buf[512];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  OutputDebugStringA(buf);
}

std::atomic<int> g_msg_log_budget{20};  // log first N WndProc hits

void MaybeLogMsg(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
  if (g_msg_log_budget.load(std::memory_order_relaxed) <= 0) return;
  // Only log the mouse/keyboard messages that matter here.
  if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK ||
      msg == WM_SYSKEYDOWN) {
    int left = g_msg_log_budget.fetch_sub(1, std::memory_order_relaxed);
    (void)left;
    DbgLog("[fs-toggle] wnd=%p msg=0x%04X w=0x%p l=0x%p\n",
           (void*)hwnd, msg, (void*)w, (void*)l);
  }
}

WNDPROC g_orig_wndproc = nullptr;
std::atomic<bool> g_is_fullscreen{false};
std::atomic<HWND> g_hwnd{nullptr};  // the subclassed game window
RECT g_saved_rect{};
LONG g_saved_style = 0;
LONG g_saved_exstyle = 0;

void LogRect(const char* label, HWND hwnd) {
  RECT r{};
  GetWindowRect(hwnd, &r);
  DbgLog("[fs-toggle] %s rect=(%ld,%ld)-(%ld,%ld) w=%ld h=%ld\n",
         label, r.left, r.top, r.right, r.bottom,
         r.right - r.left, r.bottom - r.top);
}

// Rex creates the window borderless-fullscreen from the start, so we
// don't have "the original windowed rect" to remember. When toggling
// from fullscreen -> windowed we use this default centered rect. Tweak
// to taste (or later plumb through a cvar).
constexpr int kDefaultWindowedW = 1600;
constexpr int kDefaultWindowedH = 900;

RECT CenteredDefaultRect(HMONITOR mon) {
  MONITORINFO mi{sizeof(MONITORINFO)};
  GetMonitorInfo(mon, &mi);
  const LONG mw = mi.rcMonitor.right - mi.rcMonitor.left;
  const LONG mh = mi.rcMonitor.bottom - mi.rcMonitor.top;
  const LONG w  = kDefaultWindowedW;
  const LONG h  = kDefaultWindowedH;
  RECT r{};
  r.left   = mi.rcMonitor.left + (mw - w) / 2;
  r.top    = mi.rcMonitor.top  + (mh - h) / 2;
  r.right  = r.left + w;
  r.bottom = r.top  + h;
  return r;
}

void DoToggle(HWND hwnd) {
  DbgLog("[fs-toggle] DoToggle: currently %s\n",
         g_is_fullscreen.load() ? "FULLSCREEN" : "WINDOWED");
  LogRect("before", hwnd);
  HMONITOR mon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi{sizeof(MONITORINFO)};
  GetMonitorInfo(mon, &mi);

  if (!g_is_fullscreen.load()) {
    // WINDOWED -> FULLSCREEN. Save current rect only if it looks like a
    // real windowed size (smaller than the monitor). If we're already at
    // monitor size, keep any previously-saved rect so a later restore
    // works.
    RECT cur{};
    GetWindowRect(hwnd, &cur);
    const bool cur_is_fullsize =
        cur.left <= mi.rcMonitor.left && cur.top <= mi.rcMonitor.top &&
        cur.right >= mi.rcMonitor.right && cur.bottom >= mi.rcMonitor.bottom;
    if (!cur_is_fullsize) {
      g_saved_rect = cur;
    }
    g_saved_style   = GetWindowLong(hwnd, GWL_STYLE);
    g_saved_exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    DbgLog("[fs-toggle] saved style=0x%lX exstyle=0x%lX\n",
           g_saved_style, g_saved_exstyle);

    LONG new_style = g_saved_style & ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU |
                                       WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
    SetWindowLong(hwnd, GWL_STYLE, new_style);
    LONG new_exstyle = g_saved_exstyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE |
                                           WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(hwnd, GWL_EXSTYLE, new_exstyle);

    BOOL ok = SetWindowPos(hwnd, HWND_TOP,
                 mi.rcMonitor.left, mi.rcMonitor.top,
                 mi.rcMonitor.right - mi.rcMonitor.left,
                 mi.rcMonitor.bottom - mi.rcMonitor.top,
                 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    DbgLog("[fs-toggle] -> FULLSCREEN SetWindowPos ok=%d err=%lu\n",
           ok, GetLastError());
    g_is_fullscreen.store(true);
  } else {
    // FULLSCREEN -> WINDOWED. Add proper window chrome back so the user
    // can actually see the mode change even if the game started
    // borderless. Use the saved rect if we have one that's a real
    // windowed size, otherwise fall back to a centered default.
    RECT target = g_saved_rect;
    const bool saved_valid =
        target.right - target.left > 0 &&
        target.bottom - target.top > 0 &&
        (target.right - target.left) < (mi.rcMonitor.right - mi.rcMonitor.left) &&
        (target.bottom - target.top) < (mi.rcMonitor.bottom - mi.rcMonitor.top);
    if (!saved_valid) target = CenteredDefaultRect(mon);

    // Restore original style, and OR in decorations so we get chrome
    // back even if the initial state was borderless.
    LONG restore_style = g_saved_style |
                         WS_OVERLAPPEDWINDOW;  // caption+thickframe+sysmenu+minbox+maxbox
    LONG restore_exstyle = g_saved_exstyle;
    SetWindowLong(hwnd, GWL_STYLE, restore_style);
    SetWindowLong(hwnd, GWL_EXSTYLE, restore_exstyle);
    BOOL ok = SetWindowPos(hwnd, HWND_TOP,
                 target.left, target.top,
                 target.right - target.left,
                 target.bottom - target.top,
                 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    DbgLog("[fs-toggle] -> WINDOWED to (%ld,%ld) %ldx%ld ok=%d err=%lu\n",
           target.left, target.top,
           target.right - target.left, target.bottom - target.top,
           ok, GetLastError());
    g_is_fullscreen.store(false);
  }
  LogRect("after", hwnd);
}

LRESULT CALLBACK SubclassWndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
  MaybeLogMsg(hwnd, msg, w, l);
  // Double-click toggling is DELIBERATELY not handled: it fired by
  // accident during normal play. Alt+Enter remains the toggle.
  // Alt+Enter: SYSKEYDOWN with VK_RETURN, ALT held flagged in bit 29 of lParam.
  if (msg == WM_SYSKEYDOWN && w == VK_RETURN && (l & (LPARAM(1) << 29))) {
    DoToggle(hwnd);
    return 0;
  }
  return CallWindowProc(g_orig_wndproc, hwnd, msg, w, l);
}

HWND FindMainWindow() {
  struct Ctx { HWND best; DWORD pid; LONG best_area; };
  Ctx ctx{nullptr, GetCurrentProcessId(), 0};
  EnumWindows([](HWND hwnd, LPARAM lp) -> BOOL {
    auto* c = (Ctx*)lp;
    DWORD wpid = 0;
    GetWindowThreadProcessId(hwnd, &wpid);
    if (wpid != c->pid) return TRUE;
    if (!IsWindowVisible(hwnd)) return TRUE;
    RECT r;
    if (!GetClientRect(hwnd, &r)) return TRUE;
    LONG area = (r.right - r.left) * (r.bottom - r.top);
    // Log every process-owned visible window we consider so we can see
    // if we're picking the wrong HWND (e.g. an ImGui child instead of
    // the game window).
    wchar_t cls[64] = {}, title[128] = {};
    GetClassNameW(hwnd, cls, 63);
    GetWindowTextW(hwnd, title, 127);
    char cls_a[64] = {}, title_a[128] = {};
    WideCharToMultiByte(CP_UTF8, 0, cls, -1, cls_a, sizeof(cls_a), nullptr, nullptr);
    WideCharToMultiByte(CP_UTF8, 0, title, -1, title_a, sizeof(title_a), nullptr, nullptr);
    DbgLog("[fs-toggle] candidate hwnd=%p area=%ld class='%s' title='%s'\n",
           (void*)hwnd, area, cls_a, title_a);
    if (area > c->best_area) { c->best_area = area; c->best = hwnd; }
    return TRUE;
  }, (LPARAM)&ctx);
  return ctx.best;
}

void InitThread() {
  DbgLog("[fs-toggle] InitThread starting -- polling for main window\n");
  HWND hwnd = nullptr;
  for (int i = 0; i < 600; ++i) {  // up to 60 s
    hwnd = FindMainWindow();
    if (hwnd) break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  if (!hwnd) {
    DbgLog("[fs-toggle] gave up -- no main window found in 60 s\n");
    return;
  }
  DbgLog("[fs-toggle] subclassing hwnd=%p\n", (void*)hwnd);
  g_hwnd.store(hwnd);
  SetLastError(0);
  g_orig_wndproc = (WNDPROC)SetWindowLongPtrW(
      hwnd, GWLP_WNDPROC, (LONG_PTR)SubclassWndProc);
  const DWORD err = GetLastError();
  if (!g_orig_wndproc && err) {
    DbgLog("[fs-toggle] SetWindowLongPtrW failed err=%lu\n", err);
    return;
  }

  // Detect the initial window state. Rex creates the window
  // borderless-fullscreen, so if we find the window sized to the
  // monitor at subclass time, remember that as "fullscreen" and pick a
  // sensible default windowed rect for the first restore.
  RECT cur{};
  GetWindowRect(hwnd, &cur);
  HMONITOR mon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi{sizeof(MONITORINFO)};
  GetMonitorInfo(mon, &mi);
  const bool starts_fullscreen =
      cur.left <= mi.rcMonitor.left && cur.top <= mi.rcMonitor.top &&
      cur.right >= mi.rcMonitor.right && cur.bottom >= mi.rcMonitor.bottom;
  if (starts_fullscreen) {
    g_is_fullscreen.store(true);
    // Preallocate a sane windowed rect so the first fullscreen->windowed
    // toggle actually changes visible size.
    g_saved_rect  = CenteredDefaultRect(mon);
    g_saved_style = GetWindowLong(hwnd, GWL_STYLE);
    g_saved_exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    DbgLog("[fs-toggle] initial state: FULLSCREEN (cur=%ldx%ld mon=%ldx%ld) -- "
           "first toggle will windowize to %dx%d\n",
           cur.right - cur.left, cur.bottom - cur.top,
           mi.rcMonitor.right - mi.rcMonitor.left,
           mi.rcMonitor.bottom - mi.rcMonitor.top,
           kDefaultWindowedW, kDefaultWindowedH);
  } else {
    g_is_fullscreen.store(false);
    DbgLog("[fs-toggle] initial state: WINDOWED (cur=%ldx%ld)\n",
           cur.right - cur.left, cur.bottom - cur.top);
  }

  // Make sure the window class generates WM_LBUTTONDBLCLK. If CS_DBLCLKS
  // is already on this is a no-op; if not, Windows starts coalescing the
  // second click of a pair into a true double-click message.
  const LONG_PTR class_style = GetClassLongPtrW(hwnd, GCL_STYLE);
  DbgLog("[fs-toggle] class style before=0x%lX CS_DBLCLKS=%s\n",
         (unsigned long)class_style,
         (class_style & CS_DBLCLKS) ? "yes" : "no");
  if (!(class_style & CS_DBLCLKS)) {
    SetClassLongPtrW(hwnd, GCL_STYLE, class_style | CS_DBLCLKS);
    DbgLog("[fs-toggle] class style updated to add CS_DBLCLKS\n");
  }
  DbgLog("[fs-toggle] subclass installed -- waiting for clicks\n");
}

}  // namespace

void Skate3InitFullscreenToggle() {
  std::thread(InitThread).detach();
}

bool Skate3IsFullscreen() {
  return g_is_fullscreen.load();
}

void Skate3SetFullscreen(bool fullscreen) {
  // Same path as Alt+Enter, so the settings menu and the hotkey never
  // disagree about the window state. Called on the UI thread, which owns
  // the window.
  HWND hwnd = g_hwnd.load();
  if (hwnd == nullptr || fullscreen == g_is_fullscreen.load()) {
    return;
  }
  DoToggle(hwnd);
}