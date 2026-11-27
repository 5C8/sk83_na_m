// build/livesk8/sk8shader/dev/xbox2/final/gen/source/minimap.cpp (sk83 assert path; the shader
// compiler generated the original). 2 effect binders, generated from sk83_na_m by
// tools/gen_shader_binders.py -- type check, constant uploads into the D3D device's constant
// shadow (XMemCpy + dirty bits), texture and sampler binds, bind counters. Verified with
// sk83_na_m --sk83_replay.
#include "rwp_guestcall.h"
#include "rwp_ppc_ops.h"
#include "sk8shader_binder_support.h"

namespace Sk8 { namespace ShaderGen {
#ifdef RWP_GUEST
using namespace ::rwp;
#endif

// sk83_na_m 0x82f5aca0: const binder (effect type 0xd681342a)
void Binder_82f5aca0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd681342aull))
    {
        BINDER_ASSERT(0x823c3758u, 19);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f95240: const+tex binder (effect type 0xcc81a698)
void Binder_82f95240(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcc81a698ull))
    {
        BINDER_ASSERT(0x823c37c0u, 40);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t3 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t5 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t6);
}

} }
