// build/livesk8/sk8shader/dev/xbox2/final/gen/source/postfx.cpp (sk83 assert path; the shader
// compiler generated the original). 13 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f959c0: tex binder (effect type 0xe47db5b3)
void Binder_82f959c0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c41a0u, 19);
    }
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xe4)), 0x0u);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
}

// sk83_na_m 0x82f95a40: const+tex binder (effect type 0xe47db5b3)
void Binder_82f95a40(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c41f8u, 34);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0xa0);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0xa4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xb8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0xbc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xe4)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f95b50: const+tex binder (effect type 0xe47db5b3)
void Binder_82f95b50(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c4250u, 53);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc4)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xe4)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf0)), 0x4u);
    unsigned long long t4 = (BLD32(0x833c31c0u) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31c0u, t4);
}

// sk83_na_m 0x82f95c30: const+tex binder (effect type 0xe47db5b3)
void Binder_82f95c30(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c42a8u, 71);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t3 = BLD32(b + 0xac);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t5 = BLD32(b + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t7 = BLD32(b + 0x9c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t9 = BLD32(b + 0x44);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t11 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xcc)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xd0)), 0x7u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xdc)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xe0)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf0)), 0x4u);
    unsigned long long t12 = (BLD32(0x833c31c0u) + 0x5ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x5ull));
    BST32(0x833c31c0u, t12);
}

// sk83_na_m 0x82f95da8: const+tex binder (effect type 0xe47db5b3)
void Binder_82f95da8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c4300u, 95);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc4)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xe4)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xec)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf0)), 0x4u);
    unsigned long long t4 = (BLD32(0x833c31c0u) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31c0u, t4);
}

// sk83_na_m 0x82f95e98: const+tex binder (effect type 0xe47db5b3)
void Binder_82f95e98(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c4358u, 114);
    }
    RWP_NV(27, 0xffffffff833a0000ull);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t9 = BLD32(b + 0xb4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    RWP_NV(29, 0x1000000000000000ull);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1840ull);
    unsigned long long t11 = BLD32(b + 0x88);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    RWP_NV(28, 0x4000000000000000ull);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t13 = BLD32(b + 0xa8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t15 = BLD32(b + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x1850ull);
    unsigned long long t17 = BLD32(b + 0x84);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    unsigned long long t18 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t19 = BLD32(b + 0x50);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t18, t19, 0x10u);
    unsigned long long t20 = (BLD32(0x833a5ea0u) + 0x1860ull);
    unsigned long long t21 = BLD32(b + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t20, t21, 0x10u);
    unsigned long long t22 = (BLD32(0x833a5ea0u) + 0x1870ull);
    unsigned long long t23 = BLD32(b + 0xb0);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x1000000000000000ull));
    BINDER_XMEMCPY(t22, t23, 0x10u);
    unsigned long long t24 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t25 = BLD32(b + 0x4c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t24, t25, 0x10u);
    RWP_NV(29, 0x2000000000000000ull);
    unsigned long long t26 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t27 = BLD32(b + 0x58);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t26, t27, 0x10u);
    unsigned long long t28 = (BLD32(0x833a5ea0u) + 0x1830ull);
    unsigned long long t29 = BLD32(b + 0x5c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t28, t29, 0x10u);
    unsigned long long t30 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t31 = BLD32(b + 0x38);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t30, t31, 0x10u);
    unsigned long long t32 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t33 = BLD32(b + 0x3c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t32, t33, 0x10u);
    unsigned long long t34 = (BLD32(0x833a5ea0u) + 0x17e0ull);
    unsigned long long t35 = BLD32(b + 0x68);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t34, t35, 0x10u);
    unsigned long long t36 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t37 = BLD32(b + 0x6c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t36, t37, 0x10u);
    unsigned long long t38 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t39 = BLD32(b + 0x60);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t38, t39, 0x10u);
    unsigned long long t40 = (BLD32(0x833a5ea0u) + 0x1820ull);
    unsigned long long t41 = BLD32(b + 0x64);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t40, t41, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc4)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xcc)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xe4)), 0x0u);
    unsigned long long t42 = (BLD32(0x833c31c0u) + 0x10ull);
    unsigned long long t43 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31bcu, t43);
    BST32(0x833c31c0u, t42);
}

// sk83_na_m 0x82f961f0: const+tex binder (effect type 0xe47db5b3)
void Binder_82f961f0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c43b0u, 151);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t7 = BLD32(b + 0x18);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t9 = BLD32(b + 0x1c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t11 = BLD32(b + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t13 = BLD32(b + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t15 = BLD32(b + 0x54);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t17 = BLD32(b + 0x2c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t18 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t19 = BLD32(b + 0x3c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t18, t19, 0x10u);
    unsigned long long t20 = (BLD32(0x833a5ea0u) + 0x17e0ull);
    unsigned long long t21 = BLD32(b + 0x38);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t20, t21, 0x10u);
    unsigned long long t22 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t23 = BLD32(b + 0x34);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t22, t23, 0x10u);
    unsigned long long t24 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t25 = BLD32(b + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t24, t25, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc8)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xcc)), 0x1u);
    unsigned long long t26 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t27 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t27);
    BST32(0x833c31c0u, t26);
}

// sk83_na_m 0x82f96428: const+tex binder (effect type 0xe47db5b3)
void Binder_82f96428(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c4408u, 179);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0xa0);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0xa4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xb8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0xbc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf4)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f96538: const+tex binder (effect type 0xe47db5b3)
void Binder_82f96538(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c4460u, 198);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0xa0);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0xa4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xb8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0xbc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t11 = BLD32(b + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t13 = BLD32(b + 0xc0);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t15 = BLD32(b + 0x48);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf4)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf8)), 0x2u);
    unsigned long long t16 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t16);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x3ull));
}

// sk83_na_m 0x82f966c8: const+tex binder (effect type 0xe47db5b3)
void Binder_82f966c8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c44b8u, 221);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0xa0);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0xa4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xb8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0xbc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t11 = BLD32(b + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t13 = BLD32(b + 0xc0);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t15 = BLD32(b + 0x48);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf4)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf8)), 0x2u);
    unsigned long long t16 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t16);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x3ull));
}

// sk83_na_m 0x82f96858: const+tex binder (effect type 0xe47db5b3)
void Binder_82f96858(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c4510u, 244);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0xa0);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0xa4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xb8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0xbc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t11 = BLD32(b + 0x98);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t13 = BLD32(b + 0xc0);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t15 = BLD32(b + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    RWP_NV(29, 0x4000000000000000ull);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t17 = BLD32(b + 0x94);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    unsigned long long t18 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t19 = BLD32(b + 0x48);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t18, t19, 0x10u);
    unsigned long long t20 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t21 = BLD32(b + 0x8c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t20, t21, 0x10u);
    unsigned long long t22 = (BLD32(0x833a5ea0u) + 0x17e0ull);
    unsigned long long t23 = BLD32(b + 0x90);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t22, t23, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xd8)), 0x5u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf4)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xf8)), 0x2u);
    unsigned long long t24 = (BLD32(0x833c31c0u) + 0x7ull);
    unsigned long long t25 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31bcu, t25);
    BST32(0x833c31c0u, t24);
}

// sk83_na_m 0x82f96a80: const+tex binder (effect type 0xe47db5b3)
void Binder_82f96a80(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c4568u, 272);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x78);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xd4)), 0x8u);
    unsigned long long t4 = (BLD32(0x833c31c0u) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t4);
}

// sk83_na_m 0x82f96b40: const+tex binder (effect type 0xe47db5b3)
void Binder_82f96b40(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffe47db5b3ull))
    {
        BINDER_ASSERT(0x823c45c0u, 288);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x70);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t5 = BLD32(b + 0x40);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xcc)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xe8)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31c0u, t6);
}

} }
