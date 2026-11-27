# Release notes

## 1.1.0

Performance. Each piece of game code that now runs natively was checked against
the recompiled original and gives bit-identical results.

- **Faster busy scenes:** the custom renderer no longer lets the game submit draw
  calls for static world geometry it already holds. About half of the main
  view's draw calls go away in Downtown. Static items are built once and reused
  instead of every frame. The game's own depth pre-pass is skipped for the main
  view (the custom renderer does its own), and the world-shading pass records its
  meshes directly.
- **Native game code:** the rigid-body physics solver (about 2x faster),
  animation pose and blend jobs, packed-animation decoding and header
  decoding, collision queries, the job system's job runner and the audio
  library's lock. Audio updates run on their own thread, and the
  living-world animation fetch is split across helper threads, which shortens the
  pedestrian/traffic job the renderer waits on.
- **Lower and steadier input latency:** frames are timed by when the game
  stored them, and the simulation tick timer runs on an exact 60 Hz line instead
  of the game's jittery timer thread. In testing, the present delay went from
  about 21-28 ms to about 18 ms. Also fixes the camera running a frame ahead after
  a respawn.
- **Less host overhead per draw:** shader classification tables were enlarged,
  and the per-draw debug-console check no longer takes a lock.
- **Profile-guided optimisation:** the executable is optimised with a new
  profile recorded on this code.

Fixes

- **Game freeze:** fixed a race in the game's job system. A job could wait
  forever on a dependency that had already finished, freezing the game.
- **Crash while loading:** the game deleted the old streamer controller before
  publishing the new one, so the render thread could call into a deleted object.
  It caused about 2% of loads to crash on PC. The old controller is now deleted
  only after the swap is safe.
- **AMD GPU hang on Direct3D 12:** fixed. Shadow passes left shader inputs
  unbound, and render targets were created uninitialised. GPU crash diagnostics
  (DRED) are now always on, and Direct3D 12 debug messages are written to the log.
- **Streaming stalls:** file lookups no longer hold the lock that every guest
  thread's atomic operations use.
- **Animation precision:** two-frame animation decoding now rounds exactly like
  the game.

Vulkan

- **Frame generation on Vulkan:** generated frames between game frames now work
  on the Vulkan backend. The FPS counter shows the real game rate next to the
  presented rate there too. The separate native render thread remains Direct3D
  12-only.

Diagnostics

- Unhandled crashes and pure virtual calls log a host call chain. A call through
  a null function pointer logs the guest call stack. A job-system watchdog
  reports stuck jobs with their dependencies.

- **SDK revision:** `942046b66c098113fb578422f914b121e721f7c4` ([source](https://github.com/5C8/rexglue-skate3/commit/942046b66c098113fb578422f914b121e721f7c4)).

## 1.0.0

First public snapshot.
