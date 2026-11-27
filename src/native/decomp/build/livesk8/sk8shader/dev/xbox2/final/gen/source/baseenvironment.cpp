// build/livesk8/sk8shader/dev/xbox2/final/gen/source/baseenvironment.cpp (sk83 assert path; the shader
// compiler generated the original). 8 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f48008: const binder (effect type 0xc31a540b)
void Binder_82f48008(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffc31a540bull))
    {
        BINDER_ASSERT(0x823adef8u, 78);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(28, t2);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t12 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x30u);
    unsigned long long t13 = (BLD32(0x833c31c0u) + 0x1ull);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x4ull));
    BST32(0x833c31c0u, t13);
}

// sk83_na_m 0x82f48130: const binder (effect type 0xd2e0bd03)
void Binder_82f48130(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd2e0bd03ull))
    {
        BINDER_ASSERT(0x823adf88u, 98);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f481d0: const binder (effect type 0xd2e0bd03)
void Binder_82f481d0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd2e0bd03ull))
    {
        BINDER_ASSERT(0x823ae008u, 114);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x18d0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x400000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f48270: const binder (effect type 0x58bc2cf9)
void Binder_82f48270(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x58bc2cf9u))
    {
        BINDER_ASSERT(0x823ae198u, 181);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x2000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x810ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x3ull));
}

// sk83_na_m 0x82f66df8: const+tex binder (effect type 0xc31a540b)
void Binder_82f66df8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffc31a540bull))
    {
        BINDER_ASSERT(0x823ade08u, 19);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(28, t2);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t12 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x60u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x17e0ull);
    unsigned long long t14 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t16 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x10u);
    unsigned long long t17 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t18 = BLD32(t2 + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t17, t18, 0x10u);
    unsigned long long t19 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t20 = BLD32(t2 + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t19, t20, 0x30u);
    unsigned long long t21 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t22 = BLD32(t2 + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t21, t22, 0x10u);
    unsigned long long t23 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t24 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t23, t24, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x88)), 0x0u);
    unsigned long long t25 = (BLD32(0x833c31c0u) + 0x7ull);
    unsigned long long t26 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t26);
    BST32(0x833c31c0u, t25);
}

// sk83_na_m 0x82f67008: const+tex binder (effect type 0xc31a540b)
void Binder_82f67008(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(26);
    RWP_GUEST_FRAME(frame, 0x90);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffc31a540bull))
    {
        BINDER_ASSERT(0x823ade80u, 45);
    }
    RWP_NV(27, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(26, t2);
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
    unsigned long long t12 = BLD32(t2 + 0x34);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t14 = BLD32(t2 + 0x2c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t16 = BLD32(t2 + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x10u);
    unsigned long long t17 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t18 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t17, t18, 0x10u);
    unsigned long long t19 = (BLD32(0x833a5ea0u) + 0x1850ull);
    unsigned long long t20 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t19, t20, 0x10u);
    unsigned long long t21 = (BLD32(0x833a5ea0u) + 0x1870ull);
    unsigned long long t22 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1c00000000000000ull));
    BINDER_XMEMCPY(t21, t22, 0x60u);
    unsigned long long t23 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t24 = BLD32(t2 + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t23, t24, 0x10u);
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
    unsigned long long t31 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t32 = BLD32(t2 + 0x18);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t31, t32, 0x10u);
    unsigned long long t33 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t34 = BLD32(t2 + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t33, t34, 0x10u);
    unsigned long long t35 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t36 = BLD32(t2 + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t35, t36, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x88)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x90)), 0x2u);
    unsigned long long t37 = (BLD32(0x833c31c0u) + 0xdull);
    unsigned long long t38 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t38);
    BST32(0x833c31c0u, t37);
}

// sk83_na_m 0x82f672f8: const+tex binder (effect type 0x58bc2cf9)
void Binder_82f672f8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x58bc2cf9u))
    {
        BINDER_ASSERT(0x823ae088u, 135);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x810ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x8u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x24)), 0x7u);
    unsigned long long t8 = (BLD32(0x833c31bcu) + 0x3ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x6ull));
    BST32(0x833c31bcu, t8);
}

// sk83_na_m 0x82f67440: const+tex binder (effect type 0x58bc2cf9)
void Binder_82f67440(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x58bc2cf9u))
    {
        BINDER_ASSERT(0x823ae108u, 158);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x810ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x8u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x24)), 0x7u);
    unsigned long long t8 = (BLD32(0x833c31bcu) + 0x3ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x6ull));
    BST32(0x833c31bcu, t8);
}

} }
