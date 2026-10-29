#include "skate3_shader_translation.h"

#include <cstdio>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <rex/runtime.h>
#include <rex/graphics/xenos.h>
#include <rex/graphics/pipeline/shader/shader.h>
#include <rex/graphics/pipeline/shader/dxbc_translator.h>
#include <rex/string/buffer.h>
#include <rex/ui/graphics_provider.h>

#include <dxgi.h>

using Microsoft::WRL::ComPtr;
using rex::graphics::Shader;
using rex::graphics::ParsedVertexFetchInstruction;

namespace {

std::mutex g_translation_mutex;
std::unordered_map<uint64_t, TranslatedVertexShader> g_vertex_cache;   // keyed by microcode content hash
std::unordered_map<uint64_t, ComPtr<ID3D11PixelShader>> g_pixel_cache;  // keyed by microcode content hash

// FNV-1a -- fine for a content-addressed cache key, no cryptographic need.
uint64_t HashMicrocode(const uint32_t* dwords, size_t count) {
  uint64_t h = 1469598103934665603ull;
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(dwords);
  for (size_t i = 0; i < count * 4; ++i) {
    h ^= bytes[i];
    h *= 1099511628211ull;
  }
  return h;
}

// Ground truth confirmed via XGRegisterVertexShader/GetFunction (see chat
// notes): D3DVertexShader+32 = pointer to microcode, +880 = size in bytes.
bool ReadVertexMicrocode(uint32_t guest_shader_addr, std::vector<uint32_t>& out_dwords) {
  auto* mem = rex::Runtime::instance()->memory();
  if (!mem) return false;
  auto* size_ptr = mem->TranslateVirtual<uint32_t*>(guest_shader_addr + 880);
  auto* code_ptr_field = mem->TranslateVirtual<uint32_t*>(guest_shader_addr + 32);
  if (!size_ptr || !code_ptr_field) return false;
  const uint32_t size_bytes = __builtin_bswap32(*size_ptr);
  const uint32_t code_guest_addr = __builtin_bswap32(*code_ptr_field);
  if (!size_bytes || !code_guest_addr) return false;
  auto* code = mem->TranslateVirtual<uint32_t*>(code_guest_addr);
  if (!code) return false;
  out_dwords.assign(code, code + (size_bytes / 4));
  return true;
}

// Ground truth confirmed via XGRegisterPixelShader: D3DPixelShader+24 =
// pointer to microcode. NO SIZE FIELD EXISTS ON THIS OBJECT -- genuine open
// item. Xenos ALU/fetch instructions are fixed 3-dword units and the ISA is
// believed to be self-terminating (an exec instruction with an end-of-
// program flag), matching why no D3DPixelShader_GetFunction-equivalent
// exists to query a length. This reads a generous upper bound and relies on
// Shader::AnalyzeUcode to stop at the real instruction count -- UNCONFIRMED
// that AnalyzeUcode actually behaves this way from a raw pointer; needs
// checking against its implementation (not in headers) or empirical testing.
bool ReadPixelMicrocode(uint32_t guest_shader_addr, std::vector<uint32_t>& out_dwords) {
  auto* mem = rex::Runtime::instance()->memory();
  if (!mem) return false;
  auto* code_ptr_field = mem->TranslateVirtual<uint32_t*>(guest_shader_addr + 24);
  if (!code_ptr_field) return false;
  const uint32_t code_guest_addr = __builtin_bswap32(*code_ptr_field);
  if (!code_guest_addr) return false;
  auto* code = mem->TranslateVirtual<uint32_t*>(code_guest_addr);
  if (!code) return false;
  constexpr size_t kMaxDwords = 4096;  // TODO: replace with a real length once found/confirmed
  out_dwords.assign(code, code + kMaxDwords);
  return true;
}

// Real, public xenos::VertexFormat enum (graphics/xenos.h) -- direct
// mapping, not RE-derived. is_signed/is_normalized come from the fetch
// instruction's own fields (VertexFetchInstruction::Data DWORD1, see chat
// notes) and are NOT threaded through yet below -- TODO, currently assumes
// the common UNORM/FLOAT case rather than switching on those flags.
DXGI_FORMAT VertexFormatToDXGI(rex::graphics::xenos::VertexFormat fmt) {
  using VF = rex::graphics::xenos::VertexFormat;
  switch (fmt) {
    case VF::k_32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
    case VF::k_32_32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
    case VF::k_32_32_32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
    case VF::k_32_32_32_32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case VF::k_16_16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;
    case VF::k_16_16_16_16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case VF::k_8_8_8_8: return DXGI_FORMAT_R8G8B8A8_UNORM;  // TODO: UINT/SNORM/SINT variants
    case VF::k_2_10_10_10: return DXGI_FORMAT_R10G10B10A2_UNORM;
    case VF::k_16_16: return DXGI_FORMAT_R16G16_UNORM;      // TODO: SINT/SNORM variants
    case VF::k_16_16_16_16: return DXGI_FORMAT_R16G16B16A16_UNORM;
    case VF::k_32: return DXGI_FORMAT_R32_UINT;
    case VF::k_32_32: return DXGI_FORMAT_R32G32_UINT;
    case VF::k_32_32_32_32: return DXGI_FORMAT_R32G32B32A32_UINT;
    case VF::k_10_11_11:
    case VF::k_11_11_10:
      // No direct DXGI equivalent -- these are packed fixed-point formats
      // DXGI doesn't have a matching type for. Would need either a shader-
      // side unpack (sample as UINT, decode in the input assembler stage
      // via a wrapper) or CPU-side conversion at buffer-upload time.
      OutputDebugStringA("[shader-diag] unmapped VertexFormat requested (k_10_11_11/k_11_11_10)\n");
      return DXGI_FORMAT_R32_UINT;  // wrong but won't crash
    default:
      return DXGI_FORMAT_UNKNOWN;
  }
}

// Real GpuVendorID values are just standard PCI vendor IDs (0x1002=AMD,
// 0x10DE=Nvidia, 0x8086=Intel, etc. -- see ui/graphics_provider.h), so this
// is a genuine DXGI adapter query, not RE or guesswork.
rex::ui::GraphicsProvider::GpuVendorID QueryGpuVendorID(ID3D11Device* device) {
  ComPtr<IDXGIDevice> dxgi_device;
  if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(dxgi_device.GetAddressOf())))) {
    ComPtr<IDXGIAdapter> adapter;
    if (SUCCEEDED(dxgi_device->GetAdapter(adapter.GetAddressOf()))) {
      DXGI_ADAPTER_DESC desc{};
      if (SUCCEEDED(adapter->GetDesc(&desc))) {
        return static_cast<rex::ui::GraphicsProvider::GpuVendorID>(desc.VendorId);
      }
    }
  }
  return rex::ui::GraphicsProvider::GpuVendorID::kAMD;  // arbitrary fallback, shouldn't be hit
}

}  // namespace

TranslatedVertexShader* GetOrTranslateVertexShader(ID3D11Device* device,
                                                   uint32_t guest_vertex_shader_addr,
                                                   uint32_t guest_pixel_shader_addr,
                                                   const std::vector<VertexElementUsage>& decl_usages) {
  std::vector<uint32_t> vs_dwords;
  if (!ReadVertexMicrocode(guest_vertex_shader_addr, vs_dwords)) return nullptr;
  const uint64_t hash = HashMicrocode(vs_dwords.data(), vs_dwords.size());

  std::lock_guard<std::mutex> lock(g_translation_mutex);
  auto it = g_vertex_cache.find(hash);
  if (it != g_vertex_cache.end()) return &it->second;

  // ucode_source_endian defaults to std::endian::big, matching the guest's
  // actual byte order -- no manual pre-swap needed, confirmed from the
  // Shader constructor's own doc comment.
  Shader shader(rex::graphics::xenos::ShaderType::kVertex, hash, vs_dwords.data(), vs_dwords.size());

  rex::string::StringBuffer disasm;  // default-constructible confirmed (initial_capacity has a
                                     // default arg) -- safe as written.
  shader.AnalyzeUcode(disasm);
  // NOTE: Shader itself has no is_valid()/error accessor confirmed -- that
  // only exists on Shader::Translation (checked below, after translation).
  // If AnalyzeUcode failed on genuinely bad microcode, that should surface
  // as translation->is_valid() == false rather than being caught here.

  // TODO (genuine open item, not attempted below): correct Modification bits
  // require the PAIRED pixel shader's required-interpolator mask via
  // Shader::GetInterpolatorInputMask(SQ_PROGRAM_CNTL), which itself needs a
  // synthesized SQ_PROGRAM_CNTL value we haven't derived. Using modification
  // = 0 as an explicit placeholder -- this is very likely wrong for any
  // shader that doesn't happen to want the default interpolator set, not a
  // safe assumption to build on without revisiting.
  (void)guest_pixel_shader_addr;
  rex::graphics::DxbcShaderTranslator::Modification modification{};
  modification.value = 0;

  bool is_new = false;
  Shader::Translation* translation = shader.GetOrCreateTranslation(modification.value, &is_new);
  if (!translation) return nullptr;

  // GpuVendorID is now a real DXGI adapter query (see QueryGpuVendorID
  // above), not a placeholder. bindless_resources_used=false and
  // edram_rov_used=false remain correct for us -- we don't do bindless
  // resources or EDRAM/ROV blending, using normal D3D11 blend state instead.
  static rex::graphics::DxbcShaderTranslator translator(
      QueryGpuVendorID(device), /*bindless_resources_used=*/false, /*edram_rov_used=*/false);

  // TODO (genuine open item): exact ownership/threading contract of
  // TranslateAnalyzedShader isn't confirmed from the header alone (e.g.
  // whether it's safe to call concurrently, whether the same translator
  // instance can be reused across shaders -- assumed yes above via `static`,
  // but not verified).
  translator.TranslateAnalyzedShader(*translation);
  if (!translation->is_valid()) {
    OutputDebugStringA("[shader-diag] vertex shader translation failed\n");
    return nullptr;
  }

  TranslatedVertexShader result;
  const auto& bytecode = translation->translated_binary();
  if (FAILED(device->CreateVertexShader(bytecode.data(), bytecode.size(), nullptr,
                                        result.shader.GetAddressOf()))) {
    return nullptr;
  }
  result.bytecode = bytecode;  // plain copy -- CreateInputLayout needs it kept alive below

  // Real input layout, built entirely from the shader's own analyzed vfetch
  // instructions -- offset/format require no vertex-declaration decoding at
  // all, per the chat notes' key insight. Semantic name/index still need to
  // come from the declaration's confirmed Usage/UsageIndex fields (+9/+10)
  // -- combining both isn't wired up at this call site yet (needs the
  // VertexDeclarationInfo passed in alongside the two shader addresses).
  // Semantic name convention: UNCONFIRMED what DxbcShaderTranslator's
  // generated input signature actually expects here -- couldn't find it
  // documented in the headers available (searched for TEXCOORD/SemanticName;
  // the only TEXCOORD references found are for VS->PS interpolator linkage,
  // a different part of the pipeline, not vertex input attributes). Using
  // "TEXCOORD" + decl_usages index as a placeholder. If CreateInputLayout
  // fails, the D3D11 debug layer's error message will name the exact
  // expected semantic directly -- that's the fastest real fix, faster than
  // more static searching.
  UINT offset_running = 0;
  size_t attr_index = 0;
  for (const auto& binding : shader.vertex_bindings()) {
    for (const auto& attr : binding.attributes) {
      const auto& a = attr.fetch_instr.attributes;
      D3D11_INPUT_ELEMENT_DESC desc{};
      if (attr_index < decl_usages.size()) {
        desc.SemanticName = "TEXCOORD";  // TODO: confirm real convention (see comment above)
        desc.SemanticIndex = decl_usages[attr_index].usage_index;
      } else {
        desc.SemanticName = "TEXCOORD";
        desc.SemanticIndex = static_cast<UINT>(attr_index);
      }
      desc.Format = VertexFormatToDXGI(a.data_format);
      desc.AlignedByteOffset = static_cast<UINT>(a.offset);  // already in bytes? or dwords -- TODO verify
      desc.InputSlot = static_cast<UINT>(binding.binding_index);
      desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
      result.input_elements.push_back(desc);
      ++attr_index;
    }
  }

  auto [inserted_it, ok] = g_vertex_cache.emplace(hash, std::move(result));
  return &inserted_it->second;
}

ComPtr<ID3D11PixelShader> GetOrTranslatePixelShader(ID3D11Device* device,
                                                    uint32_t guest_pixel_shader_addr) {
  std::vector<uint32_t> ps_dwords;
  if (!ReadPixelMicrocode(guest_pixel_shader_addr, ps_dwords)) return nullptr;
  const uint64_t hash = HashMicrocode(ps_dwords.data(), ps_dwords.size());

  std::lock_guard<std::mutex> lock(g_translation_mutex);
  auto it = g_pixel_cache.find(hash);
  if (it != g_pixel_cache.end()) return it->second;

  Shader shader(rex::graphics::xenos::ShaderType::kPixel, hash, ps_dwords.data(), ps_dwords.size());
  rex::string::StringBuffer disasm;
  shader.AnalyzeUcode(disasm);
  // Same note as the vertex path: no Shader::is_valid() -- checked via
  // Translation below instead.

  rex::graphics::DxbcShaderTranslator::Modification modification{};
  modification.value = 0;  // same open item as the vertex path
  Shader::Translation* translation = shader.GetOrCreateTranslation(modification.value);
  if (!translation) return nullptr;

  static rex::graphics::DxbcShaderTranslator translator(QueryGpuVendorID(device), false, false);
  translator.TranslateAnalyzedShader(*translation);
  if (!translation->is_valid()) return nullptr;

  ComPtr<ID3D11PixelShader> result;
  const auto& bytecode = translation->translated_binary();
  device->CreatePixelShader(bytecode.data(), bytecode.size(), nullptr, result.GetAddressOf());
  g_pixel_cache[hash] = result;
  return result;
}
