// build/livesk8/sk8shader/dev/xbox2/final/gen/source/WorldShadow.cpp (sk83 assert path; the shader
// compiler generated the original). 1 effect binders, generated from sk83_na_m by
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

// sk83_na_m 0x82f62e88: const binder (effect type 0xd6b08a45)
void Binder_82f62e88(unsigned int binding)
{
    const unsigned long long b = binding;
    RWP_SAVEGPRLR(31);
    RWP_GUEST_FRAME(frame, 0x60);
    RWP_NV(31, b);
    const unsigned int t1 = RWP_VCALL((unsigned int)b, 0x4);
    if ((unsigned int)(t1) != (unsigned int)(0xffffffffd6b08a45ull))
    {
        BINDER_ASSERT(0x823cbaa0u, 19);
    }
    unsigned long long t2 = (BLD32(0x833a5ea0u) + 0x780ull);
    unsigned long long t3 = BLD32(BLD32(b + 0x4) + 0x64);
    BST64(BLD32(0x833a5ea0u), (BLD64(BLD32(0x833a5ea0u)) | 0x8000000000000000ull));
    BINDER_XMEMCPY(t2, t3, 0x40u);
    BST32(0x833c31bcu, (BLD32(0x833c31bcu) + 0x1ull));
}

} }
