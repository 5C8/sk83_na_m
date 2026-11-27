// build/livesk8/sk8shader/dev/xbox2/final/gen/source/samplerbinders.cpp (sk83 assert path; the shader
// compiler generated the original). 90 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f64d68: samp binder
void Binder_82f64d68(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3134u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
    }
    unsigned long long t3 = BLD32(0x833c313cu);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x0u);
        BST32(0x833c3d50u, t3);
    }
}

// sk83_na_m 0x82f64df0: samp binder
void Binder_82f64df0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3134u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3140u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x2u);
        BST32(0x833c3d58u, t3);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
}

// sk83_na_m 0x82f66780: samp binder
void Binder_82f66780(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    unsigned long long t4 = BLD32(0x833c3138u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
}

// sk83_na_m 0x82f67588: samp binder
void Binder_82f67588(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t3 = BLD32(0x833c313cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x0u);
        BST32(0x833c3d50u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t4 = BLD32(0x833c3134u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x3u);
        BST32(0x833c3d5cu, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
    }
}

// sk83_na_m 0x82f67698: samp binder
void Binder_82f67698(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x8u);
        BST32(0x833c3d70u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
    }
}

// sk83_na_m 0x82f68190: samp binder
void Binder_82f68190(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t3 = BLD32(0x833c3150u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x9u);
        BST32(0x833c3d74u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
    }
}

// sk83_na_m 0x82f682c8: samp binder
void Binder_82f682c8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t4 = BLD32(0x833c3150u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x9u);
        BST32(0x833c3d74u, t4);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x8u);
        BST32(0x833c3d70u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(28, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
    }
}

// sk83_na_m 0x82f68da8: samp binder
void Binder_82f68da8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3138u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3150u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
    }
}

// sk83_na_m 0x82f68e90: samp binder
void Binder_82f68e90(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
    }
    unsigned long long t4 = BLD32(0x833c3150u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x7u);
        BST32(0x833c3d6cu, t4);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(29, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(30, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
    }
}

// sk83_na_m 0x82f69ab8: samp binder
void Binder_82f69ab8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x9u);
        BST32(0x833c3d74u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
        t2 = BLD32(0x833c3138u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
        t2 = BLD32(0x833c3138u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t3 = BLD32(0x833c313cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x0u);
        BST32(0x833c3d50u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t4 = BLD32(0x833c3134u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x3u);
        BST32(0x833c3d5cu, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d78u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0xau);
        BST32(0x833c3d78u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
    }
}

// sk83_na_m 0x82f69c10: samp binder
void Binder_82f69c10(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x9u);
        BST32(0x833c3d74u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
        t3 = BLD32(0x833c3138u);
        RWP_NV(28, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x8u);
        BST32(0x833c3d70u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
        t3 = BLD32(0x833c3138u);
        RWP_NV(28, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d78u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0xau);
        BST32(0x833c3d78u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
    }
}

// sk83_na_m 0x82f6af40: samp binder
void Binder_82f6af40(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c0000ull);
    unsigned long long t1 = BLD32(0x833c3158u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
}

// sk83_na_m 0x82f6bca0: samp binder
void Binder_82f6bca0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c315cu);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
        t1 = BLD32(0x833c315cu);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
}

// sk83_na_m 0x82f6bd08: samp binder
void Binder_82f6bd08(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c0000ull);
    unsigned long long t1 = BLD32(0x833c315cu);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
}

// sk83_na_m 0x82f6c478: samp binder
void Binder_82f6c478(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    unsigned long long t2 = BLD32(0x833c3150u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x9u);
        BST32(0x833c3d74u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    unsigned long long t3 = BLD32(0x833c313cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x0u);
        BST32(0x833c3d50u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    unsigned long long t4 = BLD32(0x833c3134u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x3u);
        BST32(0x833c3d5cu, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
    }
}

// sk83_na_m 0x82f6ca00: samp binder
void Binder_82f6ca00(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c313cu);
    RWP_NV(31, t2);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3134u);
    RWP_NV(31, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
}

// sk83_na_m 0x82f6cf40: samp binder
void Binder_82f6cf40(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    unsigned long long t2 = BLD32(0x833c313cu);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    unsigned long long t3 = BLD32(0x833c3134u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
    }
    unsigned long long t4 = BLD32(0x833c3160u);
    RWP_NV(31, t4);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x9u);
        BST32(0x833c3d74u, t4);
    }
}

// sk83_na_m 0x82f6f7c8: samp binder
void Binder_82f6f7c8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3148u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x6u);
        BST32(0x833c3d68u, t4);
        t4 = BLD32(0x833c3148u);
        RWP_NV(29, t4);
    }
    unsigned long long t5 = BLD32(0x833c3168u);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x5u);
        BST32(0x833c3d64u, t5);
        t4 = BLD32(0x833c3148u);
        RWP_NV(29, t4);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t6 = BLD32(0x833c3144u);
    RWP_NV(30, t6);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x0u);
        BST32(0x833c3d50u, t6);
    }
}

// sk83_na_m 0x82f6f8d0: samp binder
void Binder_82f6f8d0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t4 = BLD32(0x833c3168u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x5u);
        BST32(0x833c3d64u, t4);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f70fd8: samp binder
void Binder_82f70fd8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3168u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x5u);
        BST32(0x833c3d64u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3148u);
    RWP_NV(29, t5);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x4u);
        BST32(0x833c3d60u, t5);
    }
    unsigned long long t6 = BLD32(0x833c3144u);
    RWP_NV(30, t6);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x0u);
        BST32(0x833c3d50u, t6);
    }
}

// sk83_na_m 0x82f710c0: samp binder
void Binder_82f710c0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3168u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3144u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x0u);
        BST32(0x833c3d50u, t3);
        t3 = BLD32(0x833c3144u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x1u);
        BST32(0x833c3d54u, t3);
    }
    unsigned long long t4 = BLD32(0x833c3164u);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x3u);
        BST32(0x833c3d5cu, t4);
    }
}

// sk83_na_m 0x82f72e50: samp binder
void Binder_82f72e50(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t3 = BLD32(0x833c3164u);
        RWP_NV(29, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3148u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x7u);
        BST32(0x833c3d6cu, t4);
        t4 = BLD32(0x833c3148u);
        RWP_NV(29, t4);
    }
    unsigned long long t5 = BLD32(0x833c316cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x6u);
        BST32(0x833c3d68u, t5);
        t4 = BLD32(0x833c3148u);
        RWP_NV(29, t4);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t6 = BLD32(0x833c3144u);
    RWP_NV(30, t6);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x0u);
        BST32(0x833c3d50u, t6);
    }
}

// sk83_na_m 0x82f72f78: samp binder
void Binder_82f72f78(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
        t1 = BLD32(0x833c3164u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(BLD32(0x833c3144u)))
    {
        BINDER_SETSAMPLER((unsigned long long)RwpRlwinm((unsigned int)BLD32(0x833c3144u), 0, 0, 31), 0x1u);
        BST32(0x833c3d54u, t2);
        t1 = BLD32(0x833c3164u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    unsigned long long t4 = BLD32(0x833c316cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x6u);
        BST32(0x833c3d68u, t4);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x8u);
        BST32(0x833c3d70u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x9u);
        BST32(0x833c3d74u, t3);
    }
    unsigned long long t5 = BLD32(0x833c3144u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(BLD32(0x833c3144u)))
    {
        BINDER_SETSAMPLER((unsigned long long)RwpRlwinm((unsigned int)BLD32(0x833c3144u), 0, 0, 31), 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f730d0: samp binder
void Binder_82f730d0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
        t1 = BLD32(0x833c3164u);
        RWP_NV(29, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
        t1 = BLD32(0x833c3164u);
        RWP_NV(29, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t4 = BLD32(0x833c316cu);
    RWP_NV(27, t4);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x6u);
        BST32(0x833c3d68u, t4);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f74618: samp binder
void Binder_82f74618(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f74708: samp binder
void Binder_82f74708(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3148u);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x5u);
        BST32(0x833c3d64u, t4);
        t4 = BLD32(0x833c3148u);
        RWP_NV(30, t4);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
        t4 = BLD32(0x833c3148u);
        RWP_NV(30, t4);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x6u);
        BST32(0x833c3d68u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3144u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f75bf0: samp binder
void Binder_82f75bf0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3148u);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x5u);
        BST32(0x833c3d64u, t4);
        t4 = BLD32(0x833c3148u);
        RWP_NV(30, t4);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3144u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f75cd8: samp binder
void Binder_82f75cd8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(28, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
}

// sk83_na_m 0x82f78db8: samp binder
void Binder_82f78db8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t3 = BLD32(0x833c3164u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3148u);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x6u);
        BST32(0x833c3d68u, t4);
        t4 = BLD32(0x833c3148u);
        RWP_NV(30, t4);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3144u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f78ec0: samp binder
void Binder_82f78ec0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
        t1 = BLD32(0x833c3164u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(BLD32(0x833c3144u)))
    {
        BINDER_SETSAMPLER((unsigned long long)RwpRlwinm((unsigned int)BLD32(0x833c3144u), 0, 0, 31), 0x1u);
        BST32(0x833c3d54u, t2);
        t1 = BLD32(0x833c3164u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x8u);
        BST32(0x833c3d70u, t3);
    }
    unsigned long long t4 = BLD32(0x833c3144u);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(BLD32(0x833c3144u)))
    {
        BINDER_SETSAMPLER((unsigned long long)RwpRlwinm((unsigned int)BLD32(0x833c3144u), 0, 0, 31), 0x0u);
        BST32(0x833c3d50u, t4);
    }
}

// sk83_na_m 0x82f78ff0: samp binder
void Binder_82f78ff0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
        t2 = BLD32(0x833c3144u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
}

// sk83_na_m 0x82f79078: samp binder
void Binder_82f79078(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
        t1 = BLD32(0x833c3164u);
        RWP_NV(29, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
        t1 = BLD32(0x833c3164u);
        RWP_NV(29, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f7a070: samp binder
void Binder_82f7a070(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
}

// sk83_na_m 0x82f7cd90: samp binder
void Binder_82f7cd90(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, 0xffffffff833c3130ull);
    RWP_NV(31, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
        t1 = BLD32(0x833c3144u);
        RWP_NV(29, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3164u);
        RWP_NV(28, t2);
        t1 = BLD32(0x833c3144u);
        RWP_NV(29, t1);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(27, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t2 = BLD32(0x833c3164u);
        RWP_NV(28, t2);
        t1 = BLD32(0x833c3144u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
        t2 = BLD32(0x833c3164u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f7ce68: samp binder
void Binder_82f7ce68(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
        t3 = BLD32(0x833c3164u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
    }
    unsigned long long t4 = BLD32(0x833c3148u);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3144u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f7cf50: samp binder
void Binder_82f7cf50(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
}

// sk83_na_m 0x82f7cfb0: samp binder
void Binder_82f7cfb0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3130ull);
    RWP_NV(31, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
}

// sk83_na_m 0x82f7d038: samp binder
void Binder_82f7d038(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
        t1 = BLD32(0x833c3164u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
        t1 = BLD32(0x833c3164u);
        RWP_NV(30, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f7d150: samp binder
void Binder_82f7d150(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(29, 0xffffffff833c3130ull);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
        t1 = BLD32(0x833c3144u);
        RWP_NV(31, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
        t1 = BLD32(0x833c3144u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
    }
}

// sk83_na_m 0x82f7d9b8: samp binder
void Binder_82f7d9b8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3170u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3174u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f7e4a0: samp binder
void Binder_82f7e4a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d78u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0xau);
        BST32(0x833c3d78u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x9u);
        BST32(0x833c3d74u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t3 = BLD32(0x833c3178u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t4 = BLD32(0x833c314cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(28, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
    }
}

// sk83_na_m 0x82f7e5f8: samp binder
void Binder_82f7e5f8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d78u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0xau);
        BST32(0x833c3d78u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x9u);
        BST32(0x833c3d74u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t4 = BLD32(0x833c3178u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x5u);
        BST32(0x833c3d64u, t4);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t5 = BLD32(0x833c314cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x4u);
        BST32(0x833c3d60u, t5);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t6 = BLD32(0x833c313cu);
    RWP_NV(28, t6);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x0u);
        BST32(0x833c3d50u, t6);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t7 = BLD32(0x833c3134u);
    RWP_NV(28, t7);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t7))
    {
        BINDER_SETSAMPLER(t7, 0x3u);
        BST32(0x833c3d5cu, t7);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x8u);
        BST32(0x833c3d70u, t2);
    }
}

// sk83_na_m 0x82f7f178: samp binder
void Binder_82f7f178(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x9u);
        BST32(0x833c3d74u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t3 = BLD32(0x833c314cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
    }
}

// sk83_na_m 0x82f7f2b0: samp binder
void Binder_82f7f2b0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x9u);
        BST32(0x833c3d74u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x8u);
        BST32(0x833c3d70u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t4 = BLD32(0x833c314cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(28, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
    }
}

// sk83_na_m 0x82f7fdd0: samp binder
void Binder_82f7fdd0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t3 = BLD32(0x833c314cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
    }
}

// sk83_na_m 0x82f7fec0: samp binder
void Binder_82f7fec0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
    }
    unsigned long long t4 = BLD32(0x833c314cu);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(29, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(30, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
    }
}

// sk83_na_m 0x82f80990: samp binder
void Binder_82f80990(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t3 = BLD32(0x833c3178u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
    }
}

// sk83_na_m 0x82f80a80: samp binder
void Binder_82f80a80(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
    }
    unsigned long long t4 = BLD32(0x833c3178u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(29, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(30, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
    }
}

// sk83_na_m 0x82f81590: samp binder
void Binder_82f81590(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x9u);
        BST32(0x833c3d74u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t3 = BLD32(0x833c3178u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
    }
}

// sk83_na_m 0x82f816c8: samp binder
void Binder_82f816c8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x9u);
        BST32(0x833c3d74u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x8u);
        BST32(0x833c3d70u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t4 = BLD32(0x833c3178u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(28, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
    }
}

// sk83_na_m 0x82f82208: samp binder
void Binder_82f82208(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d78u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0xau);
        BST32(0x833c3d78u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
        t2 = BLD32(0x833c3138u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x9u);
        BST32(0x833c3d74u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
        t2 = BLD32(0x833c3138u);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c314cu);
    RWP_NV(27, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
        t2 = BLD32(0x833c3138u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
        t1 = BLD32(0x833c3148u);
        RWP_NV(29, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
    }
}

// sk83_na_m 0x82f82368: samp binder
void Binder_82f82368(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d78u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0xau);
        BST32(0x833c3d78u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
        t3 = BLD32(0x833c3138u);
        RWP_NV(28, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d74u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x9u);
        BST32(0x833c3d74u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
        t3 = BLD32(0x833c3138u);
        RWP_NV(28, t3);
    }
    unsigned long long t4 = BLD32(0x833c314cu);
    RWP_NV(27, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
        t3 = BLD32(0x833c3138u);
        RWP_NV(28, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(28, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x8u);
        BST32(0x833c3d70u, t2);
    }
}

// sk83_na_m 0x82f83e30: samp binder
void Binder_82f83e30(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
}

// sk83_na_m 0x82f85940: samp binder
void Binder_82f85940(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(28, 0xffffffff833c3130ull);
    RWP_NV(31, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
        t1 = BLD32(0x833c3144u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t2 = BLD32(0x833c3164u);
        RWP_NV(29, t2);
        t1 = BLD32(0x833c3144u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
        t2 = BLD32(0x833c3164u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f859f0: samp binder
void Binder_82f859f0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, 0xffffffff833c3130ull);
    RWP_NV(31, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t2 = BLD32(0x833c3164u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c314cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x1u);
        BST32(0x833c3d54u, t3);
        t2 = BLD32(0x833c3164u);
        RWP_NV(29, t2);
    }
    unsigned long long t4 = BLD32(0x833c3140u);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x2u);
        BST32(0x833c3d58u, t4);
        t2 = BLD32(0x833c3164u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f85ac0: samp binder
void Binder_82f85ac0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(29, 0xffffffff833c3130ull);
    RWP_NV(31, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t2 = BLD32(0x833c3164u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f86658: samp binder
void Binder_82f86658(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3134u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
}

// sk83_na_m 0x82f87580: samp binder
void Binder_82f87580(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3164u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
}

// sk83_na_m 0x82f87608: samp binder
void Binder_82f87608(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f8ae20: samp binder
void Binder_82f8ae20(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3138u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(28, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
}

// sk83_na_m 0x82f8af08: samp binder
void Binder_82f8af08(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3138u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t3 = BLD32(0x833c3148u);
        RWP_NV(30, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
        t2 = BLD32(0x833c3144u);
        RWP_NV(28, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f8ced0: samp binder
void Binder_82f8ced0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
}

// sk83_na_m 0x82f8f3d0: samp binder
void Binder_82f8f3d0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x7u);
        BST32(0x833c3d6cu, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
        t1 = BLD32(0x833c3148u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
}

// sk83_na_m 0x82f8f4a0: samp binder
void Binder_82f8f4a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(27);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
        t2 = BLD32(0x833c314cu);
        RWP_NV(28, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t2 = BLD32(0x833c314cu);
        RWP_NV(28, t2);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
    }
    unsigned long long t4 = BLD32(0x833c3138u);
    RWP_NV(27, t4);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x5u);
        BST32(0x833c3d64u, t4);
        t2 = BLD32(0x833c314cu);
        RWP_NV(28, t2);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x6u);
        BST32(0x833c3d68u, t3);
        t2 = BLD32(0x833c314cu);
        RWP_NV(28, t2);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(28, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
        t3 = BLD32(0x833c3148u);
        RWP_NV(29, t3);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
    }
}

// sk83_na_m 0x82f918b0: samp binder
void Binder_82f918b0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f91918: samp binder
void Binder_82f91918(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c314cu);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x1u);
        BST32(0x833c3d54u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3140u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x2u);
        BST32(0x833c3d58u, t4);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f92410: samp binder
void Binder_82f92410(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3138u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    unsigned long long t3 = BLD32(0x833c317cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x7u);
        BST32(0x833c3d6cu, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
    }
}

// sk83_na_m 0x82f93558: samp binder
void Binder_82f93558(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3180u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
}

// sk83_na_m 0x82f93758: samp binder
void Binder_82f93758(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3134u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f93c98: samp binder
void Binder_82f93c98(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c0000ull);
    unsigned long long t1 = BLD32(0x833c3184u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
}

// sk83_na_m 0x82f94c88: samp binder
void Binder_82f94c88(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(29, 0xffffffff833c3130ull);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
        t1 = BLD32(0x833c3144u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(31, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f94d10: samp binder
void Binder_82f94d10(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3164u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
    unsigned long long t2 = BLD32(0x833c3144u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x0u);
        BST32(0x833c3d50u, t2);
    }
}

// sk83_na_m 0x82f95310: samp binder
void Binder_82f95310(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c0000ull);
    unsigned long long t1 = BLD32(0x833c3188u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
}

// sk83_na_m 0x82f95650: samp binder
void Binder_82f95650(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(30, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    unsigned long long t2 = BLD32(0x833c317cu);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x6u);
        BST32(0x833c3d68u, t1);
        t1 = BLD32(0x833c3148u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t3 = BLD32(0x833c3134u);
    RWP_NV(31, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
}

// sk83_na_m 0x82f96c20: samp binder
void Binder_82f96c20(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c318cu);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x8u);
        BST32(0x833c3d70u, t1);
    }
}

// sk83_na_m 0x82f98390: samp binder
void Binder_82f98390(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c0000ull);
    unsigned long long t1 = BLD32(0x833c3190u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
    }
}

// sk83_na_m 0x82f995c8: samp binder
void Binder_82f995c8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3194u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3198u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
        t2 = BLD32(0x833c3198u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
    }
}

// sk83_na_m 0x82f99a48: samp binder
void Binder_82f99a48(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c319cu);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
}

// sk83_na_m 0x82f99f18: samp binder
void Binder_82f99f18(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3138u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
}

// sk83_na_m 0x82f9b7c8: samp binder
void Binder_82f9b7c8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3134u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
}

// sk83_na_m 0x82f9b870: samp binder
void Binder_82f9b870(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3134u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
    }
    unsigned long long t4 = BLD32(0x833c3140u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x2u);
        BST32(0x833c3d58u, t4);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f9c180: samp binder
void Binder_82f9c180(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3148u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3150u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3134u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x3u);
        BST32(0x833c3d5cu, t3);
    }
    unsigned long long t4 = BLD32(0x833c3138u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x4u);
        BST32(0x833c3d60u, t4);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
}

// sk83_na_m 0x82f9c248: samp binder
void Binder_82f9c248(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3140u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3138u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
    }
    unsigned long long t3 = BLD32(0x833c3148u);
    RWP_NV(29, t3);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x5u);
        BST32(0x833c3d64u, t3);
    }
    unsigned long long t4 = BLD32(0x833c3150u);
    RWP_NV(29, t4);
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x6u);
        BST32(0x833c3d68u, t4);
    }
    unsigned long long t5 = BLD32(0x833c313cu);
    RWP_NV(29, t5);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x0u);
        BST32(0x833c3d50u, t5);
    }
    unsigned long long t6 = BLD32(0x833c3134u);
    RWP_NV(30, t6);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t6))
    {
        BINDER_SETSAMPLER(t6, 0x3u);
        BST32(0x833c3d5cu, t6);
    }
}

// sk83_na_m 0x82f9c5d8: samp binder
void Binder_82f9c5d8(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3154u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x3u);
        BST32(0x833c3d5cu, t1);
    }
}

// sk83_na_m 0x82f9ca40: samp binder
void Binder_82f9ca40(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(30, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3184u);
    RWP_NV(29, t1);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x5u);
        BST32(0x833c3d64u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3134u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
    unsigned long long t3 = BLD32(0x833c3138u);
    RWP_NV(30, t3);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x4u);
        BST32(0x833c3d60u, t3);
    }
}

// sk83_na_m 0x82f9d938: samp binder
void Binder_82f9d938(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(28, 0xffffffff833c3130ull);
    RWP_NV(31, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
        t1 = BLD32(0x833c3144u);
        RWP_NV(30, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(29, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3164u);
        RWP_NV(29, t2);
        t1 = BLD32(0x833c3144u);
        RWP_NV(30, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
        t2 = BLD32(0x833c3164u);
        RWP_NV(29, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f9e5a0: samp binder
void Binder_82f9e5a0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(29, 0xffffffff833c3130ull);
    RWP_NV(30, 0xffffffff833c3d50ull);
    unsigned long long t1 = BLD32(0x833c3144u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
        t1 = BLD32(0x833c3144u);
        RWP_NV(31, t1);
    }
    unsigned long long t2 = BLD32(0x833c3164u);
    RWP_NV(28, t2);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x4u);
        BST32(0x833c3d60u, t2);
        t1 = BLD32(0x833c3144u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
    }
}

// sk83_na_m 0x82f9e830: samp binder
void Binder_82f9e830(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3138u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3134u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x3u);
        BST32(0x833c3d5cu, t2);
    }
}

// sk83_na_m 0x82f9f078: samp binder
void Binder_82f9f078(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(29);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(30, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c31a0u);
    RWP_NV(31, t1);
    if ((unsigned int)(BLD32(0x833c3d58u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x2u);
        BST32(0x833c3d58u, t1);
        t1 = BLD32(0x833c31a0u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x0u);
        BST32(0x833c3d50u, t1);
        t1 = BLD32(0x833c31a0u);
        RWP_NV(31, t1);
    }
    if ((unsigned int)(BLD32(0x833c3d54u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x1u);
        BST32(0x833c3d54u, t1);
    }
}

// sk83_na_m 0x82f9f5b0: samp binder
void Binder_82f9f5b0(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(28);
    RWP_GUEST_FRAME(frame, 0x80);
    RWP_NV(31, 0xffffffff833c3d50ull);
    RWP_NV(29, 0xffffffff833c3130ull);
    unsigned long long t1 = BLD32(0x833c3138u);
    RWP_NV(30, t1);
    if ((unsigned int)(BLD32(0x833c3d60u)) != (unsigned int)(t1))
    {
        BINDER_SETSAMPLER(t1, 0x4u);
        BST32(0x833c3d60u, t1);
    }
    unsigned long long t2 = BLD32(0x833c3148u);
    RWP_NV(30, t2);
    if ((unsigned int)(BLD32(0x833c3d64u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x5u);
        BST32(0x833c3d64u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    unsigned long long t3 = BLD32(0x833c317cu);
    RWP_NV(28, t3);
    if ((unsigned int)(BLD32(0x833c3d70u)) != (unsigned int)(t3))
    {
        BINDER_SETSAMPLER(t3, 0x8u);
        BST32(0x833c3d70u, t3);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d68u)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x6u);
        BST32(0x833c3d68u, t2);
        t2 = BLD32(0x833c3148u);
        RWP_NV(30, t2);
    }
    if ((unsigned int)(BLD32(0x833c3d6cu)) != (unsigned int)(t2))
    {
        BINDER_SETSAMPLER(t2, 0x7u);
        BST32(0x833c3d6cu, t2);
    }
    unsigned long long t4 = BLD32(0x833c313cu);
    RWP_NV(30, t4);
    if ((unsigned int)(BLD32(0x833c3d50u)) != (unsigned int)(t4))
    {
        BINDER_SETSAMPLER(t4, 0x0u);
        BST32(0x833c3d50u, t4);
    }
    unsigned long long t5 = BLD32(0x833c3134u);
    RWP_NV(30, t5);
    if ((unsigned int)(BLD32(0x833c3d5cu)) != (unsigned int)(t5))
    {
        BINDER_SETSAMPLER(t5, 0x3u);
        BST32(0x833c3d5cu, t5);
    }
}

} }
