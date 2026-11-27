// build/livesk8/sk8shader/dev/xbox2/final/gen/source/sky.cpp (sk83 assert path; the shader
// compiler generated the original). 4 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f5e270: const binder (effect type 0x4f2a34e0)
void Binder_82f5e270(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x4f2a34e0u))
    {
        BINDER_ASSERT(0x823c6b68u, 19);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(29, t2);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t4 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x40u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t6 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t8 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t10 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x30u);
    unsigned long long t11 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x2ull));
    BST32(0x833c31c0u, t11);
}

// sk83_na_m 0x82f5e378: const binder (effect type 0x4f2a34e0)
void Binder_82f5e378(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x4f2a34e0u))
    {
        BINDER_ASSERT(0x823c6bb8u, 38);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(29, t2);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t4 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x40u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t6 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t8 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x30u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x2ull));
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f5e460: const binder (effect type 0x88723dcc)
void Binder_82f5e460(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff88723dccull))
    {
        BINDER_ASSERT(0x823c6c08u, 56);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f99528: tex binder (effect type 0x96db2af2)
void Binder_82f99528(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff96db2af2ull))
    {
        BINDER_ASSERT(0x823c6c58u, 77);
    }
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x4)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x8)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc)), 0x5u);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
}

} }
