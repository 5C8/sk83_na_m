// Buffer touch events for the world-item cache (skate3_native_render_scene_static_persist).
//
// The native scene caches each world mesh's walk (buffer addresses, sizes, a payload
// fingerprint) and used to POLL guest memory to notice changes: structural + buffer checks
// every other frame and a fingerprint every 4th, per item, per frame -- the dominant cost of
// the scene build in dense views. These hooks report every game path that can (re)point,
// fill or free a vertex / index buffer, so the cache only re-checks buffers something
// actually touched. Mode 2 (audit) keeps polling and logs every change the poll caught that
// no hook reported; event mode is only safe once that count stays 0.
//
// Object identity: a renderengine::VertexBuffer / IndexBuffer starts with the D3D buffer
// header (fetch / address words at +0x18, what the cache validates), so the D3D lock's
// buffer argument and the renderengine object are the same address.
#include "generated/sk83_na_m_init.h"

#include <cstdint>

namespace skate3::native_scene {
void NoteBufferDirty(uint32_t obj);
}

namespace {
inline void Touch(uint32_t obj) { skate3::native_scene::NoteBufferDirty(obj); }
}  // namespace

// The hook is reported both before (pointer known) and after (writes done) the original.
#define BUFFER_EVENT_HOOK(sym)                \
  extern "C" REX_FUNC(sym) {                  \
    const uint32_t obj = ctx.r3.u32;          \
    Touch(obj);                               \
    __imp__##sym(ctx, base);                  \
    Touch(obj);                               \
  }

// XDK D3D runtime: CPU access to the buffer contents.
BUFFER_EVENT_HOOK(rex_D3DVertexBuffer_Lock_82D96BB0)
BUFFER_EVENT_HOOK(rex_D3DIndexBuffer_Lock_82D96C00)
BUFFER_EVENT_HOOK(rex_D3DVertexBuffer_Unlock_82D95C70)
BUFFER_EVENT_HOOK(rex_D3DIndexBuffer_Unlock_82D95C80)

// renderengine wrappers (this = r3).
BUFFER_EVENT_HOOK(rex_renderengine_VertexBuffer_Lock_QAA_NIAAVBatchIterator_12_II_Z_829087B8)
BUFFER_EVENT_HOOK(rex_renderengine_IndexBuffer_Lock_QAA_NIAAULocked_12_Z_82C71820)
BUFFER_EVENT_HOOK(rex_renderengine_VertexBuffer_Release_QAAXXZ_82C74D50)
BUFFER_EVENT_HOOK(rex_renderengine_IndexBuffer_Release_QAAXXZ_82C73A70)
BUFFER_EVENT_HOOK(rex_renderengine_VertexBuffer_Destruct_SAXPAV12_Z_82C74CC8)
BUFFER_EVENT_HOOK(rex_renderengine_IndexBuffer_Destruct_SAXPAV12_Z_82C737C8)

// Arena load fixup of a vertex buffer object (r3 = the object).
BUFFER_EVENT_HOOK(rex_renderengine_VertexBuffer_Fixup_SAIPAXPAVArenaIterator_arena_core_rw_Z_82C76B68)

// Initialize(Resource const& r3, Parameters const& r4): the object is the resource's first
// word; returns it in r3.
#define BUFFER_INIT_HOOK(sym)                                  \
  extern "C" REX_FUNC(sym) {                                   \
    const uint32_t obj = ctx.r3.u32 ? REX_LOAD_U32(ctx.r3.u32) : 0u; \
    Touch(obj);                                                \
    __imp__##sym(ctx, base);                                   \
    Touch(obj);                                                \
    Touch(ctx.r3.u32);                                         \
  }
BUFFER_INIT_HOOK(rex_renderengine_VertexBuffer_Initialize_SAPAV12_ABVResource_rw_ABUParameters_12_Z_82C74BC8)
BUFFER_INIT_HOOK(rex_renderengine_IndexBuffer_Initialize_SAPAV12_ABVResource_rw_ABUParameters_12_Z_82C73968)
