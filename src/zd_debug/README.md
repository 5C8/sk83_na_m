# zd_debug: sk83_na_zd debug features in sk83_na_m

The Aug 2009 dev build `sk83_na_zd` contains debug systems that the Dec 2009
`sk83_na_m` build compiled out. This folder re-adds them to the sk83_na_m
recompilation as hooks.

* **Removable:** everything is in `src/zd_debug/`. Delete the folder and the
  `SK83_ZD_DEBUG` block in `CMakeLists.txt`, or configure with
  `-DSK83_ZD_DEBUG=OFF`.
* **Switchable:** cvar `zd_debug` (default on) turns every feature off at
  runtime; each feature also has its own cvar. With a feature off, its hooks
  fall straight through to the game's code.
  The player settings expose this master switch as **System → Debugging →
  ZD debugging features**. The checkbox applies immediately and saves to TOML.
* **CPU profiling scopes:** zd's scope-entry/exit helpers (`834643A8` and
  `83464470`) are host no-ops. Their per-thread profiler was compiled out of m;
  interpreting m's TLS slot 1 as that profiler produced a null event buffer
  and eventual writes past guest `0x20000`. The debug drawing and menus remain
  available. Both helpers are mapped in `zdx_config.py` so full regeneration
  retains this fix. To apply new HOST mappings to existing generated output
  without the original runtime snapshot/cache inputs, use:

  ```
  python sk83_na_zd/tools/zdx.py --retarget-host-calls sk83_na_zd/tools/zdx_config.py sk83_na_m/src/zd_debug 834643A8 83464470
  ```
* **How a port works: zd's own code runs (zdx transplant).** zd's
  recompiled C++ for each debug function is copied by
  `sk83_na_zd/tools/zdx.py` (set: `tools/zdx_config.py`) into
  `zdx_generated.{h,cpp}` -- regenerate, never edit -- and rewired to run
  inside m:
  * calls go to the matched m function, another transplanted zd function
    (`zdx_<zd addr>`), or a host replacement (`zdx_host.cpp`);
  * every zd global goes through `zdx::Map()`: zd's constant data and zd-only
    state live in a guest-memory pool (zd image + zd memory snapshot), m's
    globals/cvars are mapped (by name for cvars, by aligned-code voting for
    the rest, `tools/global_map.py`), zd-only cvars are registered with the
    game's cvar system under their zd names (see Options);
  * struct fields that moved are remapped (`STRUCTS`, measured by aligning
    live objects in zd/m memory dumps, `tools/struct_align.py`, and checked
    against matched-method offset votes); unverified fields are errors;
  * virtual calls are translated to m's vtable slot (`tools/vtable_map.py`:
    ~1500 paired vtables); methods m dropped run zd's transplanted version;
  * zd's DebugOverlay shape store is replaced by a host queue replayed
    through zd's DebugShapeRenderer (`zdx_host.cpp`); 3D shapes draw in the
    world debug-draw callback, text/2D in the debug HUD text pass.
  * startup code for objects m lacks runs once as fragments (config `INIT`,
    e.g. ThePrintChannelMan); callbacks zd installs at runtime get thunks
    (`FNPTRS`); fragment live-in constants come from `LIVEIN`;
  * **debug blocks m compiled out of functions it kept** (`BLOCKS`,
    `tools/zdx_blocks.py`, found by `tools/debug_blocks.py`): each block
    guarded by a dead option runs as a fragment *after* m's own function, so
    m's gameplay code is untouched. The block's live-in registers are
    recomputed by a backward slice (`tools/block_slice.py`: each use resolved
    at its own position, stack values through the stores that wrote them);
    the generated override (`zdx_overrides.cpp`) tests the option first and
    only then copies the entry registers and runs the fragment;
  * **calls to stripped zd functions from functions m kept** (`CALLBLOCKS`,
    `tools/zdx_callblocks.py`): the call plus its argument slice, gated by the
    callee's own debug switches (bool options whose zd default is off).
    Hand-written entries in `zdx_config.py`: `["*"]` = ungated (zd always ran
    it; only needs the pool, so it also works before the first frame),
    `["*pre"]` = run *before* m's function (it needs what the call sets up),
    `CALLER@LO-HI` = a whole instruction range (e.g. a Snprintf into a stack
    buffer plus the call using it), `CALL_ARGS` = a callee's argument registers
    (the slice skips the others), `CALLS` = route the call to a fragment (only
    the debug part of a stripped function, e.g. SurfacePhysics::Update);
  * zd function addresses used as values (sort comparators, callbacks) become
    m's pair address or a dispatcher thunk to the transplant (`zdx::FnThunk`);
    indirect calls through null are logged and skipped (`zdx::IndirectCall`);
  * vtables transplanted code installs: m's paired vtable when one exists
    (`tools/match/vtmap.json`), else the pool copy with each slot fixed up to m's
    function or a thunk to the transplant (automatic; abstract bases with a
    `_purecall` slot are skipped, config `VTABLES` for explicit ones);
  * other config: `REPLACE_EARLY` (a REPLACE that runs before the first frame),
    `TAPS` (host function called at entry of a generated override), `PATCH`
    with `"#N"` (instruction N of a function), `HOST_ADDRS` (zd data only the
    host uses, kept in the pool); OWN `snapshot` sub-ranges are byte-exact;
  * `REPLACE` runs zd's whole version of a kept function (only where the
    zd/m layouts are verified);
  * **gated REPLACE**: when a debug block needs values its function computed
    with other calls (a slice can't recompute them after m's function), zd's
    whole function runs instead of m's -- only while one of the block's
    options is on (automatic for such BLOCKS / CALLBLOCKS; `GATED_REPLACE`
    for gameplay functions m inlined whose debug output sits inside the
    inlined callee, e.g. SkeletonIK contact debug, RAIN road draws, character
    render debug). Options on by default in zd are never gates. Fields read
    through other objects are checked against m's aligned code: same
    instruction with only the displacement changed (incl. renamed registers
    and `li` index constants) is patched to m's offset; anything else is an
    error until mapped (`PATCH` / `REPLACE_OK`, `field_ctx.py <fn> #insn`).
    Functions m's own sources already override (native renderer) are refused;
  * slicing: definitions follow the control flow (not the textual order),
    stack buffers count as read only when loaded or passed to a call (not
    for hidden by-value result buffers), and side-effect-free calls are
    repeated for their result (getters, const methods, Math statics, and any
    function that only writes its own stack frame -- `stores_nothing`);
  * call blocks whose callee only has numeric options (positions, sizes) are
    gated by the caller's option block around the call; enum display modes
    (value 0 = off) gate like bools;
  * `li`-indexed struct accesses (`li r10,N` / `lvx v,rThis,r10`) are mapped
    like d-form fields; `FIELD_TYPES` types pointer members (e.g. BipedAir
    +8 = Skeleton) so accesses through them are mapped too; pointers several
    callers pass with their own maps get the union (`merged:`);
  * struct maps for `this` come from the zd/m pair alignment of every paired
    method of the class (`zdx_replace.py`, strict: fields without evidence are
    errors), struct types propagate into transplanted callees, and voted global
    mappings are checked against the zd/m memory dumps;
  * blocks that cannot be ported automatically yet stay in `PENDING` /
    `CALL_PENDING` (unverified fields, values from calls, branches out of the
    block) with the reason in the zdx report;
  * the zd -> m function pairs it calls are checked by `tools/pair_audit.py`
    (callee agreement + body similarity); wrong pairs go in
    `tools/match/bad_pairs.tsv` and zd's code is transplanted instead.
  The tool prints a review report (every external call, mapped global, field
  rewrite, patch, virtual call) and refuses anything it cannot map.
* **Options:** the features read the game's debug options exactly as zd
  does, under zd's names. Options m already has are m's own cvars (its menus
  drive them). Options only zd has are registered with the game's cvar system
  by zd's own static initializer (transplanted) at startup -- same name,
  group, label and default as in zd. `--zd_debug_set="kShowSkaterDebug=1;..."`
  sets options by name at startup. zd-only options (and the print channels'
  `kbDebugPrint_<channel>` options ThePrintChannelMan creates) are registered
  right after m constructs its cvar manager, before it reads startup.ini
  (`boot.cpp`, as zd did), so startup.ini and the guest console set them like
  any other option. m's `cVarBoolRef` constructor links before storing its
  default, so the host stores the default first (`zdx_host_cVarBoolRef`).
* **Data only zd has:** fields zd's objects carry and m's dropped (e.g. the
  Skeleton's toe/hand debug matrices) live in per-object extension blocks,
  filled by zd's own code (transplanted fragments, e.g. from
  Skeleton::ProcessData) and read by zd's debug code.

## How the inventory was made

`sk83_na_zd/tools/stripped_features.py` lists, per zd class (names recovered
from the sk82_na_zd .map), whether any of its functions matched an m function
and whether the strings its code references survive in m's image. A class is
*stripped* when nothing matched and none of its strings are in m. Most of the
622 stripped classes are assert text (m has asserts compiled out) or debug
builds of system libraries (XAudio, D3D capture); those are not features. The
game-side debug systems are below.

## Systems (status)

| # | System | zd evidence | Status |
|---|--------|-------------|--------|
| 1 | **DebugOverlay** — the debug-draw API every feature below draws through: Line2D/3D, Curve, Triangle, wide lines, Sphere, Circle, pie wedge, Box/AABox/AACube, Rectangle, Arrow, Cylinder, Capsule, Axes, physics volumes/aggregates, Text2D (scaled, outlined, persistent, permanent), Text3D, PrintTextLine | `Sk8::Debug::DebugOverlay` 0x83A47E88..0x83A4D048 (46 fns), `DebugOverlayPresentation` 0x8389E130 / Draw 0x838A1420, `DebugShapeRenderer`, `DebugTextManager` 0x83629558 | **done** -- zd's code (transplanted), both renderers |
| 2 | **Debug HUD panel** — "Warning Count" list (e.g. missing DMO physics assemblies), "Debug Object Bandwidth", "IP:", entity counters (Skaters/DMOs/Pedestrians/Vehicles/Waypoints/Crowds/Terrain), surface type | strings only in zd; drawn via DebugOverlay | **mostly**: RenderMan::DebugRender runs as zd's code (`render_debug.cpp`): BE state names, marquee viewer text, image-operate test, sun direction map, print-channel warnings (`kbShowWarnings`). **Debug-strings panel** (top right, `kbDrawDebugStrings`, default on): zd's `Sk8::Debug::Manager` (DebugTextManager) is created at app init (CALLBLOCKS `*pre` on GameModule's init) and drawn from the HUD text pass (`DrawDebugStrings`; zd drew it as a scene-graph object); its DebugStringRefs live in extension blocks of their owners: Speed km/h + m/s and Deck Position (SkaterPresentation), Time (SimPresentation), IP (Sk8GameTalkProtocol), Replay Bandwidth (GameModule). **Surface Type** (`kShowSurfaceType`) and the **camera state path** (`kbCameraGraphDebugDraw`) at the top left are ported. Open: "Debug Object Bandwidth" (zd's DebugOverlayPresentation, a presentation object m does not have); the warnings list stays empty because zd issues those warnings inside gameplay error paths (e.g. LWDynamicObjectModel::CreateFromMesh) -- porting them needs the branch conditions, not just the call; GameTalk's blinking busy marker (SetValue on state changes inside m's Update) |
| 2c | **RenderMan::RenderStats** (`render_debug.cpp`, replaces m 829A2E18) — entity counts, FPS, version, camera position, `kShowPauseManagerStates` (pause state names), `kDrawRWACProfile` (audio CPU profile), `kbDrawCSM` (shadow-map cascades), `kbDrawCaCSnapshot`, arena stats, debug graph | zd 83917590 | **done**, except: RWAC timings stay 0 (m's audio code has no profiler writer); arena table empty (m's XAllocator has no category tracking); `kbDrawCSM`/`kbShowFresnelLookupTexture` textured quads come out tiny/blank in m (open) |
| 2b | **BE state stack text** (`kbEnableBEStateName`) | zd StateManagerFrontEnd vtable slot 5 sends cMsgClear/AddDebugBEStateStack; m kept the receivers only | **done** (`be_stack_debug.cpp`) |
| 3 | **cvars m kept that draw nothing** (e.g. `kShowAnimationSkeleton`, `kShowPhysicsSkeleton`, `kDisplayBlendTree`) — their drawing went through DebugOverlay | both builds define the cvar | **in progress**: the `kShowSkaterDebug` chain is done (Actor / PhysicalPlayerHiLOD / Skeleton / SkeletonIK / SkeletonBody DebugDraw: skeleton views, centre of mass, IK, physics state + state-graph text); the rest per option |
| 4 | **zd-only debug cvars** — 215 switches, grouped below | cvar names only in zd | pending |
| 5 | **Debug graphs** — `kbEnableDebugGraphXY`, `Graph`/`Channel` (avg/min/max channels) | `Sk8::Debug::Graph` (CreateChannel 8362D098, SubmitChannel, Update 8362CB98, Render 8362A5E8), `GraphHandle`, `GraphMenu` | **done**: channels, Graph::Update, the in-game "Debug Graph" menu (GraphMenu) and the channel submits run; the channel node-pool mispair that froze the location menu is fixed (see "Freeze in the location menu = Debug Graph") |
| 6 | **Profilers** — BaseProfiler, InGameProfiler, XmlProfiler, GPUProfile, Apt ActionScript profiler, job capture (`kbEnableGPUProfile`, `kbEnableJobCapture`) | stripped classes | pending |
| 7 | **Log to file** — `PrintLogFile` writes `<dir>\MM-DD-YYYY\<name>_HH_MM_SS.log` (output + warnings; zd's month is 0-based) | `Sk8::PrintLogFile` | **done**: game\logs\<date>\output_*.log / warnings_*.log (logfile_debug.cpp; needs allow_game_relative_writes) |
| 8 | **AI path tools** — PathEditor, PathRecording, AutoPathTester, AIPath{Main,Edit,Browsing,Filter,Load,Recording,Skater,Test,TestScenarios}Menu | stripped classes | **mostly**: the in-game "AI Paths" menu (AIPathMainMenu + its sub-menus) opens and every sub-menu opens without faults (2026-10-18); the path drawing / debug info is ported. PathEditor (the menu's interactive tool) is patched out: m's PathManager never builds it |
| 9 | **CAS debug controls** — stamp/colour debug UIs (`Character::StampControl`, `ColourControl`), `CACPrototypeDebugMenu` | stripped classes | **done** (2026-10-18): CASPrototype / CASPrototypeImpl (StampControl, ColourControl, RostralControl, PartSwapControl) and CACPrototypeDebugMenu, via Character Debug Menu > Start CAC Prototype Viewer (see below) |
| 10 | **Anim debug dumper** — `Andale::cAnimDebugDumper` | stripped class | **ported, not triggered**: the blend tree display (zd 83F11D08) fills m's emptied vtable slot; zd itself shows nothing with kDisplayBlendTree alone and its caller is not found yet (2026-10-18 notes) |
| 11 | **Living-world debug** — `LivingWorldAssetDebugger`, `LivingWorldDebugHarness`, `WatchProtectee` | stripped classes | **partly** (2026-10-20, see *Living world*): pedestrian debug draw (17 options) done; LivingWorldAssetDebugger runs but its Render::Debug meshes are not drawn in m yet |
| 12 | **Juice extensions** — list/message readers and writers, RPC plugins, reboot/file-IO plugins, `kJuiceAutomating` | `Juice::*` | **done** (2026-10-19, see *Juice*): zd's 17 extra RPCs, TTY output, Hall of Meat completion, `kJuiceAutomating`; the plugins themselves (reboot, heart monitor, file IO, gamepad, ...) were all still in m; SkateJuice has a *zd debug RPCs* section |
| 13 | **Visual debugger** — streams physics to EA's external PC viewer (`kVisualDebuggerPush*`) | `rw::physics::visualdebugger` (21 classes) | pending; needs the external client, low priority |
| 16 | **Debug menus m dropped** — in-game GameDebugMenu entries "Debug Graph" (GraphMenu), "AI Paths" (AIPathMainMenu + 9 AIPath sub-menus), "Memory Category"; top-level MarqueeViewerMenu, CACPrototypeMenu, SyncErrorMenu (GameModule::SetupInGameDebugMenus 837BDD70 = m 82845A08, stored at GameModule+60/64/68) | menu ctors only in zd | **done** except SyncErrorMenu: "Debug Graph" and "AI Paths" added to m's GameDebugMenu after its ctor; MarqueeViewerMenu and CACPrototypeMenu built into GameModule's extension block and reachable from the Character Debug Menu flows (2026-10-18). SyncErrorMenu needs the multiplayer sync checker (SimController +12) m dropped. "Memory Category" (2026-10-19): appended to m's debug menu as an m cEnumMenuItem (own item id; m uses 3); Print / Print(Verbose) run zd's XAllocator::PrintReport to the TTY/log, Save to file(/Verbose) write `game\data\metrics\memory\xbox2memLog.csv` (test: `--zdx_test_memcat=N`) |
| 15 | **Debug menu options m still shows but that do nothing** — entries in m's Debug Menu / Debug Frontend Menu / Config Variables whose handlers (or the code they toggle) were compiled out; restore zd's behaviour for each | per-entry comparison of the handler in zd vs m (scan: pending) | pending (user-reported) |
| 14 | **Debug state-stack messages** — `cMsgAddDebugBEStateStack`, `cMsgAddDebugFEScreenStack`, `cMsgCheatState`, ... | stripped message classes | **done**: BE state stack (`be_stack_debug.cpp`); zd never sends cMsgAddDebugFEScreenStack; cMsgCheatState is posted from Lua (binding kept by m) |

## Juice (2026-10-19)

`juice_debug.cpp`, zdx_config "Juice" blocks. SkateJuice (`REXGLUE\SkateJuice`)
drives all of it; its Skate tools pane has a *zd debug RPCs* section.

- **zd's RPCs m dropped (17)** are registered after m's own (`Register_Pres` hook) with
  zd's transplanted handlers: Sim `PrintSimFPSMetricsFile`, `PrintMemoryMetricsFile`,
  `PrintLoadTimeMetricsFile`, `SetPhoneListCash`, `SpawnNPC`, `SpawnVehicle`, `SpawnDMO`,
  `SetAutoSpawnVehicle`, `GetNPCLocation`, `HoleDetection`; Pres `PrintPresFPSMetricsFile`,
  `BootUpMarqueeViewer`, `LoadCharacterInMarqueeViewer`, `SetAnimationInMarqueeViewer`,
  `GetShader`, `GetScopes`; System `IsHOMCompleted` (sync). Their messages to the
  sim / pres threads are tagged (+0x1000) in m's senders and handed to zd's controllers.
- The controllers' workers had been call-paired by position with m's (different) cases:
  PrintSimFPS -> m's GetLocalPlayerLocation worker (the "825241141" answer was a location
  string), SpawnNPC/Vehicle/DMO -> GetWorldName/Enable_SCV/..., marquee load/animation ->
  others. All six are in `bad_pairs.tsv`, so zd's workers run. (zdx: two-column lines in
  `bad_pairs.tsv` were never applied, their newline stayed in the m address; fixed.)
- **Metrics files** land in `game\data\metrics\FPSMetrics|memory|LoadTimeMetrics`. The
  memory report (`XAllocator::Internal` zd 83B14538, which m stripped) reads m's allocator
  512 bytes lower (category vector 49588, count 59972, report writer 60008). File paths
  use the version string, which in m is `618274/618259/618097`: the path copy gets `_`
  for `/` (`zdx::GameVersionForPath`), the CSV keeps it.
- **Marquee viewer RPCs**: `BootUpMarqueeViewer` goes through the menu's path (m's flow
  message + request flag, `zdx::RequestMarqueeViewer`). `SetAnimationInMarqueeViewer`:
  zd's worker plays any name, and one the loaded animation db lacks crashed m's animators;
  the Pres controller hook checks the name against `MVManager::GetAvailableAnimationNames`
  (m's viewing state is 8, zd's 7) and answers false for unknown names.
  `LoadCharacterInMarqueeViewer` (slots 0/1) replaces a loaded character. In m that broke
  the *other* slot: animation ids resolve through per-asset id ranges in Andale's
  DataBaseManager, the other animators used ids from a range the unloaded character's
  asset owned, and their next update tripped `DataBaseManager.cpp(296)` (same with zd's
  LoadMarquee). The hook on `MVManager::LoadMarquee` (calls from zd code) unloads every
  loaded slot and loads them all again with the new character in place, so no animator
  outlives its ranges.
- **Spawns** (`SpawnNPC` / `SpawnVehicle` / `SpawnDMO`) work: m's census (`CensusMan::Spawn*`)
  treats a Juice spawn (the "spawned through Juice" flag) as forcing the Juice *model* key,
  which must be a `livingworld_models` record: pedestrians use the entity's name for both
  (`bum01`), vehicles `hatchback01` + `vehicle_hatchback01`. Record lists:
  `data\db\skatercollections_summaryreport.txt` in `big\db.big`. Vehicles also need a
  road under the request (the skater on a street) and room in the traffic pool
  (`SetAutoSpawnVehicle false`). `SpawnDMO`: zd's request passes its two cAssetIDs by
  reference (+96/+100) where m's has them inline (+96/+104) plus three flags (+112);
  the request is rebuilt in m's layout, with m's null locator id (0x833C2368) when zd's
  is unset (a wrong one hit `LWDynamicObjectLocatorMan.cpp(732)`). Its argument is a
  loaded DMO model cAssetID (e.g. `0x0001990A03E38707`); SkateJuice now sends zd's `long`
  parameters as int64 (they were strings, so the id read as 0). On success zd's worker
  answers with the shared model-key buffer (the last spawn's recipe), on failure `-`.
- `GetShader` / `GetScopes` return -1: zd's Pres controller has no case for them either.
- **TTY**: zd's `PrintChannelMan::Internal::Print` sends each line to the Juice TTY
  plugin (`JuiceBase::PrintTTY` -> `TTYPlugin::LogText`, which m dropped; JuiceBase's
  layout is unchanged, TTY plugin at +1880). m's print hook (`logfile_debug.cpp`) now does
  the same; SkateJuice's TTY pane fills while connected.
- **Hall of Meat**: zd's `ChallengeHOMManager::Update` ends a finished run with
  `JuiceRPCNG::SetIsHOMCompleted(true)` (what `IsHOMCompleted` reports and clears); m
  dropped the call. A hook on m's Update makes it when its ending path ran
  (+32 cleared, +44 = 1). Not exercised in a test (needs a finished HOM run).
- **kJuiceAutomating** (zd-only option): while on, zd's `DoRequestProfileName` /
  `DisplayCreateAccount` skip the keyboard / create-account prompt (GATED_REPLACE).
- Checked and present in m already: Reboot / HeartMonitor (hang detection, timeout),
  GamePad, FileIO, Screenshot, Online plugins (only unnamed), the Juice session ID
  overlay, and the music "now playing" telemetry (m 825E5C68 is its `LogToJuice`).
  Not ported: Juice's own `DebugSystem` prints (library diagnostics), zd's
  `cChallengeJuiceManager` (a challenge activity class m does not have, one timer
  setter), `JuiceFileLog` (only used by SyncLog, part of the SyncErrorMenu work).

## Living world (2026-10-20)

- **Pedestrian debug draw** (`Pedestrian::FullFPSDebugDraw`, zd 8421F1A0 = m 83132478):
  `kDisplayPedestrianEntityIds`, `kDisplayPedestrianRecipeNames`, `kDebugNavigationType`,
  `kDebugNavigation`, `kDebugNavModifiers`, `kDebugAITimers`, `kDebugIntentMap`,
  `kDebugShowIntentFlags`, `kShowPositions`, `kShowMoodSummary`, `kShowMoodDetails`,
  `kShowMoodProbTimers`, `kShowIdealChasePositions`, `kShowRecommendedChasePositions`,
  `kShowExhaustion`, `kShowWantsTable` run as blocks after m's function, plus
  `kShowJumpEvents` (IntentGen::GetNavTargetDirection). Like the other per-object
  living-world draws they only run through LWManager::FullFPSDebugDraw's call block, so
  they are its gates too. Their mood / timer / intent data is
  `LivingWorld::tPedestrianStateStorage` (zd Pedestrian+6648 = m +5796), mapped from its 26
  paired methods: same up to +592, +16 after (exhaustion read NaN and the intent map
  blacked the game out before that map). Tested downtown with pedestrians spawned next to
  the skater (Juice SpawnNPC): ids, recipe names, AI timers, moods, wants, exhaustion,
  nav-modifier states and positions draw.
  Not ported: `kShowBestNavDirection`, `kDebugRorSpring_ShowKDResults`,
  `kDebugDMOAvoidanceNavModifier_ShowSituations` (they need zd's whole gameplay
  function, which reads other objects' fields m moved); the nav-rig views
  (`kDebugNavRigs`, `kNavRig*_Show*`; m's NavRig member is still unknown).
- **LivingWorldAssetDebugger** (`kWorldPainterDebugger*`, `kShowNavMesh`): zd's debugger
  (LivingWorldPresentation+25228 = m +25360) runs on m's object, with zd's extra
  CharacterEntity map in an extension block that `livingworld_debug.cpp` constructs (a
  zero-filled eastl map made DebugDraw's per-frame clear loop spin, so the world never
  finished loading). It runs safely now, but its world-painter / nav-mesh views are built as
  `Render::Debug::LineList` / `GoraudMesh` GPU primitives, and nothing in m renders those
  yet: nothing shows.
- zdx: fragments' indirect calls passed the fragment name as the zd address (build error);
  they pass their zd function's address now.

## zd-only debug cvars by system

Generated from the names present in zd's image and absent from m's.

215 zd-only debug cvars (of 384 zd-only cvar-like names; the rest are enums or tuning values).

### Traffic (vehicle AI) debug draw/text (33)

`kDebugNavRigs`, `kDisplayVehicleEntityIds`, `kDisplayVehicleGrabSplines`, `kDisplayVehicleGrindSplines`, `kDisplayVehicleRecipeNames`, `kDrawVehicleBoundingBoxes`, `kShowVehicleNavRigValidator`, `kTrafficDebugTextCullRadius`, `kTrafficDrawAxes`, `kTrafficDrawCorneringSpheres`, `kTrafficDrawFidelity`, `kTrafficDrawForcedStopping`, `kTrafficDrawHornSpheres`, `kTrafficDrawLaneChangeCurve`, `kTrafficDrawPreStateUpdateTests`, `kTrafficDrawSkiddingSpheres`, `kTrafficDrawSkitchHelperVision`, `kTrafficDrawVehicleLights`, `kTrafficDrawVision`, `kTrafficShowAcceleration`, `kTrafficShowActionDecision`, `kTrafficShowCornering`, `kTrafficShowDebugText`, `kTrafficShowDimensions`, `kTrafficShowHornState`, `kTrafficShowID`, `kTrafficShowParkingInfo`, `kTrafficShowSpeed`, `kTrafficShowState`, `kTrafficShowStoppingReason`, `kTrafficShowTimers`, `kTrafficShowWheelAngles`, `kTrafficVehicleRemovalDebugTests`

### Living world / pedestrians / Big Black (19)

`kAISkaterAssetDebug_X`, `kAISkaterAssetDebug_Y`, `kDebugBigBlackAvoidanceNavModifier_AllowHandleAvoidBigBlack`, `kDebugBigBlackAvoidanceNavModifier_ShowBigBlackSituations`, `kDebugBigBlackAvoidanceNavModifier_ShowSituations`, `kDebugBigBlackAvoidanceNavModifier_maxTurnAngle`, `kDebugShowChaseAudioState`, `kDebugSpawn_HandPropType`, `kDrawEntityOnRoadPoints`, `kDrawPedestrianBoundingBoxes`, `kNumBigBlackPatrolPoints`, `kShowBigBlackPatrolPoints`, `kShowBigBlackRegions`, `kShowBigBlackTakeDownTarget`, `kShowIdealBigBlackPositions`, `kShowOnRoadWander`, `kVisualDebuggerPushPedestrianPool`, `kbEnableAISkaterAssetDebug`, `kbEnableAISkaterUsableDebug`

### Nav-mesh / AI path tools (39)

`kAutoTestFirstPathIndex`, `kDebugAStarInstance`, `kDebugAStarInstanceDepth`, `kDebugFillRadius`, `kDrawCurrentNavCell`, `kDrawPathEndConnections`, `kDrawPathWidths`, `kFilledNavmeshCellAlpha`, `kNavMeshAnalyzer`, `kNavMeshAnalyzerDetailed`, `kNavMeshAnalyzerDetailedShowInvalidCells`, `kNavMeshConnectivityType`, `kNavTileEdgeAnalyzer`, `kNavmeshCellsFilledWithScoreOffsetY`, `kNavmeshConnectivityOffsetY`, `kNavmeshEdgeOffsetY`, `kOverridePathDrawCol`, `kOverridePathDrawColAlpha`, `kOverridePathDrawColBlue`, `kOverridePathDrawColGreen`, `kOverridePathDrawColRed`, `kPathOpacity`, `kPathStatisticsDrawX`, `kPathStatisticsDrawY`, `kPathStatisticsWidth1`, `kPathStatisticsWidth2`, `kShowAvoidNavMeshChecks`, `kShowClearPaths`, `kShowFullyBlockedPaths`, `kShowNavMesh`, `kShowNavMeshCellsFilledWithScore`, `kShowNavMeshConnectivity`, `kShowNavMeshFollowCamera`, `kShowNavMeshScoreLines`, `kShowPartiallyBlockedPaths`, `kbDrawBreadCrumbs`, `kbEnableNavMeshConstraints`, `kbIsPathStatisticsCollectionEnabled`, `kbStopOnBoardCrumbs`

### Visual debugger (remote PC physics viewer) (9)

`kVisualDebuggerJobProfiling`, `kVisualDebuggerPushContacts`, `kVisualDebuggerPushDmoPool`, `kVisualDebuggerPushDrives`, `kVisualDebuggerPushGroundPool`, `kVisualDebuggerPushJoints`, `kVisualDebuggerPushSkaterPool`, `kVisualDebuggerPushSpies`, `kVisualDebuggerPushVehiclePool`

### Debug overlay / warnings / graphs (6)

`kDebugDrawMode`, `kDebugShapeMemorySize`, `kDrawLineDebug`, `kbEnableDebugGraphXY`, `kbEnableDebugOverlay`, `kbWarnDebugOverlayOOM`

### Profiling / capture / metrics (13)

`kArenaStatsDisplayMode`, `kArenaStatsVerbose`, `kDrawAllBeStats`, `kDrawAllDMOStats`, `kDrawAllPersonalBests`, `kDrawStatistics`, `kListTrickMetrics`, `kListTrickTypesInSequence`, `kListTricksInLine`, `kListTricksInSequence`, `kShowStreamMetrics`, `kbEnableGPUProfile`, `kbEnableJobCapture`

### Streaming / assets (13)

`kDrawTerrainTransforms`, `kDumpTerrainModelInfo`, `kEnableRefreshAssetInfo`, `kShowPresLoadedBoxes`, `kShowPresUnloadedBoxes`, `kShowSimLoadedBoxes`, `kShowSimUnloadedBoxes`, `kShowTextureLoadedBoxes`, `kShowTextureUnloadedBoxes`, `kWorldScriptStartDMOTest`, `kWorldScriptStartVehicleTest`, `kWorldScriptVehicleSpeed`, `kbUseFEBigFile`

### Skater / physics / IK debug draw (59)

`kCommunitySkatersTestFallbackAlreadyUsed`, `kCommunitySkatersTestPrimaryAlreadyUsed`, `kCommunitySkatersTestUseActualFriends`, `kDrawBigTrickInfo`, `kDrawBoundingBoxForSelectedPath`, `kDrawContactSpies`, `kDrawContextInfoForAir`, `kDrawContextInfoForNow`, `kDrawCursor`, `kDrawDMOPoolInertiaTensors`, `kDrawDmoBoundingBoxes`, `kDrawFeetOnDeckIK`, `kDrawIKBlendValues`, `kDrawPostFeetIK`, `kDrawPostPhysIK`, `kDrawPreFeetIK`, `kDrawSkaterBoundingBoxes`, `kDrawTrajectoryInfo`, `kEnableOffBoardCollectHookToHipsStats`, `kEnableOffBoardConsoleOutputHookToHipsStats`, `kEnableOffBoardDebugDrawHookToHipsStats`, `kPhysicalPlayerDebugLocationX`, `kPhysicalPlayerDebugLocationY`, `kPhysicalPlayerTrajDebugLocationY`, `kRenderAudioSpheres`, `kScoring2DebugDraw`, `kScoring2DebugDrawLocalOnly`, `kScoring2DebugDrawShowGroundInfo`, `kScoringDrawPosX`, `kScoringDrawPosY`, `kScoringShowPumpInfo`, `kShadowMode`, `kShowBodyTiltInfo`, `kShowCoffinInfo`, `kShowLastTeleportLoadScreenTestPoints`, `kShowPoses`, `kShowSelectionSpace`, `kShowSkateboardCollision`, `kShowSunDirectionMap`, `kShowSurfaceType`, `kShowSurfaceTypeLocationX`, `kShowSurfaceTypeLocationY`, `kShowWheelPositions`, `kSkaterOpacity`, `kTransparentSkaters`, `kWipeoutScoringVisualDirectorDebugDraw`, `kbDebugHelperBones`, `kbDisableCameraCollisionWithEverything`, `kbDistanceTrackerDebug`, `kbDrawBoundaries`, `kbDrawCSM`, `kbDrawCaCSnapshot`, `kbDrawCollisionTest`, `kbDrawGroundNormalTris`, `kbShowClipDistance`, `kbShowCollision`, `kbShowDynamicUp`, `kbShowGroundNormal`, `kbShowUpVector`

### Front end / online debug (11)

`kCreateASpotDoVersionChecking`, `kCreateASpotUseUncompressedThumbnail`, `kDebugFEGotoWebKit`, `kDisableAutoSave`, `kFakeLowHDD`, `kUseDebugCompetitorData`, `kbDebugDLCRequired`, `kbFEShowOnline`, `kbForceFEBlur`, `kbHideDLCIPs`, `kbHideDLCPros`

### Juice / live tuning / replay tests (8)

`kAssertOnOverwrite`, `kEnable_LiveDev`, `kJuiceAutomating`, `kTestReplayEvent`, `kTestReplayTrigger`, `kValidateInput`, `kValidateOutput`, `kValidateWriteLocation`

### Other debug (5)

`kCullAllButSelectedEntity`, `kCullOptimeshIslands`, `kDrawRAINDataBasedOnID`, `kDrawRAINDataOfID`, `kUseWeightCutOff`


## Bulk port status (debug-menu options m kept but compiled out)

Generated tables in `sk83_na_zd/tools/`: `zdx_blocks.py` (BLOCKS / PENDING)
and `zdx_callblocks.py` (CALLBLOCKS / CALL_PENDING). Every ported option is
tested alone in the park (alive at 50 s with a normal frame, scratchpad
`verify.sh`). Pending reasons, from the zdx report:

* **unverified fields** -- the struct layout of `this` (or of an object the
  block uses) is not proven for the fields the block reads; resolved per
  class with votes (`field_votes.py`) or live objects (`class_dump_align.py`);
* **values from calls** -- the block prints values gameplay computed from
  call results before it (can't be recomputed after m's function);
* **gameplay entries** -- the zd function holding the debug output is a
  gameplay function m inlined (calling it again would run gameplay twice):
  the output needs a block port inside it;
* **no debug switch** -- the zd function only reads numeric options (or
  options on by default), so nothing gates a second call;
* **runtime** -- crashes / black frame when enabled (e.g. the BE tricks
  table: hundreds of text lines per frame);
* zd's caller log (`sk83_na_zd/src/sk83_na_zd_callers.cpp`, TEMP) gives the
  real call sites of stripped functions, incl. virtual calls m's vtables
  dropped (e.g. PathController::Draw, slot 9).

## Item 15: debug-menu options that do nothing in sk83_na_m

371 options sk83_na_m registers (they appear in its debug menus / Config Variables) that no m gameplay code reads; zd reads them. Found by `sk83_na_zd/tools/cvar_consumers.py`.

#### Debug draw / print / display toggles (222) - need the DebugOverlay (item 1)

| option | zd readers |
|---|---|
| `kAIPathManagerDepthStencilState` | 834F4178 |
| `kAnimScoreDebug` | 83644CA0 83648540 836495C0 |
| `kApplyAntiFLipTOrqueTest` | 840C7FA0 |
| `kAreAmbientSkatersDrawnTowardsPlayer` | 8350DB18 |
| `kAttachCensusToCamera` | 83763AD8 83763BA0 837813F0 83781500 |
| `kBipedAir_DebugDraw` | 840C3958 |
| `kBiped_DebugDraw` | 8412ADB0 |
| `kBiped_DebugDrawLanding` | 8412B248 |
| `kBiped_DrawCadence` | 84134DE8 84134EA0 |
| `kBiped_DrawVelocity` | 841333B0 |
| `kBiped_IntentsDebug` | 8412F1F8 |
| `kBoardMountTestAngularThreshold` | 84170320 |
| `kBoundaryTest1` | 841EED40 |
| `kBoundaryTest2` | 841EED40 |
| `kBoundaryTest3` | 841EED40 |
| `kCameraPositionDebugEnabled` | 83991780 |
| `kCrowdPresDebugCrowd` | 83879198 83893BD8 |
| `kCrowdPresDebugImposterCount` | 8387C428 838934A0 |
| `kCurrentCollisionPrint` | 83556768 8357C268 |
| `kDMOCollisionOutput` | 83561440 8357E900 |
| `kDebugChaseMan` | 83772398 |
| `kDebugDMOAvoidanceNavModifier_ShowSituations` | 84215BC0 84236598 |
| `kDebugDefaultLocationTest` | 834F8CC8 |
| `kDebugDisplayBodySpinGraphs` | 8411A9F0 |
| `kDebugDrawLaneChangeCurveResolution` | 841DC900 |
| `kDebugDrawOffBoardFeetIK` | 8411B940 8411CD88 8411FA58 |
| `kDebugDrawPedNavDataNextRoad2` | 841EB410 |
| `kDebugDrawPedNavDataPrevCrosswalk2` | 841EED40 |
| `kDebugDrawPedNavData_Reverse` | 841D5DF0 |
| `kDebugDrawRoadCurveResolution` | 841D5E08 |
| `kDebugDrawRoadCurves` | 841E8E58 |
| `kDebugDrawWaypoint` | 841F4C18 |
| `kDebugGeneralNavModifier_ZeroVelocityThreshold` | 84213A58 8421D0B8 |
| `kDebugHoMEnabledSetting` | 8384B8D0 |
| `kDebugHoMEnabledSettingTextY` | 8384EDA0 |
| `kDebugIntentMap` | 841FFF70 |
| `kDebugNavModifiers` | 84225748 |
| `kDebugNavRigDistanceChecks` | 841DA3C8 841DA8D0 |
| `kDebugSkaterAvoidanceNavModifier_MinVelocityForBigAvoid` | 84233CD0 |
| `kDebugSkaterAvoidanceNavModifier_PredictTime` | 842076F0 |
| `kDebugSkaterVelocityExceeds` | 83780D38 |
| `kDebugTargetingActions` | 84207F60 |
| `kDisplayAiSkaterManagerDebugEnabled` | 834EC638 834F6738 |
| `kDisplayAiSkaterManagerDebugY` | 834DD7D8 834DE4A0 |
| `kDisplayGestureDebug` | 8373FA40 |
| `kDoActorToNodeTest` | 834D7B18 |
| `kDrawActualVisibilityIndicator` | 8423DDF0 |
| `kDrawBeTricksDebug_X` | 835C5890 |
| `kDrawChallengeCrowdPositions` | 840E3918 |
| `kDrawChallengeGeneratorVolumes` | 840E3918 840F5E30 |
| `kDrawCollidingPathNodeLineTests` | 834D3678 |
| `kDrawContactInfo` | 83F32F68 |
| `kDrawDynamicObjectAttribNames` | 83FC0C08 |
| `kDrawDynamicObjectGrabSplineApproachVectors` | 83FC4768 |
| `kDrawFilledBoundingVolumes` | 834E0958 |
| `kDrawGrabbableObjectGrabSplineNormals` | 841DAE58 841E1F68 |
| `kDrawGrindGeometryTest` | 83F8F8C0 |
| `kDrawGrindPairDetection` | 83F8BE88 |
| `kDrawGrip` | 8411B5A0 |
| `kDrawHeadPitchTarget` | 84213C90 |
| `kDrawHeadYawTarget` | 84213C90 |
| `kDrawHighDetailRecordedPath` | 838655D8 |
| `kDrawIKContactDebug` | 83F315D0 83F31A90 83F38448 |
| `kDrawPathsForMinSkaterCount` | 8350C708 |
| `kDrawRAINTargetsBasedOnID` | 8420C4A8 |
| `kDrawRAINTargetsOfID` | 841FFBF0 8420C4A8 |
| `kDrawRWACProfile` | 83534680 |
| `kDrawSBJoints` | 840D4D30 |
| `kEnableHandGrabOutput` | 835871F0 835873C8 |
| `kEnableLandCollisionOutput` | 83561440 83588C20 |
| `kFEHudDebugOut` | 8354D140 |
| `kForceExitTestDown` | 8412A2E0 |
| `kGodHandCameraDebugDraw` | 8398E568 83991780 |
| `kGodHandCameraLOSDebugDraw` | 8398DA68 |
| `kGodHandCursorYOffsetLineTestLength` | 83983F88 839841E8 839891E8 8398DA68 |
| `kGrindPrimativeDebugYOffset` | 83F8C8C0 |
| `kHPS_DrawPrimitives` | 840FF978 |
| `kInsideVehicleTest_YOffset` | 837A0630 |
| `kInsideVehicleTest_YSize` | 83763708 |
| `kLuaStateGraphVerbose` | 840849F8 84084CC8 840855E8 840856B0 |
| `kLuaStateManagerTraceLuaIndex` | 8414B3D0 8414B5F8 8414B720 8414D2B0 |
| `kMaxLengthTestLeg` | 83F31B58 |
| `kMaxNavRigTopologyTestDistance` | 841FFBF0 84224A28 |
| `kMoodEventDrawTime` | 842073C8 |
| `kNavRigDownTestHeight` | 841FFF70 |
| `kNavRigJump_ShowFatness` | 841DAE58 |
| `kNavRigPhysics_ShowFatness` | 84224CD0 |
| `kNavRigVolume_ShowFatness` | 84225748 |
| `kNavRigVolume_ShowVolumes` | 842250C0 |
| `kNodeWidthLineTestStartYOffset` | 834F4178 |
| `kNumBatchTests` | 8384D7B8 |
| `kOBGA_DrawAnalysisScl` | 8412D110 |
| `kOBGA_DrawAnalysisX` | 8412D110 |
| `kOBGA_DrawAnalysisY` | 8412D110 |
| `kOBGA_DrawExtraFeatures` | 84128EC8 |
| `kOBPushingDebugDrawRawHandTargets` | 840CED88 |
| `kOBPushingDebugDrawSplineBlends` | 840CD510 |
| `kOffBoardDrawInteractableObjectLineTests` | 8384D7B8 |
| `kOffBoardInteractableObjectLineTestDistance` | 8384EBF0 |
| `kOffBoardInteractableObjectLineTestFatness` | 8384D7B8 |
| `kOffBoardInteractableObjectLineTestsMaxHeight` | 8384D7B8 |
| `kPathControllerDebugDrawTextX2` | 834E4B08 |
| `kPedestrianCensusOverride` | 837762E8 8377CB78 |
| `kPhysicsDoPhysicsTests` | 84224850 |
| `kPhysicsDoSpatialInstanceTests` | 84224A28 8423C398 |
| `kPhysicsDoVolumeTests` | 8420E5C8 842250C0 842260E0 |
| `kPhysicsLineTestsLength` | 84236948 |
| `kPhysicsTestFatness` | 841D5E20 |
| `kPrimitiveRenderRadius` | 83F8BE88 |
| `kPrintPathSpam` | 841EACD0 841F1C60 |
| `kPrintRelativeVel` | 83566F30 83587710 83588608 |
| `kRegeneratePathDataOnAutoTest` | 8350C708 8350DDA8 |
| `kRenderAssertOnDupAssets` | 83883FC0 83884F78 83886818 |
| `kRenderDebugCharacterLighting` | 8389D048 8389EC40 |
| `kRenderDebugCharacterTexture` | 8389EC40 838A5D60 |
| `kReverbDebugOut` | 83556B18 83594A48 |
| `kRollingSurfaceDebugOut` | 83560CF0 83594728 |
| `kScaleGraph1` | 8413D110 |
| `kScaleGraph2` | 84130A50 |
| `kScoringShowDMODebug` | 8415A960 8415AC98 |
| `kScoringShowOtherSkaterDebug` | 8415B720 |
| `kScoringShowVehicleDebug` | 8415A528 |
| `kSelectedDebugPlayerID` | 834E3348 834FA138 |
| `kShowAIControllerDebug` | 834C7C00 834ECE50 |
| `kShowAStar` | 84215110 |
| `kShowAStarAnalysis` | 842095A0 8420C4A8 |
| `kShowActiveSignUpChallenge` | 840E2890 |
| `kShowAiSkaterIntents` | 834FA138 |
| `kShowAiSkaterOffboardIntents` | 834C8D10 834FA138 |
| `kShowAllSignupChallenges` | 840E26A8 840E2890 |
| `kShowBEPlayerData` | 834EC638 |
| `kShowBEPlayerDataX_Width4` | 834E0958 |
| `kShowBodySlideSurface` | 8357AA08 |
| `kShowChallengeGameMode` | 835FDE20 |
| `kShowCharacterPositionLineTests` | 836053A0 |
| `kShowCollisionSliding` | 8420BDA0 84236230 |
| `kShowCrossWalk` | 8378B020 |
| `kShowFleeTargetIterator` | 8420CBB8 |
| `kShowGlobalLocationFiles` | 83AA13A8 83AA1498 83AA21A0 83AA2228 |
| `kShowGrindGUIDs` | 8415D028 |
| `kShowGrindPrimativesGuids` | 83F8BE88 |
| `kShowGrindSurface` | 835561C8 83556FA0 |
| `kShowGroundPosNormal` | 8423BC28 |
| `kShowHandPropTransforms` | 84211198 |
| `kShowHeadTrackingTargetingMode` | 8420D618 84213A58 8421D0B8 |
| `kShowLastKnownGoodGroundPosition` | 84213C90 |
| `kShowLinearAngularForceMagnitude` | 84213C90 84233CD0 |
| `kShowLoadedDynamicHulls` | 840E26A8 840F1B28 |
| `kShowLocationStartPoints` | 83AA1498 |
| `kShowLockedStreamerPosition` | 840E0DF0 840E2890 |
| `kShowManualOutTimer` | 83EDBCD0 |
| `kShowMissingGlobalLocatorChallenges` | 840E2890 840F5B38 |
| `kShowNearestIntersection` | 842076F0 |
| `kShowNoSimPresAnimationSkeletonLW` | 838CA008 |
| `kShowPathCostThreshold` | 842095A0 |
| `kShowPathEdgeIntersections` | 841EB410 841EED40 |
| `kShowPathEdges` | 841EED40 |
| `kShowPedSpawnDMOChecks` | 842095A0 |
| `kShowPhotographerTargetInfo` | 83773C50 8378BA38 |
| `kShowPowerSlideSurface` | 83555FB0 835576A8 |
| `kShowPreloadVolumes` | 8418F7E0 |
| `kShowPreloadedNIS` | 8418F7E0 |
| `kShowRelativeObstacleVelocity` | 834E0958 834FB2A8 |
| `kShowRewardManagerStats` | 83F880A8 83F88288 |
| `kShowSignUpMarker` | 840E2890 840E3918 |
| `kShowSignUpVolumes` | 840E2890 |
| `kShowStaticCameraDebug` | 835CD680 |
| `kShowStreamedAssets` | 83F8C8C0 83F90E90 |
| `kShowTBNState` | 8389EC40 |
| `kShowTakeDownDebug` | 842250C0 |
| `kSkeletonDebugDrawTorqueForces` | 83F45428 |
| `kSkeletonDisplayAnimToWorldMtx` | 83F46D88 83F48968 |
| `kSpatialInstanceLineTestWidth` | 8416A9D8 |
| `kTestRaceTrigger` | 83542A98 83542C50 |
| `kTestReverb` | 83560CF0 8357C268 |
| `kTestSetToLoadOnStartup` | 834C2A98 8350DB18 |
| `kTestWipeout` | 83F006E8 |
| `kUseDebugImpactForce` | 83556498 83569EE8 8356B3F8 |
| `kUseSnappingBBoxTest` | 8398FEB0 |
| `kUseTestMotionGraph` | 84236948 |
| `kWipeoutDisplayDeckAcceleration` | 84131C88 |
| `kWipeoutScoringDebugDrawList` | 8415DEE8 |
| `kWipeoutScoringDebugDrawSkeleton3D` | 8415A960 8415AC98 8415DEE8 |
| `kWorldPainterDebugger` | 838BF4B0 838C80B8 |
| `kWorldPainterDebuggerLayer` | 838C6BE0 838C80B8 |
| `kWorldPainterDebuggerLayerY` | 838C6BE0 |
| `kbAutoProfilingRenderThreshold` | 837B1370 |
| `kbDrawLockedPaths` | 834D6BF8 |
| `kbDrawLockedVolumes` | 834F5BE8 |
| `kbDrawLockedVolumesUsed` | 834D7DD0 834E14A8 |
| `kbDrawOffboardInputDebug` | 836495C0 |
| `kbDrawStreetPaths` | 834C77F0 |
| `kbDrawToggleablePathsOnly` | 8350C708 8350DDA8 |
| `kbDrawVisCurtains` | 83629C38 83629FE0 8362A360 8362A5E8 |
| `kbEnableAutoRenderProfiling` | 837B56C8 |
| `kbEnableHUD1Draw` | 8389D048 |
| `kbEnableSpatialInstanceDebugDrawText` | 8416A9D8 |
| `kbEnableVisualDebugger` | 8384B8D0 8384EE30 |
| `kbIsAiDebugInfoEnabledForAllActors` | 834D3678 834E3348 |
| `kbIsAiPathDebugInfoEnabled` | 834E3348 |
| `kbIsAiPathDrawingEnabled` | 834C2A98 83500F98 |
| `kbIsFullAiPathDebugInfoEnabled` | 834D42F0 834F5100 |
| `kbLandingOnDeck_DebugDrawTraj` | 8411C818 |
| `kbProfileLoading` | 837B56C8 |
| `kbRenderEnableWorldSHProbeStreaming` | 8390D618 8391EE00 839203B0 |
| `kbShowAirCollision` | 841CB668 |
| `kbShowAllPathsInFilter` | 834D7B18 |
| `kbShowCompassDebug` | 841B73E8 841BABA8 |
| `kbShowConditionValues` | 841CB380 |
| `kbShowFourThreeLimits` | 841C4220 |
| `kbShowFresnelLookupTexture` | 8391EE00 839203B0 |
| `kbShowMedicalBill` | 83684B90 |
| `kbShowOwnershipBoxes` | 83634C08 |
| `kbShowParameterValuesCurrent` | 841C4300 |
| `kbShowParameterValuesPrevious` | 841C3098 841C9EC8 |
| `kbShowReferencePoint` | 841C4300 841CB668 |
| `kbShowRigReferenceHeight` | 841CA700 |
| `kbShowShotChanges` | 841CB380 |
| `kbShowShotHeading` | 841C5E00 |
| `kbShowTransitionTimeDebug` | 841C7EB8 |
| `kbSpatialInstanceDebugDrawLineTests` | 8416A6E8 8416B160 |
| `kbVerboseDMOPrints` | 836321E0 83633FA0 83634F80 8363C578 |

#### Tuning values and switches (149) - m's code no longer reads them; restore the read at zd's use site

| option | zd readers |
|---|---|
| `kAmbientAIWeightsProSkaterBonus` | 834D6BF8 |
| `kBC_HardDriveStrOuter` | 83F459B0 |
| `kBiped_MaxVelDeltaDismount` | 84122180 8412D590 |
| `kBiped_NumTrajectories` | 84138770 |
| `kBodySpinInputBlend` | 8411F840 |
| `kBoundaryMinWarningAmount` | 840E0DF0 |
| `kCheckForJumpNavMesh` | 84200CD0 |
| `kCollisionFrameDecay` | 83585A18 |
| `kCollisionPartAngularDamping` | 83F1E6B8 |
| `kCollisionPushFraction` | 83F1E6B8 |
| `kCompletedObjectivesTier` | 83FDB3F0 83FDB428 83FDE768 |
| `kContainedWaypointYTolerance` | 83FC3DA0 |
| `kCopingExitStartTime` | 8412D590 |
| `kDS_PrepareJumpClearance` | 840D7800 |
| `kDS_PrepareJumpForce` | 840CD510 |
| `kDS_TruckAvoidForce` | 840D69F0 |
| `kDefaultNavMeshCheckYTolerance` | 8420E5C8 84211198 842260E0 |
| `kDemoTimeoutSeconds` | 837E6948 |
| `kDistanceFromChaseStartExceeds` | 83763250 |
| `kDoFeetPostPhysics` | 83F45C98 |
| `kDoPathValidation` | 84226B68 |
| `kDoPhysicsCaching` | 841DAE58 |
| `kDropShadowXOffset` | 8367BA40 |
| `kEnableAutoPathPreprocessing` | 83504EF0 8350C0D8 8350C298 8350C9D8 |
| `kEnableDisconnectShotcut` | 837C3278 |
| `kEnableNoPathFoundWarning` | 834EFD48 |
| `kEnableObjectResetShortcut` | 837BE700 |
| `kEnableOldAttribCharactersticMethodAsFallback` | 83FCBE58 83FD2300 |
| `kEnableWobbleTakeoff` | 83F1E6B8 |
| `kExcludeSkaterTrick` | 8421D0B8 |
| `kFineGeometryDist` | 83F8BE88 |
| `kGrindFriction_Rough` | 840DCFD0 |
| `kGrindFriction_Slow` | 840CC868 |
| `kGrindTrickVelScalar` | 840BA500 |
| `kGroundToBoardslideFrames` | 8412F1F8 |
| `kHandPlantGroundNormalY` | 8410D4C0 |
| `kHandPropDisownDistanceZ` | 84207F60 |
| `kHandPropThrowAngleTolerance` | 8420C4A8 |
| `kHandlerVerbosityLevel` | 84084888 84086438 84086730 |
| `kHeadTrackingTargetingOverrideMode` | 8420E420 8421D0B8 |
| `kIgnoreLOSTimerForZoomIn` | 83983298 8398DA68 8398E250 |
| `kIsAISkaterAvoidanceOfDMOsEnabled` | 834E6EB8 83508F30 |
| `kJitterToSpew` | 83582038 83585A18 |
| `kJumpDownLedgeDifference` | 841D5B50 |
| `kJumpLedgeFailureDistance` | 84226B68 |
| `kLOD_TimeForLandingWipeOut` | 840C7FA0 |
| `kLOD_TrajError` | 8411B5A0 |
| `kLandingImpactMaxBlendDelta` | 83ED04C8 |
| `kLandingTimeToBlendToTwoFoot` | 83F006E8 |
| `kLookAheadAvoidanceFrameAmount` | 834DE4A0 |
| `kMOSpeedMaxDelta` | 83EF2EF8 |
| `kMOVOBJ_ForceSmoothing` | 840B8420 |
| `kMakeSkateboardBad` | 84174378 |
| `kMakeSkateboardBadVel` | 84174378 |
| `kMarkMarqueePedestrianFace` | 838C3E90 838CA008 |
| `kMaxAutoSpinMag` | 84120AF8 |
| `kMaxDistanceFromLockedStreamPosition` | 8380C838 |
| `kMaxLandingError` | 8411FC20 |
| `kMaxLandingErrorAI` | 8411FC20 |
| `kMaxNavRigTopologyAngle` | 841FFBF0 84224850 |
| `kMaxPairDistY` | 83F8C8C0 |
| `kMaxSkeletonDisp` | 83F45428 |
| `kMinDarkslideDeckTilt` | 8412F1F8 |
| `kMinPairDist` | 83F8C8C0 83F90E90 |
| `kMinSpeedForApproachingTrick` | 834CD690 |
| `kMinSpeedWeightIgnore` | 84131F80 |
| `kNISLightRigHacks` | 83725130 837328C0 838A5D60 838A6050 |
| `kNavRigDistance` | 841DAE58 841E1F68 |
| `kNavRigJumpForwardDistance` | 841D5E08 |
| `kNavRigJumpHeightDistance` | 841D5E20 |
| `kNavRigJumpUpHeightMaxHeight` | 84222B00 |
| `kNavRigJumpUpHeightMinHeight` | 841FF238 84215908 84229FC8 |
| `kNavRigOrientToWorldTopology` | 84226D28 |
| `kNavRigPhysicsCacheDistance` | 841DAE58 |
| `kNavRigPhysicsDistance` | 841D5DF0 841DA3C8 841DA8D0 |
| `kNavRigPhysicsLineFatness` | 8420D618 |
| `kNavRigVolumeLineFatness` | 8420E420 |
| `kNavRigVolumeLineYClearance` | 8420E420 |
| `kNavigationTargetingMode` | 84215110 |
| `kNoRoadWanderCloseDistance` | 842250C0 |
| `kNumFramesStuckBeforeWipeoutNavPath` | 83504EF0 83508F30 8350C0D8 8350C298 |
| `kNumNodesToAdvanceForFutureNode` | 834E91B0 |
| `kNumSkaterTrajectoryBatches` | 8384D7B8 |
| `kOBGA_MinStairDeg` | 84139498 |
| `kOBIK_FramesToWipeoutForFeetNotDown` | 8411FC20 |
| `kOBIK_IKMaxDist` | 84113170 8411C818 |
| `kOBIK_IKMaxYDistUp` | 84113170 |
| `kOBIK_MinTrajValue` | 8411D620 |
| `kOBTraj_LandingNormalScalar` | 8411A9F0 |
| `kOffMeshAnchorArrivalRadius` | 8423CDC8 |
| `kOffboardMinFrameDistanceForNotStuck` | 834C7C00 |
| `kOnlyRegenerateChangedPaths` | 8350C708 |
| `kOverrideTransitionTime` | 835CE4C0 |
| `kPS_UseNormalsForProxyOffset` | 83F31DC0 83F43048 |
| `kPedestrianPoolUpdateInParallel` | 8385AC18 |
| `kPlayBodySweeteners` | 835664A8 83585C90 |
| `kPrepareJumpClearance` | 840CC278 |
| `kPullbackSpeed` | 841FFD28 |
| `kReallyFarFromPathCenterDistance` | 834E2720 |
| `kRunDmoVehicleCollisionPipeline` | 83854BF0 |
| `kRunUpdate1InParallel` | 8384EBF0 |
| `kRunUpdate2InParallel` | 8384EC80 |
| `kRunUpdate3InParallel` | 83750488 8384E118 8384ED10 |
| `kRunUpdate4InParallel` | 8384ED10 8384EDA0 |
| `kSelectObjectivesTier` | 83FDB3B8 |
| `kSelectedLWGestureType` | 83535440 835355A8 |
| `kSignificantSpeedupFractionTrickApproach` | 834F3160 |
| `kSimSampleMask` | 837B0A60 837C3278 |
| `kSimSpamDMOWarning` | 83974540 |
| `kSkaterIntentsX` | 8364D3A0 |
| `kSkaterNavmeshHeightOffset` | 834E14A8 |
| `kSkeleton_ProxyInnerRadius` | 83F2FC48 |
| `kSkillCacheTimeout` | 835A40A8 835A9C78 835A9D10 |
| `kSkitchMaxHorizontalEntryAngle` | 840B56B0 |
| `kSnapToGroundBatchCount` | 839859B0 8398A430 |
| `kSpamScreenWithDMOWarnings` | 83FCBE58 83FD2300 |
| `kSpawnPedDMOPadding` | 84204820 |
| `kSyncCheckStagesMultiPlayer` | 8384E118 |
| `kTakeDown_DistX` | 84128EC8 |
| `kTargetAId` | 83763250 83786B78 83786D40 |
| `kTargetBId` | 83786D40 |
| `kTruckGrindControlClampedSmoothness` | 841398A0 |
| `kTruckGrindControlSmoothness` | 8412D110 |
| `kUpdateMassesEveryFrame` | 841E8798 |
| `kUseBoardDirectionEvenWhenGettingBackToPath` | 834CF8F8 834E2EC8 |
| `kUseBoneAccelerationForForce` | 841522C0 |
| `kUseProperPhysBoardBackwards` | 83F11D08 |
| `kVehicleFleeDistance` | 84226D28 |
| `kWipeControlMaxAngleRotY` | 840D0258 840D03A8 840D2340 840D2758 |
| `kWipeoutForBigAirs` | 841398A0 |
| `kWipeoutLeaningContactEnabled` | 8412F1F8 |
| `kWipeoutSkeletonContactEnabled` | 8412ADB0 |
| `kYMinHeightToBeOffGround` | 84113170 8411E968 |
| `kbAutoProfilingSimThreshold` | 837B12C8 |
| `kbAutoWidthGeneration` | 834D3EC0 83500F98 |
| `kbEnableAptMetrics` | 83674F58 8368D350 |
| `kbEnableFishEye` | 838E9478 838F4B50 |
| `kbForceZeroWidthInAir` | 83500F98 |
| `kbForceZeroWidthInGrind` | 83500F98 |
| `kbGenerateHALIDs` | 836E2690 |
| `kbGenerateHALThumbHTML` | 836E2690 |
| `kbGenerateThumbnailList` | 836E2690 |
| `kbHOMEffectEnable` | 8390C320 |
| `kbLOD_ExtraXSpeed` | 840BA500 |
| `kbPreventModelSwapInFrontend` | 834CDA68 834DE8C0 |
| `kbSkate3D` | 8373CEB8 |
| `kbValidateHeaps` | 837B0A60 |
| `kbValidateSOA` | 837B0A60 |
| `kbVideoCaptureMode` | 837B0A60 837B8570 |

## Verification status (2026-10-15)

93 ported options each run alone in the park (scratchpad `verify.sh`: alive at
50 s, bright frame, and `zdx_trace_blocks` proof that the option's block ran).
85 pass. `kbValidateHeaps` / `kbValidateSOA` stay on the loading screen past
50 s (zd walks every heap each frame -- zd's own cost). `kGodHandCameraLOSDebugDraw`
runs but zd's god-hand camera asserts `lineTestBatch` (its line-test batch is
not set up in m; the null virtual call is skipped). The remaining exits are
one rare crash: `FATAL call to 0x00000000` in EventSys dispatch (sub_82E1ED28
via EventSys::Flush from GameModule::Render), roughly 1 in 20 runs with
zd_debug on, 0 in 13 with it off. No transplanted code is on its stack; the
pipe is popped from the font loader and the asset-activation loader thread at
once during startup in m's own code. Not yet proven to be (or not to be)
caused by the port.

Before 2026-10-15 a block's gate only worked when other transplanted code also
referenced the same option (unmapped cvar object = option off), so some
options counted as verified earlier never ran; fixed in zdx.py.

**AI paths (2026-10-15):** pres-side path drawing (zd 835022B8, from
Presentation::DebugDraw) and PathManager::DoUpdate (stripped from m, called
from SimUpdate1) run behind their options (`kbIsPresSideAiPathDrawingEnabled`,
`kbIsNonObjectAiPathDrawingEnabled`, `kbDraw*Paths`, ...). PathManager layout
from the live objects: identical to +29276, 16 records each a word larger in
m (+4..+64), +64 to +72608; zd's automated path-testing state (+72608..) is an
extension block. The manager singleton holder zd 0x8490442C = m 0x8336D054. The
blocks run without faults; no paths drew in the Black Box park test (the
in-game "AI Paths" menu that selects/edits them is item 16).

Extension blocks can be keyed at an offset (`"ext:ID@K"`): SkaterPresentation's
ctor receives the enclosing object (zd +0 = m +36) while SetDebugStrings gets
the SkaterPresentation, so the ctor's DebugStringRefs are keyed at this+36.

**Round 3 (2026-10-15):** gated REPLACE + slicer fixes ported 30 more options
(audio surface / VU / hand-grab / ollie-land prints, CAS morph values,
character position line tests, Good Samaritan, photographer target, grind
geometry test, wipeout displays, Lua state manager, teleport / board mount
tests, flee target iterator, head yaw/pitch targets, DMO / skater avoidance
situations, hand prop disconnect, collision debug, BodySpin display, IK
contact debug, head adjust lines, RAIN road / intersection draws, free cam
debug, shake debug info, foot catch, reverb, AI path debug info, character
render debug for sky / SH entities, surface getters). 36 of 40 passed the
first run; the failures were fixed: zd's `kbAssertOnCRCErrors` (default on, an
assert on save-data CRC) is not ported; the surface getters read the surface
record at m's offsets (+4 from +616, table of 140); `kDrawGrip` /
`kSKC_DebugDraw` are not ported (zd's Update3_State leaves board state m's
SkateboardBody asserts on).

Not ported, by rule: zd warnings about its own missing assets
(`kSpamScreenWithDMOWarnings`, `kEnableNoPathFoundWarning`), behaviour /
render switches on by default in zd (`kRenderApt`, `kEnableSunDirectionMap`,
`kUseSnappingBBoxTest`, ...), tuning values -- listed with reasons in
`zdx_blocks.NOT_PORTED` / `zdx_callblocks.NOT_PORTED`. Still pending (with
reasons in `zdx_callblocks.py`): Apt metrics, path spam / default location
test (gameplay functions m inlined with a redesigned nav-path object), the
multiplayer sync checker, OffBoardFeetIK debug data, profilers, and the
remaining unmapped zd globals in the pending report.

**Round 4 (2026-10-15, triage of the 114 unported display toggles):** each
toggle's zd users were classified (scratchpad triage: fn_diff of paired
functions, callers of unpaired ones, vtable slots, raw data references).
* Ported as debug blocks: land collision output, interactable-object line
  tests, skeleton torque forces, shot heading, biped cadence, scoring
  other-skater / vehicle debug, camera parameter values (previous /
  interpolated), grind pair detection, SB joints, hand-plant primitives, ped
  spawn DMO checks, landing-on-deck trajectory, frame composer reference
  point, vis curtains, pumping graphs (kDrawNewPump / kScaleGraph1/2 /
  kDrawDeckAngleCompression).
* `Physics::Pumping`: zd's ctor also builds four DebugGraph widgets
  (+80..+223, zd 240 bytes vs m 96); they are an extension block built by zd's
  ctor code after m's (`pumping_debug.cpp`, EXT 14), as for BodySpin.
* **Virtual debug draws m emptied** (`vslots_debug.cpp`, config `VSLOTS`): m
  kept the vtable slots but they point at its shared empty function
  (0x831F4B98). zd's functions are transplanted and m's slots get their
  thunks, so m's own virtual calls draw again: challenge sign-up volumes /
  markers / missing locators (cChallengeSignUpManager), locked streamer
  position (cChallengeSimManager), loaded dynamic hulls
  (cChallengeDynamicHullManager), preload volumes / NIS
  (cChallengePreloadManager), off-board pushing spline blends, RAIN road
  targets. The slot address is checked to still hold the empty function.
  m also stripped the loop that calls the challenge managers' slot
  (cActivityManager's debug draw, zd 0x83F165C0, called from
  Sk8ChallengeSystem's update): it is a CALLBLOCK gated by all of the
  managers' switches (zd's caller log, `sk83_na_zd_callers.cpp`, gave the
  call site). Verified: the sign-up and sim managers' zd draws are called in
  the park. The same loop calls slot 18 of the challenge *activities*; m's
  activity vtables hold the empty function there too (the activities' own
  debug draws, kShowGlobalActivityBBoxes etc.): a follow-up for VSLOTS.
* Pairs found through vtable / table slots go in
  `sk83_na_zd/tools/match/vslot_pairs.tsv` (loaded by zdx.py and
  offset_map.py). Caution: a pair's methods vote for its class map; a pair
  whose zd body is mostly debug-only fields can outvote the class
  (Pedestrian::UpdateTargetTransform did; disabled there).
* Debug Graph "Render Time": zd's GameModule::Render (paired by vtable slot
  with m 0x8283B3B0) averages the frame time over ~1 s and submits it. Not
  ported yet: submitted from m's render thread it faulted in
  Channel::AddSample at world start (see zdx_config.py). GraphHandle::Submit /
  Release now compare with the invalid handle 0 directly (zd's constant sat
  in the vtable-slot remap range).
* Generator fixes: const member functions returning a struct (`QBA?AV`) have
  `this` in r4 (zdx_replace.this_reg_for only knew non-const ones);
  StructMap sorts its ranges, so a class map must not overlap (CameraMan +616
  is m +620, which fixed kbShowShotHeading's crash).
* Not portable, by evidence: 18 toggles zd itself never reads (only
  registered): kbShowAirCollision, kShowPathEdges, kShowCrossWalk,
  kbShowShotChanges, kbShowMedicalBill, kShowTakeDownDebug,
  kbShowFourThreeLimits, kShowNoSimPresAnimationSkeletonLW,
  kCameraPositionDebugEnabled, kDebugDrawPedNavData_Reverse,
  kDebugDrawPedNavDataNextRoad2, kDebugDrawPedNavDataPrevCrosswalk2,
  kDebugGeneralNavModifier_ZeroVelocityThreshold, kDebugSkaterVelocityExceeds,
  kDrawDynamicObjectGrabSplineApproachVectors, kPhysicsLineTestsLength,
  kPhysicsTestFatness, kSpatialInstanceLineTestWidth. Systems m does not have:
  LivingWorld::MeshNavigation (kShowAStar, kShowAStarAnalysis,
  kShowPathCostThreshold, kDebugTargetingActions, kBoundaryTest1-3,
  kShowNearestIntersection; m keeps only the base Navigation vtable), the
  biped physics states (kBiped_DebugDraw / DebugDrawLanding / DrawVelocity /
  IntentsDebug: zd's PhysState_Biped_* vtables have no m counterpart),
  PhysOutConditioner_PathRecording (kDrawHighDetailRecordedPath).
* The in-game "AI Paths" menu (AIPathMainMenu) is not added: its closure is
  ~930 zd functions (PathManager package loading, race behaviours, AI
  behaviour vtables): the menu drives real AI objects, not only the display.
  The path display it toggles is ported and reachable through the cvars.
* Pending with reasons in `zdx_blocks.PENDING`: Pedestrian FullFPSDebugDraw /
  EvaluateNavModifiers / UpdateTargetTransform (whole-function ports needing
  zd-only Pedestrian members), AISkaterManager BE player data (the sim stalls
  once the block runs), biped debug draw (+320 absent in m), off-board input
  debug (ActionGraphInputListener +60).

**AI path drawing freeze (2026-10-16):** enabling
`kbIsPresSideAiPathDrawingEnabled` froze the game: zd's
Node::GetWidthVectorSafe / GetWidthVector were paired (one call-propagation
vote) with m functions that take InterpolatedNode-like data (vectors at +16 /
+80); on a Node they asserted (aipathnode.cpp 494) on the render thread, which
never returned. AIPath::Node is identical in both builds (+0..+44,
derive_cls), so the 13 weak Node pairs are in match/bad_pairs.tsv and zd's own
methods run. `kbIsAiPathDrawingEnabled` (PathManager::DoUpdate) then reached
zd's PathEditor, the AI Paths menu's interactive tool, which m's PathManager
never builds (zd ctor: +74464; read through lis/ori/lwzx, not routed to the
extension block): PathEditor::Update / Draw are patched out. Both switches run
in the park without asserts; the park has no AI paths, so drawing is not yet
seen on screen (needs the city).

**Freeze in the location menu = Debug Graph (2026-10-16):** a live sample of
the frozen game showed the sim thread spinning in Channel::Update (Graph::Update
from GameModule::Update) and the render thread waiting on the graph lock. The
channel's sample list was broken: zd's `fixed_list<pair<float,float>,128>`
constructor (2048-byte node pool) had been paired with m's constructor of
another instantiation (512 bytes, 32 nodes), so every channel ran out of nodes
after 32 samples and linked a null node; channels fed quickly (Render Time,
or any after a location reload) broke first. That pair is in
match/bad_pairs.tsv (zd's constructor runs). Also: Graph::CreateChannel call
sites go through the host (`zdx_host_CreateChannel`), which returns the
existing channel of the same name, so reloads no longer destroy channels that
scoped graph timers still hold; "Render Time" is ported again (GameModule::Render).
* Template pairs with other constants: `const_diff.py` (scratchpad) lists pairs
  whose code only differs in immediates. One more class of bug found that way:
  **memory categories** -- m's xcore category enum gained entries after 26 (zd
  58 = m 62, ...). Literal category arguments of the category allocators in
  zd code are remapped by the generator (config MEM_CATEGORIES, voted from
  paired functions).

**Round 5 (2026-10-16, agent-assisted evidence for the pending list):** field
and global evidence was collected per item (scratchpad triage/fields*.md,
globals.md) and applied in zdx_config.py with its source noted. Newly ported:
reverb debug out, skater-avoidance situations, off-board input / pushing
debug, linear/angular force magnitude, off-board feet IK (third block),
challenge game mode, manual meter, Lua state graph verbose, lane-change curve,
head-tracking targeting mode, OBGA analysis, spatial-instance debug text,
DMO ownership boxes / table, AI skater intents, locations / location start
points (kShowLocations: zd's lazy-initialised vectors get private storage).
CameraMan methods are now typed (`this` = CameraMan) so transplanted callees
use its map (DrawConditionValues read m's float at +616 before).
`kShowStreamedAssets` (GrindData asset boxes) is not ported: on m's data
DrawAABox faulted; its branch is patched out.
Full regression of all ported options: 131 / 139 at the first pass; the
exits were the known intermittent EventSys crash (pass on re-run), the two
heap validators (zd's own cost, dark at 50 s) and the two fixed above.

**Log files (2026-10-16):** zd's PrintChannelMan opens
`d:\logs\<MM-DD-YYYY>\output_HH_MM_SS.log` and `warnings_*.log` (game\logs on
the host; zd prints the month 0-based). Fixed in the port: the root pointer
(zd "d:\logs") was auto-mapped to an m "d:" (logfile_debug.cpp / zdx::LogRoot);
m's prints (ConsolePrintFunction -> OutputDebugStringA) never reached zd's
PrintChannelMan, so the game print hook (skate3_online.cpp) forwards each line
into zd's output log. Needs `allow_game_relative_writes = true` (the runtime
mounts the game folder read-only otherwise; set in the relwithdebinfo toml).

**Marquee viewer (Character Debug Menu > Start Marquee Viewer, 2026-10-18):**
works. The viewer itself is m's own code (MVManager, MVSettings from
data\mvsettings.xml, ViewerCamera, character loading); zd supplies what m
reduced to a shell or dropped:
- the flow: the menu item sends m's message 82; its handler (82874900) runs
  zd's StateMarqueeViewer::DoMarqueeViewer on m's StateMarqueeViewer object
  (vtable slots in vslots_debug.cpp; zd's flag byte at +32 lives in ext:17,
  m's object is 32 bytes). Pause NIS (11) as in zd. "Viewer finished" = m's
  message 81 + param 1 (the menu's Leave Marquee Viewer, and what m's
  MVManager posts).
- zd's MarqueeViewerDebugMenu (built in SetupInGameDebugMenus, GameModule+60 =
  ext:9) and MVManager::ProcessInput (REPLACE), which opens it on the
  viewer's menu input (m dropped events 0, 1 and 11). "Apply animation":
  MVManager state numbers shifted (zd viewing 7 = m 8), zd's
  ViewerCharacterAnimator::PlayAnimation (m dropped it; its request field is
  zd +14984 / m +15000) and the enum tags re-pointed after zd's onGainFocus
  (host wrapper: the name vector moved its strings while growing).
- the stage: m keeps no geometry at mvsettings' absolute camera position, so
  the viewer loads m's create-a-skater stage the way m's own character viewer
  does (LooseWorldPresentation::LoadLooseWorld(Presentation+72, 2) / Unload on
  leave) and ViewerCamera::SetDefault moves mvsettings' camera anchor onto it
  (`zdx_marquee_stage`, `zdx_marquee_stage_x/y/z`); zd's layout (character
  offsets, pan/tilt/fov, animations) is unchanged.
- the "N Kb" figures: m's XAllocator category records (+60 + 16 * region:
  bytes / peak / allocs / frees) are kept again (memstats_debug.cpp, as zd's
  category allocate / free did); MVMetrics and the arena stats read them.
Needed fixes outside the viewer: DebugHeap 3 MB -> zd's 10 MB
(heaps_debug.cpp; the Debug category ran out and the assert froze the render
thread) and the BIG-directory overlay scanning BIGF backslash paths
(sk83_na_m_app.h; recipe\marquee etc. were missing).

**CAC Prototype Viewer (Character Debug Menu > Start CAC Prototype Viewer,
2026-10-18):** works: create, Startup on the create-a-skater stage, the
CACPrototypeDebugMenu (GameModule+64, ext:9 +4; LoadCACPrototypeMenu from the
prototype's controller input), Leave CAC Prototype (Shutdown, snapshot, camera
back). cacproto_debug.cpp makes zd's CASPrototype in m's empty workspace slot
(+96 = m Simulation +208) after InitWorkspace, destroys it before
ShutWorkspace, updates it after Simulation::PostUpdate and skips PostUpdate's
own CASSimManager::Update while it is active (the impl updates it). Port
notes: Simulation workspace zd +168..+300 = m -56; Presentation's
LooseWorldPresentation zd +128 = m +72; CAC::MORPH_ZONES 22 (zd) vs 19 (m);
CASUndo buffer zd +528 / m +516; m's CASSimManager::Startup takes a player
index (r7 = 0, player 0's record); false pairs 837DEC88 (fixed_vector
DoAllocate) and 835ACF58 (SendMsg) in bad_pairs.tsv; CASSimManager::Update
name-paired.

Tests: `--zdx_test_charmenu_item=0` (marquee: open, menu, apply animation,
leave) / `=1` (CAC prototype: start, menu, leave).

**Regression status (2026-10-16 morning):** all ported options, each alone in
the park: 145 / 149 pass at the first pass; re-runs: 2 were the intermittent
EventSys crash below, 1 was the spatial-instance volume draw (call block
8416A9D8 demoted), 1 passed. Intermittent `FATAL call to 0x00000000` in
EventSys::Flush (render thread, while loading): 0 / 70 boots with zd_debug off,
0 / 20 plain boots with it on, 1 / 30 boots with --zdx_crash_probe +
--zdx_trace_blocks, ~1 / 30 verification runs -- probably port-related, not
attributed yet (the popped event's handler
object has a null vtable).

**Stalls and generator fixes (2026-10-16 morning):** stack samples of stalled
runs (tools/stackdump: stackdump.exe + symdump.py, in sk83_na_zd/tools)
showed:
* `zdx::Find` (every zd global access) was a linear scan over ~600 map
  entries; heavy displays crawled. Find and Ext now have per-thread caches.
* Indexed accesses with large offsets (`lis rX,1 / ori rX,rX,N` + `lwzx`) were
  not routed through the struct maps at all: zd's raw offsets read m's
  objects. The generator now tracks lis/ori constants like li constants, maps
  them through the class map and into extension blocks. This found real
  misreads: PathManager +33500 (= m +33564) in the AI path drawing, the
  PathManager extension block (+72608..), LivingWorld::Manager +49220/+49224.
* kShowLocations' call block became a gated REPLACE of all of zd's
  SimUpdate2 (unresolved argument) -- loading never finished; now an explicit
  instruction range (259-280). Its location list belongs to cLocationManager
  (m lacks zd's word at +4); with the map applied it runs, but it draws
  thousands of markers (render ~2 fps) -- heavy, as a debug view.
* The debug overlay keeps at most 16384 shapes (zd's pool is fixed too).
* Not ported (runtime): DMO ownership boxes/table (deadlock while loading on a
  lock in 83FFB318), global location files (zd-only tree at cLocationManager
  +33500), manual meter (wrong Sk8ChallengeSystem field), streamed asset boxes.

**Collision geometry / skeleton look (2026-10-15):**
* `kDrawSkaterGroundTriangles` (startup_collisiongeometry.ini): zd drew each
  ground triangle the skater's ground collision job tested, from the job's
  triangle tester. m keeps the tester's batching but not the drawing, so
  `ground_collision_debug.cpp` draws m's batched triangles with zd's
  DrawTriangle before m's RunIntersectionTests (read-only). zd's own
  BoxTest/RipTriangles can't be used on m's meshes (cluster format changed),
  nor its whole pipeline/job (m's assembly part records grew 144 -> 224 bytes).
  The ini file itself boots m only merged into m's own startup.ini (it lacks
  settings m needs).
* Native renderer (src/skate3_native_scene*.cpp, approved by the user):
  SimpleDraw's lit shader `simpledraw_p_n_sc` (zd's debug boxes/spheres) is
  now lit per vertex as its microcode does (normal through c4..c7, light c9,
  ambient c255.x) instead of flat c8; debug lines are a whole number of pixels
  (1 px at 720p) like zd's instead of 1.125 px quads that rasterised 2 px wide.
  zd's debug spheres / boxes use a 32-bit index list that the 16-bit-first
  guess accepted (all entries in range, every other one 0): it now takes the
  32-bit reading when every even 16-bit entry is 0 and the 32-bit one is valid
  (the boxes drew as wedges before). Debug draws stay on top of the scene:
  zd draws them all with Z test on (RB_DEPTHCONTROL 0x736, LESS_EQUAL) but at a
  point where the depth buffer holds the world and not the characters, so the
  skeleton shows through the skater and the collision triangles are hidden
  only by world geometry. The native scene depth includes the characters, so
  a depth test there would hide the skeleton; matching zd exactly needs
  world-only depth (not done: the collision lines are not hidden by walls).
  Note: the native renderer is off in the tomls (skate3_native_render_scene),
  so tests of these need --skate3_native_render_scene=true.


**Item-15 batch (2026-10-18):** kept zd functions whose remaining dead
options are read inside them, as debug blocks: AI path obstruction box test
(`kbIsAiPathDebugInfoEnabled`, 834F7100), skateboard / skeleton bad-data tests
(`kMakeSkateboardBad`, `kMakeSkateboardBadVel`, 84174378), CaC HAL id /
thumbnail list generation (836E2690), census override (`kPedestrianCensusOverride`,
8378E7B8), landing-on-deck LOD speeds, skill cache timeout, sim DMO warning,
`kTestWipeout`, contained-waypoint Y tolerance, hand-plant ground normal.
Each verified alone in the park (alive, picture at 50 s). `kMakeSkateboardBad`
does what zd's does -- it writes bad data into the board on purpose (the
option resets itself) and m's own check then asserts in
LWDynamicObjectMan.cpp(392); the game keeps running.
* Generator fix: a block that calls `cVarBoolRef::Set` to reset its own
  option passes the cvar object the option check loaded into r3 *before* the
  block range. The slicer counted only argument registers written inside the
  block, so the fragment called Set with `this` (consolevarman.cpp(419)
  assert). Config `CALL_READS` (block_slice.CALL_READS) lists callees whose
  argument registers are always read: `83D90A30` cVarBoolRef::Set r3/r4.
* Dropped again (all on by default in zd, so they ran at every boot):
  `kDoPathValidation` (84226B68: a truncated loop over the LW execution
  targets; its virtual call hit 0x301), `kDoFeetPostPhysics` (83F45C98:
  mid-function fragment reading the caller's stack vectors; SkeletonBody::
  GetPart assert) and `kSpamScreenWithDMOWarnings` (83FCD490 / 83FD2300:
  CountVolumes walks rw::physics Assembly parts with zd's layout) -- the last
  two were already NOT_PORTED / EXCLUDED by rule; the batch must skip
  entries listed there.
* Test worlds: the test copy of `data\debugfe.ini` takes `World=DIST_<name>`
  (DIST_DownTown has pedestrians and traffic; an unknown name stops at the
  Debug Frontend menu). Keep its CRLF line endings.

**2026-10-18 afternoon:**
* Generator bug: `zdx_replace.override_source` reused the name `gates` for
  each fragment's gate list, so after the first BLOCKS hook every later
  "gated REPLACE + BLOCKS" function was emitted *without* its gated
  replacement. Affected: `kShowOllieAndLandSurface` (83569BB8,
  SFXObj_Contacts::GetOllieLandType, called on ollie / landing) -- listed as
  ported before but never emitted. Fixed; it runs without faults in the park
  (an ollie needs a stick flick, not testable from the key sender).
* `kDebugDrawWaypoint`: WaypointGroup::FullFPSDebugDraw (zd 841F4C18) put in
  m's emptied vtable slot (Prop-family vtables in the same order; m slot
  0x824459D0, `vslots_debug.cpp`). Class map: wrapper header zd -32 = m -36,
  tail +192.. = m +4 (its +208 / +216 checked at runtime: the first try faulted
  on +208). Verified against zd in the Black Box park: both draw the same
  magenta shapes at the roof.
* LivingWorld::Manager::FullFPSDebugDraw (zd 83781610) runs every component
  manager's per-object draw; zd calls it every frame, the port only while one
  of its gate options is on -- the per-object switches (waypoints, DMO
  attribute names, grab-spline normals) are gates now too.
* `kDrawDynamicObjectAttribNames`: DynamicObject::FullFPSDebugDraw's call
  block runs (trace) but nothing shows in the park; zd's own park has no DMOs
  (its DMOs fail to load), so there is no reference picture. Left in, open.
* Blend tree display (`kDisplayBlendTree` / Compact / Summary, the
  Andale::cAnimDebugDumper user, item 10): zd 83F11D08 is slot 21 of the skater
  animatable's vtable; m keeps the vtable (0x8235EF10) with the empty
  function there, now filled (class map +13456.. = m +16, from 54 paired
  methods). kGroupsStartYPos (zd-only int cvar the cvar scan missed) is
  registered through config `ZD_CVARS_EXTRA`; the MetricsManager singleton is
  mapped (zd 0x84913E10 = m 0x8337D5B0). Not triggered yet: zd itself draws no
  blend tree with kDisplayBlendTree alone, and zd's caller of the slot is not
  found (a temporary caller log in zd saw no call either).
* Pedestrian nav rigs (`kDebugNavRigs`, `kDebugNavRigMaxAngle`): pending --
  zd draws NavRig::DebugDraw on a Pathogen::NavRig* at Pedestrian+6664; the
  candidate m field (+5816) is null for every pedestrian in downtown, and m's
  Pathogen NavRig methods are unpaired.
* Debug state-stack messages (item 14): the BE state stack is the
  `be_stack_debug.cpp` sender; nothing in zd sends cMsgAddDebugFEScreenStack
  (only its constructor, registration and Presentation's receiver exist), and
  cMsgCheatState comes from Lua through a binding m kept -- nothing left.
* zd reference runs: `run_zdt.sh` runs zd from a scratch copy of its game
  folder (links + own debugfe.ini) with its own exe copy (sk83zdt.exe) and
  only its PID; zd's recomp cannot load DIST_DownTown (crash while loading,
  also with no options), the Black Box park works. `ZD_SET_CVARS=<zd objects>`
  turns zd bool options on.

**Viewer menus on Start (2026-10-18 evening):** both viewers open their menu
on **Start** as in zd (ViewerInputHandler: action 124 AptStart -> command 1;
MVManager::ProcessInput -> GameModule::LoadMarqueeViewerMenu, CASPrototypeImpl
835F8090 -> LoadCACPrototypeMenu); the debug-menu combination opens the
normal debug menu in zd too. Leaving: item 0 of the viewer's menu. It did
nothing in m because the generator's lis dataflow ended at `bctr` (switch
jumps): case labels started with no constants, so the Sk8AppMan base built
above ProcessInput's switch (`lwz r11,29620(r28)`) read zd's raw address and
QueryModule<GameModule> got nothing. zdx.lis_dataflow now gives a bctr the
labels nothing else reaches (the jump table) as successors; this mapped 242
more zd global accesses in switch cases across the port. Inside a menu Start
also selects (input.cfg MenuKey: Start.released = select).
Test: `--zdx_test_charmenu_item=100` / `=101` starts the marquee / CAC viewer
only (input left to the pad).

**CAC Prototype menu settings (2026-10-18 evening):** every item tested with
`--zdx_test_charmenu_item=101 --zdx_test_cac_items=...` (menu item ids, or
`cN[:v]` = viewer command N as the pad queues it; the menu is reopened after
an item closes it, as Start does). Item ids: 0 Leave, 1 Part Swap, 2
Colourize, 3 morphing toggle, 4 Stamping, 5 Remove Part, 6 Dirt Opacity, 7
Cycle Preview (On/Off, onChange), 8 Toggle Gender, 9 Update User Generated
Textures, 10 Mark Undo, 11 Perform Undo, 12 Rostral. Viewer commands
(ViewerInputHandler): 1 Start (menu), 2 RS click, 3/5 RS x/y, 4/6 LS y/x, 7
d-pad up/down axis, 11 d-pad left/right axis, 12-15 d-pad L/R/U/D pressed,
16 A, 17 B, 18 X, 19 Y. Fixes:
* The menu's onCommand (837C8040) and ProcessControllerInput (835F8090) had
  switch-case globals unmapped (the bctr dataflow fix above): before it the
  settings wrote zd addresses.
* CASSimManager::SetColourForZone: m's takes arrays (zones, colours, count,
  callback), zd's one zone + colour; zd's call (ApplyCurrentColour, Colourize
  + A) passed the callback as the count (cascolourhelper.cpp 53). Patched to
  count 1.
* The prototype camera (CameraPan / Tilt / FOV / Move: sticks, d-pad axes)
  locked a mutex at PresThreadBroadcaster+16 that m's broadcaster does not
  have; the critical section's bookkeeping landed in the asset ref counter's
  hashtable next to it (count -> "REX\0") and cAssetRefCounter::RemAsset
  later crashed the render thread (after Leave / gender / part changes, when
  assets were released). Found with a per-frame watch of the table word and
  the pad's command trace; the sends now go without the lock, as m's own do.
Verified: Part Swap (browse filters / parts, install), Colourize (browse,
apply), Stamping, Rostral, Toggle Gender, Mark / Perform Undo (gender
reverts), Remove Part, morphing toggle + right stick, Update User Generated
Textures (returns offline), camera sticks / d-pad, Leave (back to the park in
the edited outfit). Not exercised: Cycle Preview / Dirt Opacity / the
Remove Part slot value (they change through the menu's left/right, onChange).
