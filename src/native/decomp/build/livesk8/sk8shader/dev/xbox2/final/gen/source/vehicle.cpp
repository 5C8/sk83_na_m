// build/livesk8/sk8shader/dev/xbox2/final/gen/source/vehicle.cpp (sk83 assert path; the shader
// compiler generated the original). 11 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f60d20: const binder (effect type 0x3d590871)
void Binder_82f60d20(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x3d590871u))
    {
        BINDER_ASSERT(0x823c9b00u, 48);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0xc) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f60dc0: const binder (effect type 0xa30c5001)
void Binder_82f60dc0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa30c5001ull))
    {
        BINDER_ASSERT(0x823c9c58u, 122);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x60u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f60e60: const binder (effect type 0xa30c5001)
void Binder_82f60e60(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa30c5001ull))
    {
        BINDER_ASSERT(0x823c9ce0u, 143);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x60u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f60f00: const binder (effect type 0xa30c5001)
void Binder_82f60f00(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa30c5001ull))
    {
        BINDER_ASSERT(0x823c9d68u, 159);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x60u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f60fa0: const binder (effect type 0x76b0a063)
void Binder_82f60fa0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x76b0a063u))
    {
        BINDER_ASSERT(0x823c9e58u, 198);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f9cca8: const+tex binder (effect type 0x3d590871)
void Binder_82f9cca8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x3d590871u))
    {
        BINDER_ASSERT(0x823c9a98u, 19);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0xc);
    RWP_NV(27, t2);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x1370ull);
    unsigned long long t4 = BLD32(b + 0x4);
    unsigned long long t5 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0xbeull), 30, 2, 31) - 0x2full), 0) & 0xffffffffull)), 17) & 0x1ffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t3, t4, t5);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t7 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t9 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t11 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x40u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t13 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t15 = BLD32(t2 + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x30u);
    RWP_NV(29, 0x2000000000000000ull);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t17 = BLD32(t2 + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    unsigned long long t18 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t19 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t18, t19, 0x10u);
    unsigned long long t20 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t21 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t20, t21, 0x10u);
    unsigned long long t22 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t23 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t22, t23, 0x10u);
    unsigned long long t24 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t25 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t24, t25, 0x30u);
    unsigned long long t26 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t27 = BLD32(t2 + 0x84);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t26, t27, 0x30u);
    unsigned long long t28 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t29 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t28, t29, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x8c)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t30 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t31 = (BLD32(0x833c31bcu) + 0x5ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t31);
    BST32(0x833c31c0u, t30);
}

// sk83_na_m 0x82f9cf30: const+tex binder (effect type 0x3d590871)
void Binder_82f9cf30(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x3d590871u))
    {
        BINDER_ASSERT(0x823c9b68u, 64);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0xc);
    RWP_NV(27, t2);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x1370ull);
    unsigned long long t4 = BLD32(b + 0x4);
    unsigned long long t5 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0xbeull), 30, 2, 31) - 0x2full), 0) & 0xffffffffull)), 17) & 0x1ffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t3, t4, t5);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t7 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t9 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t11 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x40u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t13 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t15 = BLD32(t2 + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x30u);
    RWP_NV(29, 0x2000000000000000ull);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t17 = BLD32(t2 + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    unsigned long long t18 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t19 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t18, t19, 0x10u);
    unsigned long long t20 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t21 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t20, t21, 0x10u);
    unsigned long long t22 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t23 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t22, t23, 0x10u);
    unsigned long long t24 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t25 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t24, t25, 0x30u);
    unsigned long long t26 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t27 = BLD32(t2 + 0x84);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t26, t27, 0x30u);
    unsigned long long t28 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t29 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t28, t29, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x8c)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t30 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t31 = (BLD32(0x833c31bcu) + 0x5ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t31);
    BST32(0x833c31c0u, t30);
}

// sk83_na_m 0x82f9d1b8: const+tex binder (effect type 0x3d590871)
void Binder_82f9d1b8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x3d590871u))
    {
        BINDER_ASSERT(0x823c9bf0u, 93);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0xc);
    RWP_NV(27, t2);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x1370ull);
    unsigned long long t4 = BLD32(b + 0x4);
    unsigned long long t5 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0xbeull), 30, 2, 31) - 0x2full), 0) & 0xffffffffull)), 17) & 0x1ffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t3, t4, t5);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t7 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t9 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t11 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x40u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t13 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t15 = BLD32(t2 + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x30u);
    RWP_NV(29, 0x2000000000000000ull);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t17 = BLD32(t2 + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    unsigned long long t18 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t19 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t18, t19, 0x10u);
    unsigned long long t20 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t21 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t20, t21, 0x10u);
    unsigned long long t22 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t23 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t22, t23, 0x10u);
    unsigned long long t24 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t25 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t24, t25, 0x30u);
    unsigned long long t26 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t27 = BLD32(t2 + 0x84);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t26, t27, 0x30u);
    unsigned long long t28 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t29 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t28, t29, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x8c)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t30 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t31 = (BLD32(0x833c31bcu) + 0x5ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t31);
    BST32(0x833c31c0u, t30);
}

// sk83_na_m 0x82f9d440: const+tex binder (effect type 0x76b0a063)
void Binder_82f9d440(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x76b0a063u))
    {
        BINDER_ASSERT(0x823c9df0u, 175);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x6ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1360ull);
    unsigned long long t6 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x10000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(30, 0x400000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18c0ull);
    unsigned long long t8 = BLD32(b + 0x18);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x18f0ull);
    unsigned long long t10 = BLD32(b + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x700000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x90u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x18e0ull);
    unsigned long long t12 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x18d0ull);
    unsigned long long t14 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x24)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x28)), 0x4u);
    unsigned long long t15 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    unsigned long long t16 = BLD32(0x833c31c0u);
    BST32(0x833c31bcu, t15);
    BST32(0x833c31c0u, (t16 + 0x4ull));
}

// sk83_na_m 0x82f9d5e8: const+tex binder (effect type 0x76b0a063)
void Binder_82f9d5e8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x76b0a063u))
    {
        BINDER_ASSERT(0x823c9ee0u, 214);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x6ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1360ull);
    unsigned long long t6 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x10000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(30, 0x400000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18c0ull);
    unsigned long long t8 = BLD32(b + 0x18);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x18f0ull);
    unsigned long long t10 = BLD32(b + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x700000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x90u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x18e0ull);
    unsigned long long t12 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x18d0ull);
    unsigned long long t14 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x24)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x28)), 0x4u);
    unsigned long long t15 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    unsigned long long t16 = BLD32(0x833c31c0u);
    BST32(0x833c31bcu, t15);
    BST32(0x833c31c0u, (t16 + 0x4ull));
}

// sk83_na_m 0x82f9d790: const+tex binder (effect type 0x76b0a063)
void Binder_82f9d790(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x76b0a063u))
    {
        BINDER_ASSERT(0x823c9f50u, 237);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x6ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1360ull);
    unsigned long long t6 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x10000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(30, 0x400000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18c0ull);
    unsigned long long t8 = BLD32(b + 0x18);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x18f0ull);
    unsigned long long t10 = BLD32(b + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x700000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x90u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x18e0ull);
    unsigned long long t12 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x18d0ull);
    unsigned long long t14 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x24)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x28)), 0x4u);
    unsigned long long t15 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    unsigned long long t16 = BLD32(0x833c31c0u);
    BST32(0x833c31bcu, t15);
    BST32(0x833c31c0u, (t16 + 0x4ull));
}

} }
