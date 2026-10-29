// Full PC-native renderer: replaces the guest's D3D9-on-360 CDevice path
// entirely. No IGraphicsSystem / CommandProcessor / RegisterFile / xenos
// PM4 processing is involved anywhere in this file.
//
// !!! IMPORTANT CAVEAT ON EVERY HOOK BELOW !!!
// FunctionDispatcher::SetFunction fully replaces the guest function's
// table entry -- the original recompiled instructions never run once a
// guest address is overridden this way. That means the guest's own
// internal bookkeeping in these functions (e.g. SetVertexDeclaration
// storing the pointer at CDevice+11992 and OR'ing a dirty bit at
// CDevice+16) never happens. That's fine as long as *only* other
// D3DDevice_*/CDevice code would have read those fields -- a safe bet for
// pure pipeline state like this -- but is a real risk for anything
// unrelated guest code might inspect directly (texture width/height being
// the known example from the old-XEX decompile's GetTextureDimensions).
// Flagging per-hook below where this matters.

#include "skate3_pc_renderer.h"
#include "skate3_shader_translation.h"

#include <atomic>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <vector>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

#include <rex/cvar.h>
#include <rex/runtime.h>
#include <rex/ppc/context.h>
#include <rex/system/function_dispatcher.h>  // FunctionDispatcher::SetFunction

using Microsoft::WRL::ComPtr;

namespace {

// --- Window lookup: same heuristic used in the other skate3_* files ------

HWND FindGameWindow() {
  static HWND cached = nullptr;
  if (cached && IsWindow(cached)) return cached;
  struct Ctx { HWND best; DWORD pid; LONG best_area; };
  Ctx ctx{nullptr, GetCurrentProcessId(), 0};
  EnumWindows([](HWND hwnd, LPARAM lp) -> BOOL {
    auto* c = (Ctx*)lp;
    DWORD wpid = 0;
    GetWindowThreadProcessId(hwnd, &wpid);
    if (wpid != c->pid) return TRUE;
    if (!IsWindowVisible(hwnd)) return TRUE;
    RECT r;
    if (!GetClientRect(hwnd, &r)) return TRUE;
    LONG area = (r.right - r.left) * (r.bottom - r.top);
    if (area > c->best_area) { c->best_area = area; c->best = hwnd; }
    return TRUE;
  }, (LPARAM)&ctx);
  cached = ctx.best;
  return cached;
}

// --- D3D11 device state ---------------------------------------------------

struct D3D11State {
  ComPtr<ID3D11Device> device;
  ComPtr<ID3D11DeviceContext> context;
  ComPtr<IDXGISwapChain> swapchain;
  ComPtr<ID3D11RenderTargetView> backbuffer_rtv;
  HWND hwnd = nullptr;
  bool initialized = false;
};

D3D11State g_d3d;
std::mutex g_d3d_mutex;

bool EnsureD3D11Initialized() {
  std::lock_guard<std::mutex> lock(g_d3d_mutex);
  if (g_d3d.initialized) return true;

  HWND hwnd = FindGameWindow();
  if (!hwnd) return false;  // caller retries later -- window not up yet

  RECT rc{};
  GetClientRect(hwnd, &rc);
  UINT width = static_cast<UINT>(rc.right - rc.left);
  UINT height = static_cast<UINT>(rc.bottom - rc.top);
  if (width == 0 || height == 0) { width = 1280; height = 720; }

  DXGI_SWAP_CHAIN_DESC scd{};
  scd.BufferCount = 2;
  scd.BufferDesc.Width = width;
  scd.BufferDesc.Height = height;
  scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  scd.BufferDesc.RefreshRate.Numerator = 60;
  scd.BufferDesc.RefreshRate.Denominator = 1;
  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.OutputWindow = hwnd;
  scd.SampleDesc.Count = 1;
  scd.Windowed = TRUE;
  scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT create_flags = 0;
#ifdef _DEBUG
  create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  D3D_FEATURE_LEVEL feature_level{};
  HRESULT hr = D3D11CreateDeviceAndSwapChain(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_flags, nullptr, 0,
      D3D11_SDK_VERSION, &scd, g_d3d.swapchain.GetAddressOf(),
      g_d3d.device.GetAddressOf(), &feature_level, g_d3d.context.GetAddressOf());
  if (FAILED(hr)) return false;

  ComPtr<ID3D11Texture2D> backbuffer;
  hr = g_d3d.swapchain->GetBuffer(0, IID_PPV_ARGS(backbuffer.GetAddressOf()));
  if (FAILED(hr)) return false;
  hr = g_d3d.device->CreateRenderTargetView(backbuffer.Get(), nullptr,
                                            g_d3d.backbuffer_rtv.GetAddressOf());
  if (FAILED(hr)) return false;

  g_d3d.hwnd = hwnd;
  g_d3d.initialized = true;
  return true;
}

// --- Synthetic guest-visible handle table ---------------------------------
//
// The guest only ever stores and compares these values (SetVertexDeclaration
// etc. just do a raw pointer-sized store, per the ground-truth recomp
// bodies) -- it never dereferences them itself for the resource types
// mapped here. So a synthetic uint32_t handle works, as long as it can't
// collide with a real guest virtual address. Guest addresses observed for
// this title's image/heap all sit in the 0x82xxxxxx-0x83xxxxxx range, so
// starting the handle range at 0xF0000000 should be safe -- TODO: confirm
// against your actual title's full memory map rather than trusting this
// one sample of observed addresses.
template <typename T>
class HandleTable {
 public:
  uint32_t Allocate(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t handle = next_++;
    table_[handle] = std::move(value);
    return handle;
  }
  T* Get(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = table_.find(handle);
    return it != table_.end() ? &it->second : nullptr;
  }
  void Free(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex_);
    table_.erase(handle);
  }

 private:
  std::mutex mutex_;
  uint32_t next_ = 0xF0000000u;
  std::unordered_map<uint32_t, T> table_;
};

// --- Vertex declaration parsing --------------------------------------------
//
// Ground truth from rex_D3DDevice_CreateVertexDeclaration_82D9E2B0 +
// rex_XGSetVertexDeclaration_82D9E1C0 (sk83_na_m_recomp.77.cpp):
//   - CreateVertexDeclaration takes NO implicit CDevice* this -- r3 is the
//     element array pointer directly (declarations aren't device-specific
//     in real D3D9 either, so this tracks). Returns the allocated
//     declaration object in r3.
//   - Each element is 12 bytes (confirmed by the counting-loop stride),
//     NOT the textbook 8-byte D3DVERTEXELEMENT9. Terminator is Stream ==
//     0xFFFF at offset +0 of an element (D3DDECL_END()'s Stream=0xFF,
//     read as u16).
//   - XGSetVertexDeclaration never decodes the other fields itself -- it
//     raw-copies each 12-byte element verbatim after building a header.
//     That means the Offset/Type/Method/Usage/UsageIndex layout below is
//     an ASSUMPTION (standard D3D9 field meanings, padded from 8 to 12
//     bytes), not something this function confirms. Verify against
//     whatever populates these arrays before trusting it against a real
//     draw with unexpected vertex corruption.
#pragma pack(push, 1)
struct D3DVERTEXELEMENT9_GUEST {
  uint16_t stream;       // +0, 0xFFFF terminator -- CONFIRMED
  uint16_t offset;       // +2 -- assumed, unconfirmed
  uint8_t type;          // +4 -- assumed, unconfirmed
  uint8_t method;        // +5 -- assumed, unconfirmed
  uint8_t usage;         // +6 -- assumed, unconfirmed
  uint8_t usage_index;   // +7 -- assumed, unconfirmed
  uint8_t _pad[4];       // +8..+11 -- confirmed present (12-byte stride), meaning unknown
};
static_assert(sizeof(D3DVERTEXELEMENT9_GUEST) == 12, "must match confirmed 12-byte stride");
#pragma pack(pop)
constexpr uint16_t kD3DDeclStreamEnd = 0xFFFF;

struct VertexDeclarationInfo {
  std::vector<D3DVERTEXELEMENT9_GUEST> elements;
};

HandleTable<VertexDeclarationInfo> g_vertex_declarations;

struct ShaderHandleInfo {
  // Placeholder until the microcode-translation step is wired up -- see
  // the D3DVertexShader_Bind / D3DIndexBuffer_Lock investigation notes.
  uint32_t guest_microcode_addr = 0;
};
HandleTable<ShaderHandleInfo> g_vertex_shaders;
HandleTable<ShaderHandleInfo> g_pixel_shaders;

// --- Vertex/index buffer staging --------------------------------------------
//
// Unlike declarations/shaders, D3DVertexBuffer/D3DIndexBuffer guest objects
// already have a real, unique guest address (the `this` the game holds) --
// no synthetic handle needed, just key off that address directly. Backed
// by a growable host byte buffer; the real ID3D11Buffer is built lazily at
// draw time once stride/format are known (same "defer until we know
// enough" reasoning as the vertex declaration -> InputLayout step).
struct GpuBufferInfo {
  std::vector<uint8_t> data;
  ComPtr<ID3D11Buffer> host_buffer;  // built/rebuilt lazily at draw time
  bool dirty = true;
};
std::mutex g_buffers_mutex;
std::unordered_map<uint32_t, GpuBufferInfo> g_vertex_buffers;  // keyed by guest D3DVertexBuffer* address
std::unordered_map<uint32_t, GpuBufferInfo> g_index_buffers;   // keyed by guest D3DIndexBuffer* address

// Scratch state for an in-progress Lock -- Unlock just needs to know which
// buffer/map a given `this` corresponds to, which we already have from
// Lock, so this only exists to remember what Lock handed back in case
// Unlock needs to reconcile buffer growth. Keyed by guest object address.
struct PendingLock {
  uint32_t offset;
  uint32_t size;  // 0 interpreted as real D3D9 does: "lock to end of buffer"
};
std::mutex g_pending_locks_mutex;
std::unordered_map<uint32_t, PendingLock> g_pending_vb_locks;
std::unordered_map<uint32_t, PendingLock> g_pending_ib_locks;

// Index format: confirmed via Initialize_IndexBuffer_renderengine (writer)
// AND GetParameters_IndexBuffer_renderengine (reader) independently
// agreeing on the same bit -- IndexBuffer+0 bit 31 set means 32-bit
// indices, clear means 16-bit. Cross-confirmed from two functions, not a
// single-source guess.
bool IsIndexBuffer32Bit(uint32_t guest_ib_addr) {
  if (auto* mem = rex::Runtime::instance()->memory()) {
    if (auto* p = mem->TranslateVirtual<uint32_t*>(guest_ib_addr + 0)) {
      return (__builtin_bswap32(*p) & 0x80000000u) != 0;
    }
  }
  return false;  // default to 16-bit if we can't read it
}
//
// CreateTexture isn't wired yet (param order still needs confirming), so
// this map is here but nothing populates it until that lands. SetTexture
// below just records whatever guest handle comes through.
struct TextureInfo {
  ComPtr<ID3D11Texture2D> texture;
  ComPtr<ID3D11ShaderResourceView> srv;
  ComPtr<ID3D11RenderTargetView> rtv;  // lazily created if used as a render target
  ComPtr<ID3D11DepthStencilView> dsv;  // lazily created if used as a depth-stencil
};
HandleTable<TextureInfo> g_textures;

// --- Per-device pipeline state cache ---------------------------------------
//
// Mirrors what CDevice would have accumulated before a KickOff, but we
// never let it reach CDevice at all -- these hooks capture it directly.
struct BoundStream {
  uint32_t guest_vb_addr = 0;
  uint32_t offset_bytes = 0;
  uint32_t stride_bytes = 0;
};

// D3DRENDERSTATETYPE is a standard, public D3D9 enum (not something
// requiring RE) -- but this build also has Xbox-360-specific extensions
// (AlphaToMaskEnable, Buffer2Frames, etc.) with no PC equivalent, which
// are NOT included below. Sized to comfortably cover the standard range.
constexpr size_t kMaxRenderStates = 256;

struct PipelineStateCache {
  uint32_t vertex_declaration_handle = 0;
  uint32_t vertex_shader_handle = 0;
  uint32_t pixel_shader_handle = 0;
  BoundStream streams[16];             // D3D9 caps at 16 stream slots
  uint32_t guest_ib_addr = 0;
  uint32_t bound_textures[32] = {};    // sampler slot -> our texture handle, 0 = none
  uint32_t render_states[kMaxRenderStates] = {};
  // Best-effort assumption (see chat notes): render targets/depth-stencil
  // are passed as the same handles CreateTexture returns -- no separate
  // "surface" creation path exists in this build. Degrades to backbuffer
  // if the lookup misses, so wrong-but-harmless if the assumption is off.
  uint32_t render_target_handles[4] = {};
  uint32_t depth_stencil_handle = 0;
};
std::mutex g_state_mutex;
PipelineStateCache g_state;

// Budget-limited entry logging -- same "log the first N hits" pattern
// skate3_fullscreen_toggle.cpp already uses for its own WndProc hook.
// Each hook gets its own counter so a chatty one (Draw*, easily 1000s of
// calls/frame) doesn't drown out a rare one (CreateTexture).
template <typename Tag>
struct LogBudget {
  static std::atomic<int> remaining;
};
template <typename Tag>
std::atomic<int> LogBudget<Tag>::remaining{20};

#define SKATE3_LOG_FIRST_N(tag, ...)                                              \
  do {                                                                            \
    struct tag##_Tag {};                                                         \
    int left = LogBudget<tag##_Tag>::remaining.fetch_sub(1, std::memory_order_relaxed); \
    if (left > 0) {                                                              \
      char buf[192];                                                             \
      std::snprintf(buf, sizeof(buf), __VA_ARGS__);                              \
      OutputDebugStringA(buf);                                                   \
    }                                                                            \
  } while (0)

// --- Hooked guest functions -------------------------------------------------
//
// Ground truth for all four confirmed from sk83_na_m_recomp.77.cpp: each is
// (CDevice* this /*r3*/, <single pointer arg> /*r4*/). These are installed
// via FunctionDispatcher::SetFunction in Skate3RegisterPcRendererHooks
// below, NOT via REX_HOOK_RAW -- see skate3_pc_renderer.h for why (these
// are genuine game-specific recompiled bodies already compiled into
// sk83_na_m_recomp.77.cpp under these exact names, so redefining the same
// C++ symbol would be a link error; SetFunction overwrites the function
// *table entry* for the guest address instead, no symbol collision).
// Ordinary free functions, any name works -- kept close to the guest name
// for readability. Staying inside this anonymous namespace since nothing
// outside this file needs to call them directly (they're referenced by
// pointer, from Skate3RegisterPcRendererHooks, in the same translation unit).

void D3DDevice_CreateVertexDeclaration_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  // Confirmed: NO implicit this. r3 = pDeclElements (12-byte stride, see
  // struct comment above). Returns our synthetic handle in r3, matching
  // where the guest's own allocated-object pointer would have gone.
  const uint32_t guest_array_addr = ctx.r3.u32;
  VertexDeclarationInfo info;
  if (guest_array_addr) {
    if (auto* mem = rex::Runtime::instance()->memory()) {
      if (auto* elem = mem->TranslateVirtual<D3DVERTEXELEMENT9_GUEST*>(guest_array_addr)) {
        for (int i = 0; i < 64; ++i) {  // 64 = sane upper bound, not a confirmed limit
          uint16_t stream_be = elem[i].stream;
          uint16_t stream = static_cast<uint16_t>((stream_be >> 8) | (stream_be << 8));
          if (stream == kD3DDeclStreamEnd) break;
          D3DVERTEXELEMENT9_GUEST e = elem[i];
          e.stream = stream;
          info.elements.push_back(e);

          // DIAGNOSTIC: dumps the raw 12 bytes of every declaration
          // element the game creates. Run once against a real build and
          // compare against the actual patched VertexFetchInstruction the
          // game produces for the same declaration+shader pair -- that's
          // the empirical check that closes out the Offset/Format
          // question static analysis couldn't (see chat notes: the real
          // decode lives behind a vtable-indirect lookup table whose
          // *contents* are raw .rdata, not present in any recompiled
          // source file). Usage/UsageIndex are already solid; this is
          // purely to nail down the remaining bytes. Remove once resolved.
          const uint8_t* raw = reinterpret_cast<const uint8_t*>(&elem[i]);
          char buf[192];
          std::snprintf(buf, sizeof(buf),
              "[vdecl-diag] elem[%d] stream=%u raw=%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
              i, stream, raw[0], raw[1], raw[2], raw[3], raw[4], raw[5],
              raw[6], raw[7], raw[8], raw[9], raw[10], raw[11]);
          OutputDebugStringA(buf);
        }
      }
    }
  }
  ctx.r3.u64 = g_vertex_declarations.Allocate(std::move(info));
}

void D3DDevice_SetVertexDeclaration_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  SKATE3_LOG_FIRST_N(SetVertexDeclaration, "[pc-renderer] SetVertexDeclaration called, handle=0x%08X\n", ctx.r4.u32);
  // this = ctx.r3 (unused -- we don't touch guest CDevice memory at all),
  // pDeclaration = ctx.r4 (our synthetic handle, or 0 to clear).
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.vertex_declaration_handle = ctx.r4.u32;
}

void D3DDevice_SetVertexShader_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  SKATE3_LOG_FIRST_N(SetVertexShader, "[pc-renderer] SetVertexShader called, addr=0x%08X\n", ctx.r4.u32);
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.vertex_shader_handle = ctx.r4.u32;
}

void D3DDevice_SetPixelShader_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  SKATE3_LOG_FIRST_N(SetPixelShader, "[pc-renderer] SetPixelShader called, addr=0x%08X\n", ctx.r4.u32);
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.pixel_shader_handle = ctx.r4.u32;
}

// Ground truth confirmed earlier: (this=r3, StreamNumber=r4, pStreamData=r5,
// OffsetInBytes=r6, StrideInBytes=r7) -- the old-XEX decompile had dropped
// pStreamData entirely (the HIDWORD-artifact bug), ground truth fixed it.
void D3DDevice_SetStreamSource_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  SKATE3_LOG_FIRST_N(SetStreamSource, "[pc-renderer] SetStreamSource called, stream=%u vb=0x%08X\n",
                     ctx.r4.u32, ctx.r5.u32);
  const uint32_t stream_number = ctx.r4.u32;
  if (stream_number >= 16) return;  // D3D9 cap; guard against garbage
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.streams[stream_number].guest_vb_addr = ctx.r5.u32;
  g_state.streams[stream_number].offset_bytes = ctx.r6.u32;
  g_state.streams[stream_number].stride_bytes = ctx.r7.u32;
}

// Ground truth confirmed earlier: (this=r3, pIndexData=r4) -- this one was
// already close to right in the old-XEX decompile.
void D3DDevice_SetIndices_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  SKATE3_LOG_FIRST_N(SetIndices, "[pc-renderer] SetIndices called, ib=0x%08X\n", ctx.r4.u32);
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.guest_ib_addr = ctx.r4.u32;
}

// Ground truth confirmed earlier: (this=r3, Sampler=r4, pTexture=r5,
// PendingMask3=r6). pTexture here is whatever CreateTexture eventually
// hands back -- since that hook isn't wired yet, this records real guest
// texture addresses that don't yet resolve to anything in g_textures.
// Harmless until CreateTexture lands: draw-time texture binding will just
// no-op for handles it can't find.
void D3DDevice_SetTexture_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  SKATE3_LOG_FIRST_N(SetTexture, "[pc-renderer] SetTexture called, sampler=%u tex=0x%08X\n",
                     ctx.r4.u32, ctx.r5.u32);
  const uint32_t sampler = ctx.r4.u32;
  if (sampler >= 32) return;
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.bound_textures[sampler] = ctx.r5.u32;
}

// Ground truth confirmed: SetRenderTarget(this=r3, RenderTargetIndex=r4,
// pRenderTarget=r5), SetDepthStencilSurface(this=r3, pSurface=r4).
void D3DDevice_SetRenderTarget_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  const uint32_t index = ctx.r4.u32;
  if (index >= 4) return;
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.render_target_handles[index] = ctx.r5.u32;
}

void D3DDevice_SetDepthStencilSurface_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.depth_stencil_handle = ctx.r4.u32;
}

// Standard, public D3DRENDERSTATETYPE values (Microsoft d3d9types.h -- not
// game-specific RE, safe to state directly). Only states we've actually
// wired below are listed; this build also has Xbox-360-only extensions
// with no PC equivalent (AlphaToMaskEnable, Buffer2Frames, etc.) that
// aren't included here at all.
enum D3DRS {
  D3DRS_ZENABLE = 7,
  D3DRS_FILLMODE = 8,
  D3DRS_ZWRITEENABLE = 14,
  D3DRS_ALPHATESTENABLE = 15,
  D3DRS_CULLMODE = 22,
  D3DRS_ZFUNC = 23,
  D3DRS_ALPHAREF = 24,
  D3DRS_ALPHAFUNC = 25,
  D3DRS_ALPHABLENDENABLE = 27,
  D3DRS_COLORWRITEENABLE = 168,
  D3DRS_SCISSORTESTENABLE = 174,
};

// Every SetRenderState_X function shares the same confirmed shape --
// (this=r3, Value=r4) -- so one template stands in for what would
// otherwise be 82 near-identical hand-written functions. We record the
// raw requested value only; we do NOT replicate this build's own derived/
// computed bookkeeping (e.g. ZEnable also folds in whether a depth buffer
// is currently bound) -- we apply real D3D11 semantics ourselves from our
// own tracked state instead, which is more correct than mirroring their
// internal shortcut.
template <int kState>
void D3DDevice_SetRenderState_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  std::lock_guard<std::mutex> lock(g_state_mutex);
  g_state.render_states[kState] = ctx.r4.u32;
}

// --- Vertex/index buffer Lock/Unlock -----------------------------------
//
// Hooked at the D3DVertexBuffer_Lock/D3DIndexBuffer_Lock wrapper level, one
// step above the shared LockResource_D3D primitive -- we never reach that
// primitive at all, so its atomic-reservation/refcounting internals are
// irrelevant here. Real D3D9 Lock signature:
//   Lock(this=r3, OffsetToLock=r4, SizeToLock=r5, ppbData=r6, Flags=r7)
// This retail build's own implementation clobbers OffsetToLock/SizeToLock
// with constants before calling through (always locks the whole buffer) --
// we don't replicate that quirk; we honor the caller's real values, which
// is strictly more correct.
//
// ppbData (r6) is a GUEST address: real D3D9 Lock writes a pointer *into
// that guest memory slot* for the caller to read back and use directly.
// We give back a pointer into our own host-side staging vector -- safe
// because the guest is running as recompiled host code in the same
// process, so a plain host pointer is dereferenceable by it either way.

void* LockBufferCommon(std::unordered_map<uint32_t, GpuBufferInfo>& table,
                       std::mutex& table_mutex, uint32_t guest_obj_addr,
                       uint32_t offset, uint32_t size) {
  std::lock_guard<std::mutex> lock(table_mutex);
  auto& info = table[guest_obj_addr];  // default-constructs on first use
  const uint32_t required = offset + size;
  if (required > info.data.size()) {
    info.data.resize(required);
    info.dirty = true;
  }
  return info.data.data() + offset;
}

void D3DVertexBuffer_Lock_impl(PPCContext& ctx, uint8_t* base) {
  const uint32_t this_addr = ctx.r3.u32;
  const uint32_t offset = ctx.r4.u32;
  uint32_t size = ctx.r5.u32;
  const uint32_t ppb_data_guest = ctx.r6.u32;
  if (size == 0) {
    // Real D3D9 "lock to end of buffer" convention. Ground truth from
    // Initialize_VertexBuffer_renderengine confirms D3DVertexBuffer+32
    // holds the real total size in bytes (VertexCount * Stride, computed
    // once at init time and stored directly on this same object) -- read
    // it instead of guessing.
    uint32_t total_size = 0;
    if (auto* mem = rex::Runtime::instance()->memory()) {
      if (auto* p = mem->TranslateVirtual<uint32_t*>(this_addr + 32)) {
        total_size = __builtin_bswap32(*p);  // guest is big-endian
      }
    }
    size = total_size > offset ? total_size - offset : 0;
  }
  void* host_ptr = LockBufferCommon(g_vertex_buffers, g_buffers_mutex, this_addr, offset, size);
  if (auto* mem = rex::Runtime::instance()->memory()) {
    if (uint32_t* ppb = mem->TranslateVirtual<uint32_t*>(ppb_data_guest)) {
      // Guest reads this back as a pointer it dereferences directly --
      // safe to store a real host pointer here since guest code runs as
      // recompiled host code in this same process.
      *reinterpret_cast<void**>(ppb) = host_ptr;
    }
  }
  {
    std::lock_guard<std::mutex> lock(g_pending_locks_mutex);
    g_pending_vb_locks[this_addr] = PendingLock{offset, size};
  }
  ctx.r3.u64 = 0;  // D3D_OK
}

void D3DVertexBuffer_Unlock_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  const uint32_t this_addr = ctx.r3.u32;
  std::lock_guard<std::mutex> lock(g_buffers_mutex);
  auto it = g_vertex_buffers.find(this_addr);
  if (it != g_vertex_buffers.end()) it->second.dirty = true;  // rebuild ID3D11Buffer at next draw
  std::lock_guard<std::mutex> lock2(g_pending_locks_mutex);
  g_pending_vb_locks.erase(this_addr);
}

void D3DIndexBuffer_Lock_impl(PPCContext& ctx, uint8_t* base) {
  const uint32_t this_addr = ctx.r3.u32;
  const uint32_t offset = ctx.r4.u32;
  uint32_t size = ctx.r5.u32;
  const uint32_t ppb_data_guest = ctx.r6.u32;
  if (size == 0) {
    // Ground truth from Initialize_IndexBuffer_renderengine: unlike
    // VertexBuffer, the aligned total byte size lives at +28 here, NOT
    // +32 (+32 holds the raw index COUNT instead) -- different layout,
    // confirmed by tracing both init functions rather than assumed.
    uint32_t total_size = 0;
    if (auto* mem = rex::Runtime::instance()->memory()) {
      if (auto* p = mem->TranslateVirtual<uint32_t*>(this_addr + 28)) {
        total_size = __builtin_bswap32(*p);
      }
    }
    size = total_size > offset ? total_size - offset : 0;
  }
  void* host_ptr = LockBufferCommon(g_index_buffers, g_buffers_mutex, this_addr, offset, size);
  // Index format (16 vs 32-bit) is resolved -- see IsIndexBuffer32Bit above,
  // cross-confirmed from both Initialize_IndexBuffer_renderengine (writer)
  // and GetParameters_IndexBuffer_renderengine (reader). Not needed here in
  // Lock/Unlock (byte-copy is format-agnostic either way), but the Draw
  // hook will call IsIndexBuffer32Bit(guest_ib_addr) when building the
  // index buffer view.
  if (auto* mem = rex::Runtime::instance()->memory()) {
    if (uint32_t* ppb = mem->TranslateVirtual<uint32_t*>(ppb_data_guest)) {
      *reinterpret_cast<void**>(ppb) = host_ptr;
    }
  }
  {
    std::lock_guard<std::mutex> lock(g_pending_locks_mutex);
    g_pending_ib_locks[this_addr] = PendingLock{offset, size};
  }
  ctx.r3.u64 = 0;  // D3D_OK
}

void D3DIndexBuffer_Unlock_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  const uint32_t this_addr = ctx.r3.u32;
  std::lock_guard<std::mutex> lock(g_buffers_mutex);
  auto it = g_index_buffers.find(this_addr);
  if (it != g_index_buffers.end()) it->second.dirty = true;
  std::lock_guard<std::mutex> lock2(g_pending_locks_mutex);
  g_pending_ib_locks.erase(this_addr);
}

// --- CreateTexture --------------------------------------------------------
//
// Ground truth from sk83_na_m_recomp.77.cpp: CreateTexture DOES carry an
// implicit this (CDevice*), unlike CreateVertexDeclaration -- confirmed by
// register-count reconciliation (7 registers saved at entry: r3,r4,r5,r6,
// r7,r8,r10, matching this+6 real params+out-pointer, with r9/Pool visibly
// unused/never saved to a temp). Medium-high confidence -- an inference
// from consistent register accounting, not a cross-confirmed structural
// fact like Stream/Usage/UsageIndex were.
//   this=r3 (unused by us), Width=r4, Height=r5, Levels=r6, Usage=r7,
//   Format=r8, Pool=r9 (dead), ppTexture_out=r10
//
// Format decode is a genuine open item (see chat notes: raw Xenos
// TextureFormat values, per graphics/xenos.h, don't match PC D3DFORMAT
// numerically -- k_8_8_8_8=6 vs PC's D3DFMT_A8R8G8B8=21 -- and we don't
// know which enum this build's Format argument actually is). Logging it
// diagnostically rather than guessing; defaulting to a common format as an
// EXPLICIT placeholder, not a confirmed decode.
void D3DDevice_CreateTexture_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  const uint32_t width = ctx.r4.u32;
  const uint32_t height = ctx.r5.u32;
  const uint32_t levels = ctx.r6.u32;
  const uint32_t usage = ctx.r7.u32;
  const uint32_t format_raw = ctx.r8.u32;
  const uint32_t ppb_out_guest = ctx.r10.u32;

  char buf[160];
  std::snprintf(buf, sizeof(buf),
      "[texture-diag] CreateTexture w=%u h=%u levels=%u usage=%u format_raw=0x%08X\n",
      width, height, levels, usage, format_raw);
  OutputDebugStringA(buf);

  TextureInfo info;
  if (EnsureD3D11Initialized()) {
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = width ? width : 1;
    desc.Height = height ? height : 1;
    desc.MipLevels = levels;  // 0 = full chain, matches D3D9 CreateTexture convention
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // PLACEHOLDER -- see comment above
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    // D3DUSAGE_RENDERTARGET=0x1, D3DUSAGE_DEPTHSTENCIL=0x2 -- standard,
    // public D3D9 constants. D3D11 requires bind flags declared upfront
    // at creation time (unlike D3D9's more flexible model), so a texture
    // that will later be handed to SetRenderTarget/SetDepthStencilSurface
    // needs the matching bind flag now or CreateRenderTargetView/
    // CreateDepthStencilView will fail later. Real DXGI_FORMAT for depth
    // buffers also typically needs a _TYPELESS variant paired with a
    // separate DSV-compatible format -- not handled yet, flagged below.
    if (usage & 0x1) desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
    if (usage & 0x2) {
      desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
      desc.Format = DXGI_FORMAT_R24G8_TYPELESS;  // TODO: verify once real format is known
    }
    ComPtr<ID3D11Texture2D> tex;
    if (SUCCEEDED(g_d3d.device->CreateTexture2D(&desc, nullptr, tex.GetAddressOf()))) {
      if (!(usage & 0x2)) {  // depth textures don't get a default SRV in this simple path
        g_d3d.device->CreateShaderResourceView(tex.Get(), nullptr, info.srv.GetAddressOf());
      }
      info.texture = tex;
    }
  }
  const uint32_t handle = g_textures.Allocate(std::move(info));
  if (auto* mem = rex::Runtime::instance()->memory()) {
    if (auto* p = mem->TranslateVirtual<uint32_t*>(ppb_out_guest)) {
      *p = __builtin_bswap32(handle);  // guest reads this back as a big-endian handle value
    }
  }
  ctx.r3.u64 = 0;  // D3D_OK
}

// --- Draw path --------------------------------------------------------
//
// Lazily builds/refreshes a real ID3D11Buffer from a growable host staging
// buffer, only when the Lock/Unlock hooks have marked it dirty.
ID3D11Buffer* EnsureHostBuffer(GpuBufferInfo& info, UINT bind_flags) {
  if (!info.dirty && info.host_buffer) return info.host_buffer.Get();
  if (info.data.empty()) return nullptr;
  D3D11_BUFFER_DESC desc{};
  desc.ByteWidth = static_cast<UINT>(info.data.size());
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = bind_flags;
  D3D11_SUBRESOURCE_DATA init{};
  init.pSysMem = info.data.data();
  info.host_buffer.Reset();
  g_d3d.device->CreateBuffer(&desc, &init, info.host_buffer.GetAddressOf());
  info.dirty = false;
  return info.host_buffer.Get();
}

// D3DPRIMITIVETYPE is a standard, public D3D9 enum -- safe to map directly,
// no RE needed. D3DPT_TRIANGLEFAN (6) has no D3D11 equivalent (removed
// from the modern API) and would need geometry-shader or CPU expansion if
// this game actually uses it; flagged rather than silently mismapped.
D3D11_PRIMITIVE_TOPOLOGY MapPrimitiveType(uint32_t d3d9_type) {
  switch (d3d9_type) {
    case 1: return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;         // D3DPT_POINTLIST
    case 2: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;          // D3DPT_LINELIST
    case 3: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;         // D3DPT_LINESTRIP
    case 4: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;      // D3DPT_TRIANGLELIST
    case 5: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;     // D3DPT_TRIANGLESTRIP
    case 6:                                                     // D3DPT_TRIANGLEFAN -- no D3D11 equivalent
      OutputDebugStringA("[draw-diag] D3DPT_TRIANGLEFAN requested, not supported by D3D11 -- needs expansion\n");
      return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;  // wrong but safe fallback, not a crash
    default: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  }
}

// Cached ID3D11InputLayout objects, keyed by declaration handle.
std::unordered_map<uint32_t, ComPtr<ID3D11InputLayout>> g_input_layouts;

void DrawCommon(PPCContext& ctx, uint32_t primitive_type, uint32_t start_or_base,
                uint32_t count, bool indexed) {
  (void)start_or_base;  // primitive_type now used below via MapPrimitiveType
  SKATE3_LOG_FIRST_N(DrawCommon, "[pc-renderer] Draw%s called, primtype=%u count=%u\n",
                     indexed ? "Indexed" : "", primitive_type, count);
  if (!EnsureD3D11Initialized()) {
    SKATE3_LOG_FIRST_N(DrawD3DNotInit, "[pc-renderer] Draw skipped: EnsureD3D11Initialized failed\n");
    return;
  }

  PipelineStateCache state;
  {
    std::lock_guard<std::mutex> lock(g_state_mutex);
    state = g_state;
  }

  auto* decl = g_vertex_declarations.Get(state.vertex_declaration_handle);
  if (!decl) {
    SKATE3_LOG_FIRST_N(DrawNoDecl, "[pc-renderer] Draw skipped: no vertex declaration bound (handle=0x%08X)\n",
                       state.vertex_declaration_handle);
    return;
  }

  // Real translation pipeline, per skate3_shader_translation.h. Both shader
  // addresses must resolve or we skip the draw -- no partial rendering.
  std::vector<VertexElementUsage> decl_usages;
  for (const auto& e : decl->elements) decl_usages.push_back({e.usage, e.usage_index});

  auto* translated_vs = GetOrTranslateVertexShader(
      g_d3d.device.Get(), state.vertex_shader_handle, state.pixel_shader_handle, decl_usages);
  if (!translated_vs || !translated_vs->shader || translated_vs->bytecode.empty()) {
    SKATE3_LOG_FIRST_N(DrawNoVS, "[pc-renderer] Draw skipped: vertex shader translation failed (vs=0x%08X ps=0x%08X)\n",
                       state.vertex_shader_handle, state.pixel_shader_handle);
    return;
  }

  auto ps = GetOrTranslatePixelShader(g_d3d.device.Get(), state.pixel_shader_handle);
  if (!ps) {
    SKATE3_LOG_FIRST_N(DrawNoPS, "[pc-renderer] Draw skipped: pixel shader translation failed (ps=0x%08X)\n",
                       state.pixel_shader_handle);
    return;
  }

  ID3D11InputLayout* layout = nullptr;
  auto layout_it = g_input_layouts.find(state.vertex_declaration_handle);
  if (layout_it != g_input_layouts.end()) {
    layout = layout_it->second.Get();
  } else {
    ComPtr<ID3D11InputLayout> new_layout;
    if (SUCCEEDED(g_d3d.device->CreateInputLayout(
            translated_vs->input_elements.data(),
            static_cast<UINT>(translated_vs->input_elements.size()),
            translated_vs->bytecode.data(), translated_vs->bytecode.size(),
            new_layout.GetAddressOf()))) {
      layout = new_layout.Get();
      g_input_layouts[state.vertex_declaration_handle] = std::move(new_layout);
    }
  }
  if (!layout) {
    SKATE3_LOG_FIRST_N(DrawNoLayout, "[pc-renderer] Draw skipped: CreateInputLayout failed\n");
    return;
  }
  SKATE3_LOG_FIRST_N(DrawReachedGpu, "[pc-renderer] Draw reached actual GPU submission\n");
  g_d3d.context->IASetInputLayout(layout);
  g_d3d.context->IASetPrimitiveTopology(MapPrimitiveType(primitive_type));
  g_d3d.context->VSSetShader(translated_vs->shader.Get(), nullptr, 0);
  g_d3d.context->PSSetShader(ps.Get(), nullptr, 0);

  // Vertex buffer binding -- real ground truth for size/growth already
  // wired via Lock/Unlock; this just materializes the current data.
  ID3D11Buffer* vb = nullptr;
  UINT vb_stride = 0, vb_offset = 0;
  {
    std::lock_guard<std::mutex> lock(g_buffers_mutex);
    auto it = g_vertex_buffers.find(state.streams[0].guest_vb_addr);
    if (it != g_vertex_buffers.end()) {
      vb = EnsureHostBuffer(it->second, D3D11_BIND_VERTEX_BUFFER);
      vb_stride = state.streams[0].stride_bytes;
      vb_offset = state.streams[0].offset_bytes;
    }
  }
  if (!vb) return;
  g_d3d.context->IASetVertexBuffers(0, 1, &vb, &vb_stride, &vb_offset);

  // Bind tracked render target, falling back to the backbuffer if slot 0
  // was never set or doesn't resolve to a known texture. Confidence on
  // "render targets are the same handles CreateTexture returns" upgraded
  // after checking GetRenderTarget's ground truth: it calls the generic
  // D3DResource_AddRef on whatever's stored, not a surface-specific
  // function, consistent with no distinct surface type existing here.
  ID3D11RenderTargetView* rtv = g_d3d.backbuffer_rtv.Get();
  if (state.render_target_handles[0]) {
    if (auto* tex = g_textures.Get(state.render_target_handles[0])) {
      if (!tex->rtv && tex->texture) {
        g_d3d.device->CreateRenderTargetView(tex->texture.Get(), nullptr, tex->rtv.GetAddressOf());
      }
      if (tex->rtv) rtv = tex->rtv.Get();
    }
  }
  ID3D11DepthStencilView* dsv = nullptr;
  if (state.depth_stencil_handle) {
    if (auto* tex = g_textures.Get(state.depth_stencil_handle)) {
      if (!tex->dsv && tex->texture) {
        // TODO: DSV format needs to match the depth texture's actual
        // format properly (D24_UNORM_S8_UINT for the R24G8_TYPELESS
        // placeholder above) -- fine for the single common case, not
        // audited against every depth format this game might request.
        D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
        dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        g_d3d.device->CreateDepthStencilView(tex->texture.Get(), &dsv_desc, tex->dsv.GetAddressOf());
      }
      if (tex->dsv) dsv = tex->dsv.Get();
    }
  }
  g_d3d.context->OMSetRenderTargets(1, &rtv, dsv);

  // Apply captured render states. Rebuilding state objects every draw
  // isn't efficient (a real version would cache by state hash, matching
  // the pattern already used for host buffers) but is correct, and
  // correctness is the priority until this has actually been run once.
  // D3DCULL/D3DCMPFUNC/D3DBLEND values below are standard, public D3D9
  // enums -- not something requiring RE.
  {
    D3D11_RASTERIZER_DESC rd{};
    rd.FillMode = (state.render_states[D3DRS_FILLMODE] == 1 /*D3DFILL_POINT*/) ? D3D11_FILL_WIREFRAME
                 : (state.render_states[D3DRS_FILLMODE] == 2 /*D3DFILL_WIREFRAME*/) ? D3D11_FILL_WIREFRAME
                 : D3D11_FILL_SOLID;
    const uint32_t cull = state.render_states[D3DRS_CULLMODE];
    rd.CullMode = (cull == 2) ? D3D11_CULL_FRONT   // D3DCULL_CW
                : (cull == 3) ? D3D11_CULL_BACK     // D3DCULL_CCW
                : D3D11_CULL_NONE;                  // D3DCULL_NONE or unset
    rd.DepthClipEnable = TRUE;
    ComPtr<ID3D11RasterizerState> rs;
    if (SUCCEEDED(g_d3d.device->CreateRasterizerState(&rd, rs.GetAddressOf()))) {
      g_d3d.context->RSSetState(rs.Get());
    }

    D3D11_DEPTH_STENCIL_DESC dsd{};
    dsd.DepthEnable = state.render_states[D3DRS_ZENABLE] ? TRUE : FALSE;
    dsd.DepthWriteMask = state.render_states[D3DRS_ZWRITEENABLE] ? D3D11_DEPTH_WRITE_MASK_ALL
                                                                 : D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;  // TODO: map D3DRS_ZFUNC properly
    ComPtr<ID3D11DepthStencilState> ds;
    if (SUCCEEDED(g_d3d.device->CreateDepthStencilState(&dsd, ds.GetAddressOf()))) {
      g_d3d.context->OMSetDepthStencilState(ds.Get(), 0);
    }

    D3D11_BLEND_DESC bd{};
    bd.RenderTarget[0].BlendEnable = state.render_states[D3DRS_ALPHABLENDENABLE] ? TRUE : FALSE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;       // TODO: map D3DRS_SRCBLEND (not yet hooked)
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;  // TODO: map D3DRS_DESTBLEND (not yet hooked)
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;  // TODO: map D3DRS_COLORWRITEENABLE
    ComPtr<ID3D11BlendState> bs;
    if (SUCCEEDED(g_d3d.device->CreateBlendState(&bd, bs.GetAddressOf()))) {
      const float blend_factor[4] = {1, 1, 1, 1};
      g_d3d.context->OMSetBlendState(bs.Get(), blend_factor, 0xFFFFFFFF);
    }
  }

  if (indexed) {
    ID3D11Buffer* ib = nullptr;
    bool ib_32bit = false;
    {
      std::lock_guard<std::mutex> lock(g_buffers_mutex);
      auto it = g_index_buffers.find(state.guest_ib_addr);
      if (it != g_index_buffers.end()) {
        ib = EnsureHostBuffer(it->second, D3D11_BIND_INDEX_BUFFER);
        ib_32bit = IsIndexBuffer32Bit(state.guest_ib_addr);  // confirmed decode
      }
    }
    if (!ib) return;
    g_d3d.context->IASetIndexBuffer(ib, ib_32bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
    g_d3d.context->DrawIndexed(count, 0, 0);
  } else {
    g_d3d.context->Draw(count, 0);
  }
}

// Ground truth confirmed earlier: DrawIndexedVertices(this=r3, PrimitiveType=r4,
// StartIndex=r5, IndexCount=r6, a4=r7). DrawVertices(this=r3, PrimitiveType=r4,
// VertexCount=r5, a3=r6).
void D3DDevice_DrawIndexedVertices_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  DrawCommon(ctx, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, /*indexed=*/true);
}

void D3DDevice_DrawVertices_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  DrawCommon(ctx, ctx.r4.u32, 0, ctx.r5.u32, /*indexed=*/false);
}

// Ground truth confirmed earlier: Resolve(this=r3, Flags=r4, pDestTexture=r5,
// DestLevel=r6, pClearColor=r7, ClearZ=f1[double], ClearStencil=r8,
// pParameters=r9ish). We only handle the common "resolve backbuffer into a
// destination texture" case; clear-color/depth paths are not wired.
void D3DDevice_Resolve_impl(PPCContext& ctx, uint8_t* base) {
  (void)base;
  if (!g_d3d.initialized) return;
  const uint32_t dest_texture_handle = ctx.r5.u32;
  auto* tex_info = g_textures.Get(dest_texture_handle);
  if (!tex_info || !tex_info->texture) return;
  ComPtr<ID3D11Resource> backbuffer_resource;
  g_d3d.backbuffer_rtv->GetResource(backbuffer_resource.GetAddressOf());
  g_d3d.context->CopyResource(tex_info->texture.Get(), backbuffer_resource.Get());
  ctx.r3.u64 = 0;  // D3D_OK
}

}  // namespace

// --- Registration ------------------------------------------------------
//
// Guest addresses pulled directly from sk83_na_m_init.cpp's function table
// (confirmed against the same build this whole investigation has been
// using -- re-verify if you're on a different sk83_na_m revision):
//   SetVertexDeclaration      0x82D9E1A8
//   SetVertexShader           0x82D9DFD8
//   SetPixelShader             0x82D9DE18
//   CreateVertexDeclaration    0x82D9E2B0
//   DrawIndexedVertices        0x82D97AF8  (next pass)
//   DrawVertices               0x82D97708  (next pass)
//   SetTexture                 0x82D99C48
//   SetStreamSource            0x82D94340
//   SetIndices                 0x82D944E8
//   D3DVertexBuffer_Lock       0x82D96BB0
//   D3DVertexBuffer_Unlock     0x82D95C70
//   D3DIndexBuffer_Lock        0x82D96C00
//   D3DIndexBuffer_Unlock      0x82D95C80
//   CreateTexture              0x82D99910
//   DrawIndexedVertices        0x82D97AF8
//   DrawVertices               0x82D97708
//   Resolve                    0x82D9C448
//   SetRenderTarget            0x82D94CC8
//   SetDepthStencilSurface     0x82D95018
//   SetRenderState_ZEnable            0x82D926A8
//   SetRenderState_ZWriteEnable       0x82D926E8
//   SetRenderState_ZFunc             0x82D92718
//   SetRenderState_CullMode          0x82D91F80
//   SetRenderState_AlphaBlendEnable  0x82D92018
//   SetRenderState_AlphaTestEnable   0x82D91FE0
//   SetRenderState_AlphaFunc         0x82D924A0
//   SetRenderState_AlphaRef          0x82D92440
//   SetRenderState_ColorWriteEnable  0x82D92C48
//   SetRenderState_ScissorTestEnable 0x82D94B88
//   SetRenderState_FillMode          0x82D91FB0
// The remaining ~72 SetRenderState_* functions (plus several Xbox-only
// extensions with no PC equivalent) aren't wired yet -- same template
// mechanism, just needs each address pulled the same way.

void Skate3RegisterPcRendererHooks(rex::runtime::FunctionDispatcher* dispatcher) {
  if (!dispatcher) {
    OutputDebugStringA("[pc-renderer] Skate3RegisterPcRendererHooks: dispatcher is null\n");
    return;
  }
  // SetFunction returns false (and logs internally, per its own doc
  // comment) if guest_address falls outside every registered module's
  // range -- i.e. if this is called before the XEX image is loaded and its
  // function table set up. Checking + logging every call here rather than
  // ignoring the return value, since where exactly this needs to be called
  // from (relative to OnPostSetup/LoadXexImage/PrepareModuleLaunch in the
  // ReXApp base class, which we don't have visibility into) is an open
  // question -- if these all log failures, that's the concrete signal this
  // is the wrong call site, not a subtler bug to chase.
  int success_count = 0;
  int total_count = 0;
  auto set = [&](uint32_t addr, ::PPCFunc* fn, const char* name) {
    ++total_count;
    if (dispatcher->SetFunction(addr, fn)) {
      ++success_count;
    } else {
      char buf[128];
      std::snprintf(buf, sizeof(buf), "[pc-renderer] SetFunction FAILED for %s @ 0x%08X\n", name, addr);
      OutputDebugStringA(buf);
    }
  };
  set(0x82D9E1A8, &D3DDevice_SetVertexDeclaration_impl, "SetVertexDeclaration");
  set(0x82D9DFD8, &D3DDevice_SetVertexShader_impl, "SetVertexShader");
  set(0x82D9DE18, &D3DDevice_SetPixelShader_impl, "SetPixelShader");
  set(0x82D9E2B0, &D3DDevice_CreateVertexDeclaration_impl, "CreateVertexDeclaration");
  set(0x82D94340, &D3DDevice_SetStreamSource_impl, "SetStreamSource");
  set(0x82D944E8, &D3DDevice_SetIndices_impl, "SetIndices");
  set(0x82D99C48, &D3DDevice_SetTexture_impl, "SetTexture");
  set(0x82D94CC8, &D3DDevice_SetRenderTarget_impl, "SetRenderTarget");
  set(0x82D95018, &D3DDevice_SetDepthStencilSurface_impl, "SetDepthStencilSurface");
  set(0x82D96BB0, &D3DVertexBuffer_Lock_impl, "D3DVertexBuffer_Lock");
  set(0x82D95C70, &D3DVertexBuffer_Unlock_impl, "D3DVertexBuffer_Unlock");
  set(0x82D96C00, &D3DIndexBuffer_Lock_impl, "D3DIndexBuffer_Lock");
  set(0x82D95C80, &D3DIndexBuffer_Unlock_impl, "D3DIndexBuffer_Unlock");
  set(0x82D99910, &D3DDevice_CreateTexture_impl, "CreateTexture");
  set(0x82D97AF8, &D3DDevice_DrawIndexedVertices_impl, "DrawIndexedVertices");
  set(0x82D97708, &D3DDevice_DrawVertices_impl, "DrawVertices");
  set(0x82D9C448, &D3DDevice_Resolve_impl, "Resolve");
  set(0x82D926A8, &D3DDevice_SetRenderState_impl<D3DRS_ZENABLE>, "SetRenderState_ZEnable");
  set(0x82D926E8, &D3DDevice_SetRenderState_impl<D3DRS_ZWRITEENABLE>, "SetRenderState_ZWriteEnable");
  set(0x82D92718, &D3DDevice_SetRenderState_impl<D3DRS_ZFUNC>, "SetRenderState_ZFunc");
  set(0x82D91F80, &D3DDevice_SetRenderState_impl<D3DRS_CULLMODE>, "SetRenderState_CullMode");
  set(0x82D92018, &D3DDevice_SetRenderState_impl<D3DRS_ALPHABLENDENABLE>, "SetRenderState_AlphaBlendEnable");
  set(0x82D91FE0, &D3DDevice_SetRenderState_impl<D3DRS_ALPHATESTENABLE>, "SetRenderState_AlphaTestEnable");
  set(0x82D924A0, &D3DDevice_SetRenderState_impl<D3DRS_ALPHAFUNC>, "SetRenderState_AlphaFunc");
  set(0x82D92440, &D3DDevice_SetRenderState_impl<D3DRS_ALPHAREF>, "SetRenderState_AlphaRef");
  set(0x82D92C48, &D3DDevice_SetRenderState_impl<D3DRS_COLORWRITEENABLE>, "SetRenderState_ColorWriteEnable");
  set(0x82D94B88, &D3DDevice_SetRenderState_impl<D3DRS_SCISSORTESTENABLE>, "SetRenderState_ScissorTestEnable");
  set(0x82D91FB0, &D3DDevice_SetRenderState_impl<D3DRS_FILLMODE>, "SetRenderState_FillMode");

  char summary[128];
  std::snprintf(summary, sizeof(summary), "[pc-renderer] Hook registration: %d/%d succeeded\n",
               success_count, total_count);
  OutputDebugStringA(summary);
}

void Skate3InitPcRenderer() {
  // Device creation is deferred to first use (EnsureD3D11Initialized) since
  // the game window doesn't exist yet at static-init time -- same reason
  // skate3_fullscreen_toggle.cpp polls for it on a background thread rather
  // than assuming it's ready here. Nothing to do at this call site yet;
  // kept as the one obvious init entry point for main.cpp to call.
}
