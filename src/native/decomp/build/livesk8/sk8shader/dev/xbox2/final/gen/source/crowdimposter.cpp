// build/livesk8/sk8shader/dev/xbox2/final/gen/source/crowdimposter.cpp (sk83 assert path; the shader
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

// sk83_na_m 0x82f50bd8: const binder (effect type 0xecfc00eb)
void Binder_82f50bd8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffecfc00ebull))
    {
        BINDER_ASSERT(0x823b7890u, 19);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(31, t2);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x880ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x800000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x870ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x4ull));
}

// sk83_na_m 0x82f7d818: const+tex binder (effect type 0x3d7adad9)
void Binder_82f7d818(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x3d7adad9u))
    {
        BINDER_ASSERT(0x823b78f0u, 43);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x810ull);
    unsigned long long t3 = BLD32(b + 0xc);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x8ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(b + 0x14);
    unsigned long long t7 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x18), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x18) + 0x4ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t5, t6, t7);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x850ull);
    unsigned long long t9 = BLD32(b + 0x1c);
    unsigned long long t10 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x20), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x20) + 0xcull), 30, 2, 31) - 0x3ull), 0) & 0xffffffffull)), 61) & 0x1fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t8, t9, t10);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t12 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t14 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x24)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x28)), 0x1u);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x3ull));
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x2ull));
}

} }
