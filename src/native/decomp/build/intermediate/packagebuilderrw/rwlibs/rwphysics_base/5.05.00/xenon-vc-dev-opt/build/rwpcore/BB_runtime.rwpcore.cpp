// The rwpcore unity build (Skate 2 PDB module ...\rwphysics_base\5.05.00\xenon-vc-dev-opt\build\rwpcore\
// BB_runtime.rwpcore.cpp), partial: only the files whose codegen depends on sharing a TU.
// Includes are relative to src/ (tools/m.py passes it as an include dir).
// Simulation::BatchIntegrator reuses r3 after calling RigidBody::DynamicUpdate, which cl 15 only
// does with DynamicUpdate's body in the same TU.
#include "packages/rw/rwphysics_base/5.05.00/source/physics/core/rigidbody/host/xbox2/rwprigidbody.cpp"
#include "packages/rw/rwphysics_base/5.05.00/source/physics/core/solver/host/xbox2/rwpbatchdynamicupdate.cpp"
