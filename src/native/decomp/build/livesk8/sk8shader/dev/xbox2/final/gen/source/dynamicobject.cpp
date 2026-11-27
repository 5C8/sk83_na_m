// build/livesk8/sk8shader/dev/xbox2/final/gen/source/dynamicobject.cpp (sk83 assert path; the shader
// compiler generated the original). 30 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f56358: const binder (effect type 0xcadb29af)
void Binder_82f56358(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd748u, 47);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f563f8: const binder (effect type 0xcadb29af)
void Binder_82f563f8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd868u, 119);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f56498: const binder (effect type 0xcadb29af)
void Binder_82f56498(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd8c8u, 135);
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

// sk83_na_m 0x82f565c0: const binder (effect type 0xcadb29af)
void Binder_82f565c0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd928u, 155);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(28, t2);
    RWP_NV(30, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t12 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t14 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t16 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xc000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x50u);
    unsigned long long t17 = (BLD32(0x833c31c0u) + 0x3ull);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x4ull));
    BST32(0x833c31c0u, t17);
}

// sk83_na_m 0x82f56730: const binder (effect type 0xcadb29af)
void Binder_82f56730(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd988u, 177);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(28, t2);
    RWP_NV(30, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t12 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t14 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t16 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xc000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x50u);
    unsigned long long t17 = (BLD32(0x833c31c0u) + 0x3ull);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x4ull));
    BST32(0x833c31c0u, t17);
}

// sk83_na_m 0x82f568a0: const binder (effect type 0xcadb29af)
void Binder_82f568a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd9e8u, 199);
    }
    RWP_NV(29, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(28, t2);
    RWP_NV(30, 0x4000000000000000ull);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7e0ull);
    unsigned long long t4 = BLD32(t2 + 0x3c);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x10u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x7d0ull);
    unsigned long long t6 = BLD32(t2 + 0x40);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x10u);
    RWP_NV(31, 0x8000000000000000ull);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t8 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x40u);
    unsigned long long t9 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t10 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t9, t10, 0x10u);
    unsigned long long t11 = (BLD32(0x833a5ea0u) + 0x1790ull);
    unsigned long long t12 = BLD32(t2 + 0x80);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x10u);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1780ull);
    unsigned long long t14 = BLD32(t2 + 0x7c);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t13, t14, 0x10u);
    unsigned long long t15 = (BLD32(0x833a5ea0u) + 0x17a0ull);
    unsigned long long t16 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0xc000000000000000ull));
    BINDER_XMEMCPY(t15, t16, 0x50u);
    unsigned long long t17 = (BLD32(0x833c31c0u) + 0x3ull);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x4ull));
    BST32(0x833c31c0u, t17);
}

// sk83_na_m 0x82f56a10: const binder (effect type 0xd3cac1cf)
void Binder_82f56a10(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdb08u, 277);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1880ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56ab0: const binder (effect type 0xd3cac1cf)
void Binder_82f56ab0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdb68u, 298);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1880ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56b50: const binder (effect type 0xd3cac1cf)
void Binder_82f56b50(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdbc8u, 314);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1880ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56bf0: const binder (effect type 0xd3cac1cf)
void Binder_82f56bf0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdc28u, 340);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x6000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56c90: const binder (effect type 0xd3cac1cf)
void Binder_82f56c90(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdc88u, 356);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x6000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56d30: const binder (effect type 0xd3cac1cf)
void Binder_82f56d30(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdce8u, 372);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x17f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x6000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56dd0: const binder (effect type 0xd3cac1cf)
void Binder_82f56dd0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdd48u, 388);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1880ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56e70: const binder (effect type 0xd3cac1cf)
void Binder_82f56e70(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd3cac1cfull))
    {
        BINDER_ASSERT(0x823bdda8u, 404);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x1880ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x20u);
    BST32(0x833c31c0u, (BLD32(0x833c31c0u) + 0x1ull));
}

// sk83_na_m 0x82f56f10: const binder (effect type 0x07ea02f5)
void Binder_82f56f10(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823bde68u, 441);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t3 = BLD32(b + 0xc);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f56fc8: const binder (effect type 0x07ea02f5)
void Binder_82f56fc8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823bdf88u, 499);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t3 = BLD32(b + 0xc);
    unsigned long long t4 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t2, t3, t4);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f57080: const binder (effect type 0x07ea02f5)
void Binder_82f57080(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823bdfe8u, 515);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x2ull));
}

// sk83_na_m 0x82f89738: const+tex binder (effect type 0xcadb29af)
void Binder_82f89738(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd6e8u, 19);
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
    unsigned long long t12 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x50u);
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

// sk83_na_m 0x82f89980: const+tex binder (effect type 0xcadb29af)
void Binder_82f89980(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd7a8u, 63);
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
    unsigned long long t12 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x50u);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t14 = BLD32(t2 + 0x20);
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
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x88)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t27 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t28 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t28);
    BST32(0x833c31c0u, t27);
}

// sk83_na_m 0x82f89bc8: const+tex binder (effect type 0xcadb29af)
void Binder_82f89bc8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bd808u, 91);
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
    unsigned long long t12 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x50u);
    RWP_NV(30, 0x2000000000000000ull);
    unsigned long long t13 = (BLD32(0x833a5ea0u) + 0x1800ull);
    unsigned long long t14 = BLD32(t2 + 0x20);
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
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x88)), 0x0u);
    BINDER_SETTEXTURE(BLD32(BLD32(t2 + 0x94)), 0x1u);
    unsigned long long t27 = (BLD32(0x833c31c0u) + 0x8ull);
    unsigned long long t28 = (BLD32(0x833c31bcu) + 0x4ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x2ull));
    BST32(0x833c31bcu, t28);
    BST32(0x833c31c0u, t27);
}

// sk83_na_m 0x82f89e10: const+tex binder (effect type 0xcadb29af)
void Binder_82f89e10(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bda48u, 221);
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
    unsigned long long t12 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x50u);
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

// sk83_na_m 0x82f8a058: const+tex binder (effect type 0xcadb29af)
void Binder_82f8a058(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffcadb29afull))
    {
        BINDER_ASSERT(0x823bdaa8u, 249);
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
    unsigned long long t12 = BLD32(t2 + 0x30);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x3000000000000000ull));
    BINDER_XMEMCPY(t11, t12, 0x50u);
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

// sk83_na_m 0x82f8a2a0: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8a2a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823bde08u, 420);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18a0ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f8a410: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8a410(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823bdec8u, 457);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18a0ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f8a580: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8a580(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823bdf28u, 478);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18a0ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f8a6f0: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8a6f0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823be048u, 532);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f8a860: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8a860(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823be0a8u, 553);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f8a9d0: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8a9d0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823be108u, 574);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x1810ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f8ab40: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8ab40(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823be168u, 595);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18a0ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

// sk83_na_m 0x82f8acb0: const+tex binder (effect type 0x07ea02f5)
void Binder_82f8acb0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x7ea02f5u))
    {
        BINDER_ASSERT(0x823be1c8u, 616);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t5 = BLD32(b + 0xc);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0x10), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0x10) + 0x7ull), 30, 2, 31) - 0x2ull), 0) & 0xffffffffull)), 62) & 0x3fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    unsigned long long t7 = (BLD32(0x833a5ea0u) + 0x18a0ull);
    unsigned long long t8 = BLD32(b + 0x8);
    BST64(BLD32(0x833a5ea0u) + 0x8, (BLD64(BLD32(0x833a5ea0u) + 0x8) | 0x800000000000000ull));
    BINDER_XMEMCPY(t7, t8, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x14)), 0x3u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x18)), 0x4u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x1c)), 0x6u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x20)), 0x5u);
    unsigned long long t9 = (BLD32(0x833c31c0u) + 0x1ull);
    unsigned long long t10 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x4ull));
    BST32(0x833c31bcu, t10);
    BST32(0x833c31c0u, t9);
}

} }
