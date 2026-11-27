// build/livesk8/sk8shader/dev/xbox2/final/gen/source/font.cpp (sk83 assert path; the shader
// compiler generated the original). 12 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f59ab0: const binder (effect type 0x6b981dea)
void Binder_82f59ab0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x6b981deau))
    {
        BINDER_ASSERT(0x823c1c80u, 19);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f59b50: const binder (effect type 0x6b981dea)
void Binder_82f59b50(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x6b981deau))
    {
        BINDER_ASSERT(0x823c1cd8u, 35);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f59bf0: const binder (effect type 0x6b981dea)
void Binder_82f59bf0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x6b981deau))
    {
        BINDER_ASSERT(0x823c1d30u, 51);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f59c90: const binder (effect type 0x6b981dea)
void Binder_82f59c90(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x6b981deau))
    {
        BINDER_ASSERT(0x823c1d88u, 67);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f59d30: const binder (effect type 0x6b981dea)
void Binder_82f59d30(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x6b981deau))
    {
        BINDER_ASSERT(0x823c1de0u, 83);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f59dd0: const binder (effect type 0x6b981dea)
void Binder_82f59dd0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0x6b981deau))
    {
        BINDER_ASSERT(0x823c1e38u, 99);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

// sk83_na_m 0x82f92968: const+tex binder (effect type 0x98811b58)
void Binder_82f92968(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff98811b58ull))
    {
        BINDER_ASSERT(0x823c1e90u, 145);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t5 = BLD32(b + 0x8);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0xc), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0xc) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
}

// sk83_na_m 0x82f92a70: const+tex binder (effect type 0x98811b58)
void Binder_82f92a70(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff98811b58ull))
    {
        BINDER_ASSERT(0x823c1ee8u, 162);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t5 = BLD32(b + 0x8);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0xc), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0xc) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
}

// sk83_na_m 0x82f92b78: const+tex binder (effect type 0x98811b58)
void Binder_82f92b78(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff98811b58ull))
    {
        BINDER_ASSERT(0x823c1f40u, 179);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t5 = BLD32(b + 0x8);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0xc), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0xc) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
}

// sk83_na_m 0x82f92c80: const+tex binder (effect type 0x98811b58)
void Binder_82f92c80(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff98811b58ull))
    {
        BINDER_ASSERT(0x823c1f98u, 196);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t5 = BLD32(b + 0x8);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0xc), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0xc) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
}

// sk83_na_m 0x82f92d88: const+tex binder (effect type 0x98811b58)
void Binder_82f92d88(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff98811b58ull))
    {
        BINDER_ASSERT(0x823c1ff0u, 213);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t5 = BLD32(b + 0x8);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0xc), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0xc) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
}

// sk83_na_m 0x82f92e90: const+tex binder (effect type 0x98811b58)
void Binder_82f92e90(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffff98811b58ull))
    {
        BINDER_ASSERT(0x823c2048u, 230);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x800ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x2000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    unsigned long long t4 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t5 = BLD32(b + 0x8);
    unsigned long long t6 = (unsigned long long)RwpRlwinm((unsigned int)BLD32(b + 0xc), 4, 0, 27);
    BST64(BLD32(0x833a5ea0u), ((RwpRotl64(RwpSrad(0x8000000000000000ull, (RwpRotl64(((unsigned long long)RwpRlwinm((unsigned int)(BLD32(b + 0xc) + 0x3ull), 30, 2, 31) - 0x1ull), 0) & 0xffffffffull)), 63) & 0x7fffffffffffffffull) | BLD64(BLD32(0x833a5ea0u))));
    BINDER_XMEMCPY(t4, t5, t6);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x10)), 0x0u);
    unsigned long long t7 = (BLD32(0x833c31bcu) + 0x2ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t7);
}

} }
