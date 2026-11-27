# Decompiled native code (guest-memory build)

Copied from `REXGLUE/skate-physics-decomp` by `python tools/sync_sk83.py` there -- do not edit here.
The tree keeps the original source paths (sk83 assert strings; Skate 2 PDB line tables for the
rw::physics, AnimCommandSystem and EAPhysicsWorld packages):

- `packages/rw/rwphysics_base/5.05.00/source/physics/...` -- rw::physics solver job
- `packages/ea/animcommandsystem/1.01.00/source/...` -- PoseWithMatMult, ACSsqtBlend_job
- `packages/skate/gameplay/andale/dev/...` -- the Andale animation fetch (extractpackedVBR.cpp)
- `game/source/spu/clusteredmesh/clusteredmeshspu.cpp` -- UnitGetOverlappingGPInstances
- `support/` -- decomp-only headers (PDB-typed layouts, rwp_config.h three-way build, the
  recompiler's vector lowering in rwp_guest_rt.h); on the include path via CMakeLists.txt

Built with `RWP_GUEST` (big-endian views of guest memory). Proof: `tools/equiv/run.py --hand ...`
in that repo runs every function in this build and recompiled on the same guest memory: all
bit-exact. Hooks: `src/native/sk83_native_physics.cpp` / `_anim.cpp` / `_collision.cpp`.
