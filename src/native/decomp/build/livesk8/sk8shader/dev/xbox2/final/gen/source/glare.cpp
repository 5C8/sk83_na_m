// build/livesk8/sk8shader/dev/xbox2/final/gen/source/glare.cpp (sk83 assert path; the shader
// compiler generated the original). 2 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f59f00: const binder (effect type 0xca1c5842)
void Binder_82f59f00(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(30);
    RWP_GUEST_FRAME(frame, 0x70);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffca1c5842ull))
    {
        BINDER_ASSERT(0x823c24b8u, 19);
    }
    RWP_NV(30, 0xffffffff833a0000ull);
    unsigned long long t2 = BLD32(b + 0x4);
    RWP_NV(31, t2);
    unsigned long long t3 = (BLD32(0x833a5ea0u) + 0x7c0ull);
    unsigned long long t4 = BLD32(t2 + 0x60);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t3, t4, 0x30u);
    unsigned long long t5 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t6 = BLD32(t2 + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t5, t6, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x2ull));
}

// sk83_na_m 0x82f93498: const+tex binder (effect type 0xa872d160)
void Binder_82f93498(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffa872d160ull))
    {
        BINDER_ASSERT(0x823c2510u, 41);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x7f0ull);
    unsigned long long t3 = BLD32(b + 0x4);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x4000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x10u);
    BINDER_SETTEXTURE(BLD32(BLD32(b + 0x8)), 0x3u);
    unsigned long long t4 = (BLD32(0x833c31bcu) + 0x1ull);
    BST32(0x833c31b0u, (BLD32(0x833c31b0u) + 0x1ull));
    BST32(0x833c31bcu, t4);
}

} }
