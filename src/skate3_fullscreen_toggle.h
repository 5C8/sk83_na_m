#pragma once

// Subclass the game window so that double-clicking inside the client area
// (or pressing Alt+Enter) toggles between windowed and borderless
// fullscreen on the monitor the window currently sits on.
void Skate3InitFullscreenToggle();
// Current borderless-fullscreen state, and a programmatic toggle through the
// same path as Alt+Enter (no-op until the window is subclassed). UI thread.
bool Skate3IsFullscreen();
void Skate3SetFullscreen(bool fullscreen);