// build/livesk8/sk8shader/dev/xbox2/final/gen/source/cac_cloth.cpp (sk83 assert path; the shader
// compiler generated the original). 14 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f4dfc8: const binder (effect type 0x927e7603)
void Binder_82f4dfc8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff927e7603ull))
    {
        BINDER_ASSERT(0x823b42b8u, 82);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f4e068: const binder (effect type 0xc93b740b)
void Binder_82f4e068(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffc93b740bull))
    {
        BINDER_ASSERT(0x823b43c0u, 154);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1c00000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x80u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f4e108: const binder (effect type 0xc93b740b)
void Binder_82f4e108(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffc93b740bull))
    {
        BINDER_ASSERT(0x823b4418u, 170);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x18c0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x600000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x80u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f4e1a8: const binder (effect type 0xc93b740b)
void Binder_82f4e1a8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffc93b740bull))
    {
        BINDER_ASSERT(0x823b4470u, 191);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1c00000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x80u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f4e248: const binder (effect type 0xc93b740b)
void Binder_82f4e248(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffc93b740bull))
    {
        BINDER_ASSERT(0x823b44c8u, 207);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1c00000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x80u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f4e2e8: const binder (effect type 0x0ff06541)
void Binder_82f4e2e8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xff06541u))
    {
        BINDER_ASSERT(0x823b45d0u, 265);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f74c58: const+tex binder (effect type 0x927e7603)
void Binder_82f74c58(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff927e7603ull))
    {
        BINDER_ASSERT(0x823b4208u, 19);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(27, t2);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(29, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t12 = BLD32(t2 + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x30u);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t14 = BLD32(t2 + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t16 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x10u);
    unsigned long long t17 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t18 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t17, t18, 0x10u);
    unsigned long long t19 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t20 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t19, t20, 0x10u);
    unsigned long long t21 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t22 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t21, t22, 0x30u);
    unsigned long long t23 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t24 = BLD32(t2 + 0x84);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t23, t24, 0x30u);
    unsigned long long t25 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t26 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t25, t26, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x8c)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t27 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t28 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t28);
    BST32(0x833c31c0u, t27);
}

// sk83_na_m 0x82f74ea0: const+tex binder (effect type 0x927e7603)
void Binder_82f74ea0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(25);
    RWP_GUEST_FRAME(frame, 0x90);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff927e7603ull))
    {
        BINDER_ASSERT(0x823b4260u, 47);
    }
    RWP_NV(26, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(25, t2);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(28, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    RWP_NV(29, 0x1000000000000000ull);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1840ull);
    unsigned long long t12 = BLD32(t2 + 0x68);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t14 = BLD32(t2 + 0x34);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t16 = BLD32(t2 + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x10u);
    RWP_NV(27, 0x800000000000000ull);
    unsigned long long t17 = (BLD32(0x833a5ea0u) + 0x1890ull);
    unsigned long long t18 = BLD32(t2 + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t17, t18, 0x30u);
    unsigned long long t19 = (BLD32(0x833a5ea0u) + 0x1880ull);
    unsigned long long t20 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t19, t20, 0x10u);
    unsigned long long t21 = (BLD32(0x833a5ea0u) + 0x1870ull);
    unsigned long long t22 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t21, t22, 0x10u);
    unsigned long long t23 = (BLD32(0x833a5ea0u) + 0x1850ull);
    unsigned long long t24 = BLD32(t2 + 0x6c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t23, t24, 0x20u);
    unsigned long long t25 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t26 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t25, t26, 0x10u);
    unsigned long long t27 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t28 = BLD32(t2 + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t27, t28, 0x20u);
    unsigned long long t29 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t30 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t29, t30, 0x30u);
    unsigned long long t31 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t32 = BLD32(t2 + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t31, t32, 0x10u);
    unsigned long long t33 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t34 = BLD32(t2 + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t33, t34, 0x10u);
    unsigned long long t35 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t36 = BLD32(t2 + 0x18);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t35, t36, 0x10u);
    unsigned long long t37 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t38 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t37, t38, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x8c)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x90)), 0x2u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x98)), 0x1u);
    unsigned long long t39 = (BLD32(0x833c31c0u) + 0xeull);
    unsigned long long t40 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31bcu, t40);
    BST32(0x833c31c0u, t39);
}

// sk83_na_m 0x82f751c0: const+tex binder (effect type 0x927e7603)
void Binder_82f751c0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff927e7603ull))
    {
        BINDER_ASSERT(0x823b4310u, 98);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(27, t2);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(29, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t12 = BLD32(t2 + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x30u);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t14 = BLD32(t2 + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t16 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x10u);
    unsigned long long t17 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t18 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t17, t18, 0x10u);
    unsigned long long t19 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t20 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t19, t20, 0x10u);
    unsigned long long t21 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t22 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t21, t22, 0x30u);
    unsigned long long t23 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t24 = BLD32(t2 + 0x84);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t23, t24, 0x30u);
    unsigned long long t25 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t26 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t25, t26, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x8c)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t27 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t28 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t28);
    BST32(0x833c31c0u, t27);
}

// sk83_na_m 0x82f75408: const+tex binder (effect type 0x927e7603)
void Binder_82f75408(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff927e7603ull))
    {
        BINDER_ASSERT(0x823b4368u, 126);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(27, t2);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(29, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t12 = BLD32(t2 + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x30u);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t14 = BLD32(t2 + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t16 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x10u);
    unsigned long long t17 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t18 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t17, t18, 0x10u);
    unsigned long long t19 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t20 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t19, t20, 0x10u);
    unsigned long long t21 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t22 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t21, t22, 0x30u);
    unsigned long long t23 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t24 = BLD32(t2 + 0x84);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t23, t24, 0x30u);
    unsigned long long t25 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t26 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t25, t26, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x8c)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t27 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t28 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t28);
    BST32(0x833c31c0u, t27);
}

// sk83_na_m 0x82f75650: const+tex binder (effect type 0x0ff06541)
void Binder_82f75650(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xff06541u))
    {
        BINDER_ASSERT(0x823b4520u, 223);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x6ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1900ull);
    unsigned long long t6 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x380000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x90u);
    RWP_NV(30, 0x400000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18e0ull);
    unsigned long long t8 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x18f0ull);
    unsigned long long t10 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x4u);
    unsigned long long t11 = (BLD32(0x833c31c0u) + 0x3ull);
    unsigned long long t12 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31bcu, t12);
    BST32(0x833c31c0u, t11);
}

// sk83_na_m 0x82f757b8: const+tex binder (effect type 0x0ff06541)
void Binder_82f757b8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xff06541u))
    {
        BINDER_ASSERT(0x823b4578u, 244);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x6ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1960ull);
    unsigned long long t6 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1c0000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x90u);
    RWP_NV(30, 0x100000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x1940ull);
    unsigned long long t8 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x100000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x1950ull);
    unsigned long long t10 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x100000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x4u);
    unsigned long long t11 = (BLD32(0x833c31c0u) + 0x3ull);
    unsigned long long t12 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31bcu, t12);
    BST32(0x833c31c0u, t11);
}

// sk83_na_m 0x82f75920: const+tex binder (effect type 0x0ff06541)
void Binder_82f75920(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xff06541u))
    {
        BINDER_ASSERT(0x823b4628u, 281);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x6ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1900ull);
    unsigned long long t6 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x380000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x90u);
    RWP_NV(30, 0x400000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18e0ull);
    unsigned long long t8 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x18f0ull);
    unsigned long long t10 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x4u);
    unsigned long long t11 = (BLD32(0x833c31c0u) + 0x3ull);
    unsigned long long t12 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31bcu, t12);
    BST32(0x833c31c0u, t11);
}

// sk83_na_m 0x82f75a88: const+tex binder (effect type 0x0ff06541)
void Binder_82f75a88(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xff06541u))
    {
        BINDER_ASSERT(0x823b4680u, 302);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x8), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x8) + 0x6ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x1900ull);
    unsigned long long t6 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x380000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x90u);
    RWP_NV(30, 0x400000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18e0ull);
    unsigned long long t8 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x18f0ull);
    unsigned long long t10 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x4u);
    unsigned long long t11 = (BLD32(0x833c31c0u) + 0x3ull);
    unsigned long long t12 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31bcu, t12);
    BST32(0x833c31c0u, t11);
}

} }
