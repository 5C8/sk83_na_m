// build/livesk8/sk8shader/dev/xbox2/final/gen/source/shadow.cpp (sk83 assert path; the shader
// compiler generated the original). 17 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f5d080: const binder (effect type 0x9ed432c1)
void Binder_82f5d080(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c56d8u, 244);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f5d120: const binder (effect type 0x9ed432c1)
void Binder_82f5d120(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c5730u, 260);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f5d1c0: const binder (effect type 0x9ed432c1)
void Binder_82f5d1c0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c5788u, 276);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f5d260: const binder (effect type 0x9ed432c1)
void Binder_82f5d260(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c57e0u, 292);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f5d300: const binder (effect type 0x9ed432c1)
void Binder_82f5d300(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c5838u, 308);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f5d3a0: const binder (effect type 0x9ed432c1)
void Binder_82f5d3a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c5890u, 324);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f5d440: const binder (effect type 0x9ed432c1)
void Binder_82f5d440(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c58e8u, 340);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f5d4e0: const binder (effect type 0x9ed432c1)
void Binder_82f5d4e0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff9ed432c1ull))
    {
        BINDER_ASSERT(0x823c5940u, 356);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f97940: const+tex binder (effect type 0xa8e32783)
void Binder_82f97940(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c53c0u, 19);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f97a50: const+tex binder (effect type 0xa8e32783)
void Binder_82f97a50(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c5418u, 38);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f97b60: const+tex binder (effect type 0xa8e32783)
void Binder_82f97b60(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c5470u, 57);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f97c70: const+tex binder (effect type 0xa8e32783)
void Binder_82f97c70(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c54c8u, 76);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f97d80: const+tex binder (effect type 0xa8e32783)
void Binder_82f97d80(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c5520u, 95);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x0u);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t10);
}

// sk83_na_m 0x82f97e90: const+tex binder (effect type 0xa8e32783)
void Binder_82f97e90(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c5578u, 114);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t11 = BLD32(BLD32(b + 0x1c) + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x0u);
    unsigned long long t12 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t13 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t13);
    BST32(0x833c31c0u, t12);
}

// sk83_na_m 0x82f97fd0: const+tex binder (effect type 0xa8e32783)
void Binder_82f97fd0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c55d0u, 134);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t11 = BLD32(BLD32(b + 0x1c) + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x0u);
    unsigned long long t12 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t13 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t13);
    BST32(0x833c31c0u, t12);
}

// sk83_na_m 0x82f98110: const+tex binder (effect type 0xa8e32783)
void Binder_82f98110(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c5628u, 154);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t11 = BLD32(BLD32(b + 0x1c) + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x0u);
    unsigned long long t12 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t13 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t13);
    BST32(0x833c31c0u, t12);
}

// sk83_na_m 0x82f98250: const+tex binder (effect type 0xa8e32783)
void Binder_82f98250(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa8e32783ull))
    {
        BINDER_ASSERT(0x823c5680u, 174);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x790ull);
    unsigned long long t5 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t4, t5, 0x10u);
    unsigned long long t6 = (BLD32(0x833a5ea0u) + 0x7b0ull);
    unsigned long long t7 = BLD32(b + 0xc);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t6, t7, 0x10u);
    unsigned long long t8 = (BLD32(0x833a5ea0u) + 0x7a0ull);
    unsigned long long t9 = BLD32(b + 0x10);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t8, t9, 0x10u);
    unsigned long long t10 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t11 = BLD32(BLD32(b + 0x1c) + 0x24);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t10, t11, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x0u);
    unsigned long long t12 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t13 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t13);
    BST32(0x833c31c0u, t12);
}

} }
