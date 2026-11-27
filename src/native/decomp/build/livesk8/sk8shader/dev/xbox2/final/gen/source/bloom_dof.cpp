// build/livesk8/sk8shader/dev/xbox2/final/gen/source/bloom_dof.cpp (sk83 assert path; the shader
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

// sk83_na_m 0x82f69fa0: const+tex binder (effect type 0x2459d874)
void Binder_82f69fa0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823af838u, 19);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    RWP_NV(29, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    RWP_NV(31, 0x4000000000000000ull);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t5 = BLD32(b + 0x38);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t7 = BLD32(b + 0x68);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x40u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t9 = BLD32(b + 0x54);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t11 = BLD32(b + 0x4c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x17e0ull);
    unsigned long long t13 = BLD32(b + 0x50);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x84)), 0x0u);
    unsigned long long t14 = (BLD32(0x833c31c0u) + 0x5ull);
    unsigned long long t15 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t15);
    BST32(0x833c31c0u, t14);
}

// sk83_na_m 0x82f6a108: const+tex binder (effect type 0x2459d874)
void Binder_82f6a108(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823af890u, 40);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x68);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t4 = (BLD32(0x833c31c0u) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t4);
}

// sk83_na_m 0x82f6a1c8: const+tex binder (effect type 0x2459d874)
void Binder_82f6a1c8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823af8e8u, 56);
    }
    RWP_NV(28, 0xffffffff833a0000ull);
    RWP_NV(29, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t5 = BLD32(b + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t9 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    RWP_NV(30, 0x4000000000000000ull);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t11 = BLD32(b + 0x20);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x17c0ull);
    unsigned long long t13 = BLD32(b + 0x14);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t15 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x17e0ull);
    unsigned long long t17 = BLD32(b + 0x1c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    unsigned long long t18 = (BLD32(0x833a5ea0u) + 0x17d0ull);
    unsigned long long t19 = BLD32(b + 0x18);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t18, t19, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x78)), 0x0u);
    unsigned long long t20 = (BLD32(0x833c31c0u) + 0x9ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t20);
}

// sk83_na_m 0x82f6a388: const+tex binder (effect type 0x2459d874)
void Binder_82f6a388(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823af940u, 80);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t3 = BLD32(b + 0x44);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t5 = BLD32(b + 0x48);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t7 = BLD32(b + 0x3c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t9 = BLD32(b + 0x40);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x7c)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31c0u) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31c0u, t10);
}

// sk83_na_m 0x82f6a4a8: const+tex binder (effect type 0x2459d874)
void Binder_82f6a4a8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823af998u, 100);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t3 = BLD32(b + 0x40);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t5 = BLD32(b + 0x3c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x7c)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x8c)), 0x2u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x3ull));
    BST32(0x833c31c0u, t6);
}

// sk83_na_m 0x82f6a598: const+tex binder (effect type 0x2459d874)
void Binder_82f6a598(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823af9f0u, 119);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t3 = BLD32(b + 0x6c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xe000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x90u);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t5 = BLD32(b + 0x64);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t7 = BLD32(b + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x88)), 0x1u);
    unsigned long long t8 = (BLD32(0x833c31c0u) + 0x3ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31c0u, t8);
}

// sk83_na_m 0x82f6a6a0: const+tex binder (effect type 0x2459d874)
void Binder_82f6a6a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823afa48u, 138);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t3 = BLD32(b + 0x2c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t5 = BLD32(b + 0x30);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t7 = BLD32(b + 0x34);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t9 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f6a7b0: const+tex binder (effect type 0x2459d874)
void Binder_82f6a7b0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823afaa0u, 157);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t3 = BLD32(b + 0x6c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xe000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x90u);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t5 = BLD32(b + 0x64);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t7 = BLD32(b + 0x28);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t8 = (BLD32(0x833c31c0u) + 0x3ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t8);
}

// sk83_na_m 0x82f6a8a8: const+tex binder (effect type 0x2459d874)
void Binder_82f6a8a8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823afaf8u, 175);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t3 = BLD32(b + 0x2c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t5 = BLD32(b + 0x30);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t7 = BLD32(b + 0x34);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t9 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t11 = BLD32(b + 0x6c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xe000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x90u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t13 = BLD32(b + 0x58);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t14 = (BLD32(0x833c31c0u) + 0x2ull);
    unsigned long long t15 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t15);
    BST32(0x833c31c0u, t14);
}

// sk83_na_m 0x82f6aa10: const+tex binder (effect type 0x2459d874)
void Binder_82f6aa10(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823afb50u, 196);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t3 = BLD32(b + 0x2c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t5 = BLD32(b + 0x30);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t7 = BLD32(b + 0x34);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t9 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t11 = BLD32(b + 0x6c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xe000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x90u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t13 = BLD32(b + 0x60);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x10u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t15 = BLD32(b + 0x5c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x7c)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t16 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t16);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x3ull));
}

// sk83_na_m 0x82f6aba8: const+tex binder (effect type 0x2459d874)
void Binder_82f6aba8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823afba8u, 219);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t3 = BLD32(b + 0x2c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t5 = BLD32(b + 0x30);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t7 = BLD32(b + 0x34);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t9 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t11 = BLD32(b + 0x58);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    unsigned long long t12 = (BLD32(0x833a5ea0u) + 0x17b0ull);
    unsigned long long t13 = BLD32(b + 0x6c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xe000000000000000ull));
    BINDER_XMEMCPY(t12, t13, 0x90u);
    unsigned long long t14 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t15 = BLD32(b + 0x5c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t14, t15, 0x10u);
    unsigned long long t16 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t17 = BLD32(b + 0x60);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t16, t17, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x7c)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t18 = BLD32(0x833c31c0u);
    unsigned long long t19 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t19);
    BST32(0x833c31c0u, (t18 + 0x4ull));
}

// sk83_na_m 0x82f6ad60: const+tex binder (effect type 0x2459d874)
void Binder_82f6ad60(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823afc00u, 243);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t5 = BLD32(b + 0x70);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xf000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x100u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
    BST32(0x833c31c0u, t6);
}

// sk83_na_m 0x82f6ae60: const+tex binder (effect type 0x2459d874)
void Binder_82f6ae60(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x2459d874u))
    {
        BINDER_ASSERT(0x823afc58u, 260);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x74);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t5 = BLD32(b + 0x68);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x40u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x80)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
    BST32(0x833c31c0u, t6);
}

} }
