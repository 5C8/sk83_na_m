#pragma once

#include <cstdint>
#include <vector>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <d3d11.h>
#include <wrl/client.h>

// Translates this game's own Xenos vertex/pixel shader microcode into real
// D3D11 shaders, using the SDK's Shader/ShaderTranslator/DxbcShaderTranslator
// classes as a pure translation utility -- no CommandProcessor, no
// RegisterFile, no live GPU emulation involved anywhere in this file.
//
// KEY INSIGHT (see chat notes): we read microcode AT DRAW TIME, after the
// game's own untouched Xbox2Bind_VertexProgramState_renderengine::Bind() ->
// D3DVertexShader_Bind -> PatchVertexShaderToMatchVertexDeclaration chain
// has already run for this frame. That means the vfetch instructions in the
// microcode we read already have REAL, correct offset/format baked in by
// the game's own working code -- we never need to decode the vertex
// declaration's compact element format ourselves. The one hard requirement
// this creates: the cache below MUST key on a hash of the actual microcode
// CONTENT read at draw time, not on the guest shader object's address,
// because the same object's microcode changes as it gets re-patched for
// different (declaration, pixel shader) pairings across different draws.
struct TranslatedVertexShader {
  Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
  std::vector<uint8_t> bytecode;  // raw DXBC bytes -- CreateInputLayout just needs pointer+size,
                                  // no need for the ID3DBlob/d3dcompiler.h dependency
  std::vector<D3D11_INPUT_ELEMENT_DESC> input_elements;  // offset/format from the shader's own
                                                          // analyzed vfetch instructions
};

// Returns nullptr on failure (including "not implemented yet" -- see the
// open questions in the .cpp). Caller should skip the draw, same pattern as
// everywhere else in this project: no draw is better than a wrong one.
// Small, deliberately narrow struct shared with skate3_pc_renderer.cpp --
// avoids needing to expose that file's full VertexDeclarationInfo/
// D3DVERTEXELEMENT9_GUEST types here. Usage/UsageIndex are the two solidly-
// confirmed declaration fields (see chat notes); this is all the semantic-
// matching step needs.
struct VertexElementUsage {
  uint8_t usage;
  uint8_t usage_index;
};

TranslatedVertexShader* GetOrTranslateVertexShader(ID3D11Device* device,
                                                   uint32_t guest_vertex_shader_addr,
                                                   uint32_t guest_pixel_shader_addr,
                                                   const std::vector<VertexElementUsage>& decl_usages);

Microsoft::WRL::ComPtr<ID3D11PixelShader> GetOrTranslatePixelShader(
    ID3D11Device* device, uint32_t guest_pixel_shader_addr);
