// Skater ground collision triangles (kDrawSkaterGroundTriangles, e.g. from
// startup_collisiongeometry.ini).
//
// zd: Island::Step_Collision1 hands the switch to
// AssemblyGroundCollisionPipeline::Run, which copies it into each job's data;
// in AssemblyGroundCollisionJob the triangle tester draws every ground
// triangle it visits (AssemblyTriangleIntersectionTester::operator(),
// zd 0x83870C40 -> DrawTriangle -> DebugOverlay::DrawLine3D). m dropped the
// switch from Run / the job data and the drawing from the tester.
//
// m's tester still batches every triangle it visits (up to 20, count at
// tester+64, 192-byte GPTriangle records from +80) and tests the batch in
// RunIntersectionTests. Before that, each batched triangle is drawn with zd's
// own drawing code (zdx FRAGMENTS "GroundTriangleDraw": the tester's
// DrawTriangle call, zd's colours). Read-only: nothing is written back into
// m's physics.

#include "zd_debug.h"
#include "zdx.h"
#include "zdx_generated.h"

#include "generated/sk83_na_m_init.h"

namespace {
constexpr uint32_t kCvarDrawSkaterGroundTriangles = 0x83365044;  // m cvar object
constexpr uint32_t kTesterCount = 64;      // m tester: batched triangles (zd +60)
constexpr uint32_t kTesterTriangles = 80;  // m tester: first GPTriangle (zd +64)
constexpr uint32_t kTriangleSize = 192;
constexpr uint32_t kMaxBatch = 20;         // the tester flushes at 20
}  // namespace

// AssemblyTriangleIntersectionTester::RunIntersectionTests (m 0x82902EB0).
extern "C" REX_FUNC(rex_AssemblyTriangleIntersectionTester_RunIntersectionTests_AAAXXZ_82902EB0) {
  const uint32_t tester = ctx.r3.u32;
  if (zd_debug::Enabled() && zdx::Ready() && tester &&
      zd_debug::GameCvarBool(base, kCvarDrawSkaterGroundTriangles)) {
    const uint32_t n = REX_LOAD_U32(tester + kTesterCount);
    for (uint32_t i = 0; i < n && i < kMaxBatch; ++i) {
      zdx::GuestCall call(ctx, base, 0x100);
      call.c.r30.u64 = tester + kTesterTriangles + i * kTriangleSize;  // zd: mr r30,r4 (the triangle)
      call(zdx_frag_GroundTriangleDraw);
    }
  }
  __imp__rex_AssemblyTriangleIntersectionTester_RunIntersectionTests_AAAXXZ_82902EB0(ctx, base);
}
