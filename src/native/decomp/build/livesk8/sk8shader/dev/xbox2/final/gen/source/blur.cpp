// build/livesk8/sk8shader/dev/xbox2/final/gen/source/blur.cpp (sk83 assert path; the shader
// compiler generated the original). 10 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f4a8b0: const binder (effect type 0x9b2b5ade)
void Binder_82f4a8b0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9b2b5adeull))
    {
        BINDER_ASSERT(0x823b04c8u, 19);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f4a950: const binder (effect type 0x9b2b5ade)
void Binder_82f4a950(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9b2b5adeull))
    {
        BINDER_ASSERT(0x823b0520u, 35);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f4a9f0: const binder (effect type 0x9b2b5ade)
void Binder_82f4a9f0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9b2b5adeull))
    {
        BINDER_ASSERT(0x823b0578u, 51);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f4aa90: const binder (effect type 0x9b2b5ade)
void Binder_82f4aa90(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9b2b5adeull))
    {
        BINDER_ASSERT(0x823b05d0u, 67);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f4ab30: const binder (effect type 0x9b2b5ade)
void Binder_82f4ab30(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9b2b5adeull))
    {
        BINDER_ASSERT(0x823b0628u, 83);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f6b8d0: const+tex binder (effect type 0x16cbc674)
void Binder_82f6b8d0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x16cbc674u))
    {
        BINDER_ASSERT(0x823b0680u, 124);
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
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t6);
}

// sk83_na_m 0x82f6b9a0: const+tex binder (effect type 0x16cbc674)
void Binder_82f6b9a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x16cbc674u))
    {
        BINDER_ASSERT(0x823b06d8u, 141);
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
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t6);
}

// sk83_na_m 0x82f6ba70: const+tex binder (effect type 0x16cbc674)
void Binder_82f6ba70(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x16cbc674u))
    {
        BINDER_ASSERT(0x823b0730u, 158);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(30, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t3 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t5 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0xc)), 0x1u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31c0u, t6);
}

// sk83_na_m 0x82f6bb50: tex binder (effect type 0x16cbc674)
void Binder_82f6bb50(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x16cbc674u))
    {
        BINDER_ASSERT(0x823b0788u, 176);
    }
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
}

// sk83_na_m 0x82f6bbd0: const+tex binder (effect type 0x16cbc674)
void Binder_82f6bbd0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x16cbc674u))
    {
        BINDER_ASSERT(0x823b07e0u, 191);
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
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t6 = (BLD32(0x833c31c0u) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31c0u, t6);
}

} }
