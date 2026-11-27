// build/livesk8/sk8shader/dev/xbox2/final/gen/source/godrayspotlight.cpp (sk83 assert path; the shader
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

// sk83_na_m 0x82f5a238: const binder (effect type 0x5f5e3abb)
void Binder_82f5a238(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x5f5e3abbu))
    {
        BINDER_ASSERT(0x823c28a8u, 19);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(29, t2);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t4 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x40u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t6 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    unsigned long long t7 = (BLD32(0x833c31c0u) + 0x1ull);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
    BST32(0x833c31c0u, t7);
}

// sk83_na_m 0x82f93880: const+tex binder (effect type 0x3521ab09)
void Binder_82f93880(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x3521ab09u))
    {
        BINDER_ASSERT(0x823c2920u, 41);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc)), 0x3u);
    unsigned long long t5 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t5);
}

} }
