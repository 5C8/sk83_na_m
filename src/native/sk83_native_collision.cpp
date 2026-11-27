// Native collision functions (cvar sk83_native_collision), hand-decompiled in
// REXGLUE/skate-physics-decomp (src/collision, Skate 2 PDB types) and proven bit-exact against
// the recompiled functions there (`tools/equiv/run.py --hand unitgo`). Built for guest memory
// (RWP_GUEST); rwp::g_base is defined in sk83_native_physics.cpp.
//
// Native: UnitGetOverlappingGPInstances (ClusteredMesh unit -> GPTriangles; the RipTriangles
// functors of the collision jobs call it per unit). Units the native code does not handle (the
// original's assert paths) go to the recompiled function. "collision:" log lines every 10 s.

#include "generated/sk83_na_m_init.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <chrono>
#include <cstdint>

#define RWP_GUEST 1
#include "native/decomp/game/source/spu/clusteredmesh/clusteredmeshspu.cpp"

REXCVAR_DEFINE_BOOL(sk83_native_collision, true, "Skate 3",
                    "Run decompiled collision functions (UnitGetOverlappingGPInstances) as native "
                    "code instead of the recompiled ones. Same results bit for bit; faster");

namespace {
std::atomic<uint64_t> g_ns{0}, g_calls{0}, g_native{0};
std::atomic<int64_t> g_next_log{0};

int64_t NowNs()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

void Account(int64_t t0, bool native)
{
    const int64_t t1 = NowNs();
    g_ns.fetch_add(uint64_t(t1 - t0), std::memory_order_relaxed);
    g_calls.fetch_add(1, std::memory_order_relaxed);
    if (native) g_native.fetch_add(1, std::memory_order_relaxed);
    int64_t due = g_next_log.load(std::memory_order_relaxed);
    if (due == 0) { g_next_log.compare_exchange_strong(due, t1 + 10'000'000'000LL); return; }
    if (t1 < due || !g_next_log.compare_exchange_strong(due, t1 + 10'000'000'000LL)) return;
    const uint64_t ns = g_ns.exchange(0), c = g_calls.exchange(0), n = g_native.exchange(0);
    if (c)
        REXLOG_INFO("collision: UnitGetOverlappingGPInstances {} | {:.3f} us/call, {} calls/s, {:.2f} ms/s",
                    n == c ? "NATIVE" : n ? "mixed" : "recompiled", double(ns) / double(c) / 1000.0, c / 10,
                    double(ns) / 1e7);
}
}  // namespace

extern "C" REX_FUNC(rex_UnitGetOverlappingGPInstances_YAIPAVClusteredMeshCluster_collision_rw_IPBVMatrix44Affine_vpu_math_3_PAUGPTriangle_23_AAIIIMGI_Z_828FDB48)
{
    const int64_t t0 = NowNs();
    if (REXCVAR_GET(sk83_native_collision))
    {
        rwp::g_base = base;
        const uint32_t sp = ctx.r1.u32;     // the caller's parameter slots: flags (u16) and volume tag
        const uint32_t flags = __builtin_bswap16(*(const uint16_t *)(base + sp + 0x56));
        const uint32_t volumeTag = __builtin_bswap32(*(const uint32_t *)(base + sp + 0x5c));
        uint32_t size = 0;
        if (rw::collision::UnitGetOverlappingGPInstances(&size, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                                         ctx.r7.u32, ctx.r8.u32, ctx.r9.u32, float(ctx.f1.f64),
                                                         flags, volumeTag))
        {
            ctx.r3.u64 = size;
            Account(t0, true);
            return;
        }
    }
    __imp__rex_UnitGetOverlappingGPInstances_YAIPAVClusteredMeshCluster_collision_rw_IPBVMatrix44Affine_vpu_math_3_PAUGPTriangle_23_AAIIIMGI_Z_828FDB48(ctx, base);
    Account(t0, false);
}
