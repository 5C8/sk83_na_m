// Photo-editor postfx chain: exact ports of the game's shaders, transcribed
// mechanically from their Xenos ucode (dumped via an F11 capture) and
// validated bit-exact against an offline ucode interpreter before HLSL
// conversion. Pass order
// mirrors the game's chain while a photo-mission photo editor is up:
//   visualfx   (full res)  scene+accum color grade / vignette / depth->CoC
//   dof_down   (576x320)   4-tap alpha(CoC)-weighted downsample
//   dof_mb     (576x320)   9-tap motion-blur+DOF pass (motion from depth)
//   dof        (576x320)   9-tap variable-radius CoC blur
//   uber       (full res)  temporal-jitter combine + grade LUT + grain
//   fisheye    (full res)  lens warp + vignette gradient + tint -> screen
// Constants come LIVE from the game's PS/VS shadow banks captured at the
// SetPending_AluConstants hook per pass (rows c0..c31 + VS rows in c240..247);
// rows c250..c255 are the shaders' LITERAL tables, loaded by the game via
// PM4 LOAD_ALU_CONSTANT from the shader asset footers and baked here by the
// app layer (values read from a live capture).
//
// Register indices are kept IDENTICAL to the ucode. Do not simplify.

cbuffer Consts : register(b0) { float4 c[256]; };
// c[240..247] = the pass's VS c0..c7 (uvOffset/uvScale/oC1 rows).
// c[248].x    = output width, .y = output height (depth pack / debug).

Texture2D    t0 : register(t0);
Texture2D    t1 : register(t1);
Texture2D    t2 : register(t2);
Texture2D    t3 : register(t3);
Texture3D    t4 : register(t4);
Texture2D    t6 : register(t6);
Texture2D    t7 : register(t7);
SamplerState s_lin  : register(s0);  // linear clamp (color/gradient fetches)
SamplerState s_pt   : register(s1);  // point clamp (packed depth)
SamplerState s_wrap : register(s2);  // linear wrap (grain)

// Xenos ALU multiply: a zero operand gives 0 even against inf/NaN (D3D9
// semantics; xenia's translator emits the same). The DOF passes normalise
// by rcp(weight sum), and a frame running the chain with DOF OFF (bails /
// replays: dof c0 = 0, dofmb c2 = 0) has a zero sum: 0 * rcp(0) is 0 on
// the console, NaN under IEEE, and the NaN rode through uber into a black
// frame.
float XMul(float a, float b) { return (a == 0.0 || b == 0.0) ? 0.0 : a * b; }

struct VsOut {
  float4 pos : SV_Position;
  float4 r0 : TEXCOORD0;
  float4 r1 : TEXCOORD1;
};

// Fullscreen triangle; per-pass interpolators mirror the game quad VSs
// (uv attr = screen uv; ucode: oC0 = (uv + c3.xy, uv), oC1 = uv.x*c0 +
// uv.y*c1 + c2; the offset/scale rows are the captured VS constants).
VsOut vs_offset(uint vid : SV_VertexID) {
  VsOut o;
  float2 t = float2((vid << 1) & 2, vid & 2);
  o.pos = float4(t * float2(2, -2) + float2(-1, 1), 0, 1);
  o.r0 = float4(t + c[243].xy, t);
  o.r1 = t.x * c[240] + t.y * c[241] + c[242];
  return o;
}

// uber: oC0 = raw uv4.
VsOut vs_raw(uint vid : SV_VertexID) {
  VsOut o;
  float2 t = float2((vid << 1) & 2, vid & 2);
  o.pos = float4(t * float2(2, -2) + float2(-1, 1), 0, 1);
  o.r0 = float4(t, t);
  o.r1 = 0;
  return o;
}

// fisheye: oC0.xy = uv*c3.xy + c2.xy (uvScale/uvTrans), oC0.zw = raw uv.
VsOut vs_scaled(uint vid : SV_VertexID) {
  VsOut o;
  float2 t = float2((vid << 1) & 2, vid & 2);
  o.pos = float4(t * float2(2, -2) + float2(-1, 1), 0, 1);
  o.r0 = float4(t * c[243].xy + c[242].xy, t);
  o.r1 = 0;
  return o;
}

// ---------------------------------------------------------------------------
// Depth pack: native D32 MSAA depth (sample 0) -> the console D24S8-as-8888
// texel layout the ported shaders reconstruct: sample.x = mid byte,
// sample.y = low byte, sample.z = stencil (the game-stencil mark, t1, when
// c[249].y > 0: see RenderStencilMark), sample.w = high
// byte. dp3 with the baked weights (255/16776960, 255/256, 255/65536) then
// yields the exact 24-bit depth (visualfx c254.xyz, dof_mb c255.xyz).
#ifdef PFX_MSAA
Texture2DMS<float> depth_ms : register(t5);
// The mark at the scene's sample count. Shares t1 with the global Texture2D
// t1, which the depth pack (the only PFX_MSAA entry) never references.
Texture2DMS<float> mark_ms : register(t1);
#else
Texture2D<float> depth_1x : register(t5);
#endif

float4 ps_depthpack(VsOut i) : SV_Target {
#ifdef PFX_MSAA
  float d = depth_ms.Load(int2(i.pos.xy), 0);
  float st = c[249].y > 0.5 ? mark_ms.Load(int2(i.pos.xy), 0) : 0.0;
#else
  float d = depth_1x.Load(int3(int2(i.pos.xy), 0));
  float st = c[249].y > 0.5 ? t1.Load(int3(int2(i.pos.xy), 0)).r : 0.0;
#endif
  float D = min(floor(d * 16777216.0), 16777215.0);
  float hi = floor(D / 65536.0);
  float mid = floor((D - hi * 65536.0) / 256.0);
  float lo = D - hi * 65536.0 - mid * 256.0;
  // The stencil byte: the mark stores ref/255 in UNORM8, exact.
  return float4(mid * (1.0 / 255.0), lo * (1.0 / 255.0), st, hi * (1.0 / 255.0));
}

// Plain bilinear blit: the quarter-res accumulation update (the game feeds
// visualfx tf3 from a motion-accumulation buffer; natively it is approximated
// per skate3_native_render_scene_photo_native_accum).
float4 ps_blit(VsOut i) : SV_Target {
  return t0.SampleLevel(s_lin, i.r0.zw, 0);
}

// Debug visualization (photo_native_debug): c[249].x selects
//  1 = t0 alpha (the visualfx CoC) as grayscale
//  2 = packed-depth reconstruction from t1 (r = (1-d)*100 sat, g/b = frac
//      ramps of d for band inspection)
float4 ps_pfx_debug(VsOut i) : SV_Target {
  if (c[249].x < 1.5) {
    float a = t0.SampleLevel(s_lin, i.r0.zw, 0).w;
    return float4(a, a, a, 1);
  }
  float4 s = t1.SampleLevel(s_pt, i.r0.zw, 0);
  float d = s.y * 1.51991853e-05 + s.w * 0.99609381 + s.x * 0.00389099144;
  return float4(saturate((1.0 - d) * 100.0), frac(d * 256.0), frac(d * 4096.0), 1);
}

// ---------------------------------------------------------------------------
// postfx_visualfxPS (bit-exact transcription). t0 = scene,
// t1 = packed depth, t3 = quarter-res accumulation buffer.
float4 ps_visualfx(VsOut i) : SV_Target {
  // THIS build's ucode (shader_dump ps_postfx_visualfxPS.bin; the previous
  // transcription followed another build: it read c16, which this shader
  // never uses). Literals from the .updb container: c253 = (2, 0, 0.5, 0),
  // c254 = (-0.2, 1, 1/255, 1.41412354), c255 = (1.51991853e-05,
  // 0.99609381, 0.00389099144, 1) (the per-pass literal rows are staged by
  // the renderer). Colour rides as (B, G, R, 1) through r2.
  float2 uv = i.r0.xy;
  float4 r0 = i.r0;
  float4 r1 = i.r1;
  float4 r2 = 0, r3 = 0, r4 = 0, r5 = 0;
  float4 oC0 = 0;
  float ps = 0.0;
  // 4: tfetch2D r3.xywz, tf1  (positional: r3.z <- s.w, r3.w <- s.z)
  {
    const float4 d = t1.SampleLevel(s_pt, uv, 0);
    r3 = float4(d.x, d.y, d.w, d.z);
  }
  // 5-6
  r4.xyz = t0.SampleLevel(s_lin, uv, 0).xyz;
  r2.xyz = t3.SampleLevel(s_lin, uv, 0).xyz;
  // 7: add r5.zw, c1.yx, -c0.yx
  r5.z = c[1].y - c[0].y;
  r5.w = c[1].x - c[0].x;
  // 8: mad r2.xyz, r4.zyx, c254.w, r2.zyx
  {
    const float3 o = r2.xyz;
    r2.x = r4.z * c[254].w + o.z;
    r2.y = r4.y * c[254].w + o.y;
    r2.z = r4.x * c[254].w + o.x;
  }
  // 9: dp3 r0.y, r3.yzx, c255.xyz  +  maxs r2.w, c255.w
  r0.y = r3.y * c[255].x + r3.z * c[255].y + r3.x * c[255].z;
  r2.w = c[255].w;
  // 10: mad r3.xyz, r0.y, c5.xyz, r1.xyz
  r3.xyz = r0.y * c[5].xyz + r1.xyz;
  // dp4 cN.zxyw . r2.xzyw = cN . (R, G, B, 1)
  const float4 rgb1 = float4(r2.z, r2.y, r2.x, r2.w);
  // 11-12: color-matrix rows  +  maxs_sat r0.x, r3.w
  r1.x = dot(c[6], rgb1);
  r1.y = dot(c[7], rgb1);
  r0.x = saturate(r3.w);
  // 13: dp4 r1.z, c8  +  subsc r1.w, -c3.x, -r0.y
  r1.z = dot(c[8], rgb1);
  r1.w = r0.y - c[3].x;
  // 14: mad r0.yz, r3.z, r0.zw, r3.xy
  {
    const float oz = r0.z, ow = r0.w;
    r0.y = r3.z * oz + r3.x;
    r0.z = r3.z * ow + r3.y;
  }
  // 15: dp4 r5.x, c11  +  rcp r0.w, r1.w
  r5.x = dot(c[11], rgb1);
  r0.w = 1.0 / r1.w;
  // 16: mul r3.xy, r0.w, c254.xy  +  subsc r0.w, -c255.w, -r0.x
  r3.x = XMul(r0.w, c[254].x);
  r3.y = XMul(r0.w, c[254].y);
  r0.w = r0.x - c[255].w;
  // 17: dp4 r5.y, c10  +  mulsc r3.z, c4.x, r0.y
  r5.y = dot(c[10], rgb1);
  r3.z = c[4].x * r0.y;
  // 18: add_sat r0.xw, r0.wx, r0.xx  +  mulsc r3.w, c4.x, r0.z
  {
    const float ox = r0.x, ow = r0.w;
    r0.x = saturate(ow + ox);
    r0.w = saturate(ox + ox);
  }
  r3.w = c[4].x * r0.z;
  // 19: dp2add r4.x, r3.zw, r3.zw, c253.y
  r4.x = r3.z * r3.z + r3.w * r3.w + c[253].y;
  // 20: mul r0.yz, r3.yx, c2.x
  {
    const float oy = r3.y, ox = r3.x;
    r0.y = XMul(oy, c[2].x);
    r0.z = XMul(ox, c[2].x);
  }
  // 21: mad_sat r3.xw, r0.y, c12.xy, c13.xy
  r3.x = saturate(r0.y * c[12].x + c[13].x);
  r3.w = saturate(r0.y * c[12].y + c[13].y);
  // 22: sge r1.w, r0.y, c0.y  +  addsc r4.y, c253.x, r0.z
  r1.w = r0.y >= c[0].y ? 1.0 : 0.0;
  r4.y = c[253].x + r0.z;
  // 23: mad r3.yz, r5.zw, r1.w, c0.yx
  r3.y = r5.z * r1.w + c[0].y;
  r3.z = r5.w * r1.w + c[0].x;
  // 24: dp4 r1.w, c9  +  maxs ps, r4.y
  r1.w = dot(c[9], rgb1);
  ps = r4.y;
  // 25: mul r1, r1, r3.xxxw  +  muls_prev_sat r0.x, r0.x * ps
  r1 = r1 * float4(r3.x, r3.x, r3.x, r3.w);
  r0.x = saturate(r0.x * ps);
  // 26: mad r1.yz, r5.xy, r3.w, r1.zy
  {
    const float oy = r1.y, oz = r1.z;
    r1.y = r5.x * r3.w + oz;
    r1.z = r5.y * r3.w + oy;
  }
  // 27: add r4.y, r3.y, -r0.y  +  subsc r3.x, c255.w, r0.x
  r4.y = r3.y - r0.y;
  r3.x = c[255].w - r0.x;
  // 28: mul_sat r0.xy, r4.yx, r3.zx  +  adds r1.x, r1.x + r1.w
  {
    const float a = saturate(r4.y * r3.z);
    const float b = saturate(r4.x * r3.x);
    r0.x = a;
    r0.y = b;
  }
  r1.x = r1.x + r1.w;
  // 29: add r2.xyz, r2.zxy, -r1.xyz  +  mulsc r0.z, c15.x, r0.w
  {
    const float3 o = r2.xyz;
    r2.x = o.z - r1.x;
    r2.y = o.x - r1.y;
    r2.z = o.y - r1.z;
  }
  r0.z = c[15].x * r0.w;
  // 30: mad oC0.xyz, r0.z, r2.xzy, r1.xzy
  oC0.x = r0.z * r2.x + r1.x;
  oC0.y = r0.z * r2.z + r1.z;
  oC0.z = r0.z * r2.y + r1.y;
  // 31: max r0.z, r0.y, c254.z  +  mulsc r0.y, c253.z, r0.x
  {
    const float oy = r0.y;
    r0.z = max(oy, c[254].z);
    r0.y = c[253].z * r0.x;
  }
  // 32: mulsc r0.x, c14.x, r0.y
  r0.x = c[14].x * r0.y;
  // 33: max_sat oC0.w, r0.z, r0.x
  oC0.w = saturate(max(r0.z, r0.x));
  return oC0;
}

// ---------------------------------------------------------------------------
// bloom_dof_motionblur_dof_dowsamplePS (bit-exact): 4-tap
// alpha-weighted downsample of the visualfx output into the DOF half buffer.
float4 ps_dof_down(VsOut i) : SV_Target {
  float4 r0 = i.r0;
  float4 r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
  float4 c255 = c[255];

  r1.y = r0.y;
  r1.z = r0.x;
  r1.x = r0.x + c255.x;
  r1.w = r0.y + c255.y;
  r4 = t0.SampleLevel(s_lin, float2(r1.x, r1.y), 0);
  r5 = t0.SampleLevel(s_lin, float2(r1.z, r1.w), 0);
  r1 = t0.SampleLevel(s_lin, float2(r1.x, r1.w), 0);
  r2 = t0.SampleLevel(s_lin, float2(r0.x, r0.y), 0);
  r3.x = r2.w + c255.w;
  r0.x = r1.x * r1.w;
  r0.y = r1.z * r1.w;
  r0.z = r1.y * r1.w;
  {
    float ny = r5.x * r5.w + r0.x;
    float nz = r5.y * r5.w + r0.z;
    float nw = r5.z * r5.w + r0.y;
    r0.y = ny; r0.z = nz; r0.w = nw;
  }
  r0.x = r3.x + r4.w;
  r0.x = r0.x + r5.w;
  {
    float ny = r4.x * r4.w + r0.y;
    float nz = r4.z * r4.w + r0.w;
    float nw = r4.y * r4.w + r0.z;
    r0.y = ny; r0.z = nz; r0.w = nw;
  }
  r1.x = r3.x * r2.x + r0.y;
  r1.y = r3.x * r2.y + r0.w;
  r1.z = r3.x * r2.z + r0.z;
  r0.x = r0.x + r1.w;
  r0.y = 1.0 / r0.x;
  float4 oC0;
  oC0.x = XMul(r1.x, r0.y);
  oC0.y = XMul(r1.y, r0.y);
  oC0.z = XMul(r1.z, r0.y);
  oC0.w = c255.z * r0.x;
  return oC0;
}

// ---------------------------------------------------------------------------
// bloom_dof_tap9dofMotionBlurPS (bit-exact): 9-tap motion-blur +
// DOF pass. t0 = DOF half buffer, t1 = packed depth (motion reconstructed
// from depth + camera rows in c1).
float4 ps_dof_mb(VsOut i) : SV_Target {
  float4 r0 = i.r0;
  // r1 is blurVS's second interpolator (o1 = uv.x*c0 + uv.y*c1 + c2, the
  // per-pixel view ray): /*11*/ reads r1.xyz before anything writes it, so
  // the reprojected position (and thus the blur vector) depends on it.
  float4 r1 = i.r1;
  float4 r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0;
  float ps = 0;
  float4 c254 = c[254], c255 = c[255];

  // /*6*/
  r2.x = r0.x + c[7].x;  r2.y = r0.y + c[7].y;
  // /*7*/ tfetch2D r1.___w, r2.xy, tf0
  r1.w = t0.SampleLevel(s_lin, r2.xy, 0).w;
  // /*8*/ tfetch2D r2.xyw_, r0.xy, tf1  (dest xyw_ -> r2.x,r2.y,r2.z <- s.x,s.y,s.w)
  { float4 vel = t1.SampleLevel(s_pt, r0.xy, 0); r2.x = vel.x; r2.y = vel.y; r2.z = vel.w; }
  // /*9*/ dp3 r2._y__, r2.yzxx, c255.xyzz  +  mulsc r2.x, c2.x, r1.w
  { float dp = r2.y * c255.x + r2.z * c255.y + r2.x * c255.z; r2.y = dp; }
  r2.x = c[2].x * r1.w;
  // /*10*/
  r2.x = max(r2.x, c254.y);
  // /*11*/ mad r1.xyz_, r2.yyyy, c1.xzyy, r1.xzyy   (r1.xyz = view ray)
  { float nx = r2.y * c[1].x + r1.x, ny = r2.y * c[1].z + r1.z, nz = r2.y * c[1].y + r1.y; r1.x = nx; r1.y = ny; r1.z = nz; }
  // /*12*/ mad r1.xy__, r1.yyyy, r0.zwww, r1.xzzz
  { float nx = r1.y * r0.z + r1.x, ny = r1.y * r0.w + r1.z; r1.x = nx; r1.y = ny; }
  // /*13*/ mul r1._yz_, r1.xxyy, c0.xxxx
  { float ny = r1.x * c[0].x, nz = r1.y * c[0].x; r1.y = ny; r1.z = nz; }
  // /*14*/ dp2add r0.___w, r1.yzzz, r1.yzzz, c255.wwww
  r0.w = r1.y * r1.y + r1.z * r1.z + c255.w;
  // /*15*/ add r1.x, r1.y, -r2.x  +  sqrt_sat r0.w, |r0.w|
  r1.x = r1.y - r2.x;
  r0.w = saturate(sqrt(abs(r0.w)));
  // /*16*/ mul r1._yz_, r1.zzxx, r0.wwww  +  addsc r0.w, c254.z, r0.w
  { float ny = r1.z * r0.w, nz = r1.x * r0.w; r1.y = ny; r1.z = nz; }
  r0.w = c254.z + r0.w;
  // /*17*/
  r1.x = r1.z + r2.x;

  // /*18-27*/ tap centers
  r2.x = r1.x * c[11].w + r0.x;   r2.y = r1.y * c[11].w + r0.y;
  r4.xy = r1.xy * c[9].w;
  r6.xy = r1.xy * c[7].w;
  r5.xy = r1.xy * c[5].w;        r4.z = c[10].w * r1.x;
  r3.xy = r1.xy * c[3].w;        r6.z = c[8].w * r1.x;
  r3.z = r1.x * c[4].w; r3.w = r1.y * c[4].w;   r5.z = c[6].w * r1.x;
  r3 += r0.xyxy;                 r5.w = c[6].w * r1.y;
  r5 += r0.xyxy;                 r6.w = c[8].w * r1.y;
  r6 += r0.xyxy;                 r4.w = c[10].w * r1.y;
  r1 = r4 + r0.xyxy;

  // /*28-37*/ taps
  r4 = t0.SampleLevel(s_lin, r1.zw, 0);
  r1 = t0.SampleLevel(s_lin, r1.xy, 0);
  float4 r12 = t0.SampleLevel(s_lin, r6.zw, 0);
  float4 r10 = t0.SampleLevel(s_lin, r6.xy, 0);
  float4 r9  = t0.SampleLevel(s_lin, r5.zw, 0);
  float4 r8  = t0.SampleLevel(s_lin, r5.xy, 0);
  float4 r7  = t0.SampleLevel(s_lin, r3.zw, 0);
  r5         = t0.SampleLevel(s_lin, r3.xy, 0);
  r2         = t0.SampleLevel(s_lin, r2.xy, 0);
  r0.x       = t0.SampleLevel(s_lin, r0.xy, 0).w;   // /*37*/ r0.w___ -> r0.x <- s.w

  // /*38-49*/ coc weights (r0.w = base blur radius term)
  r3.x = saturate(r0.w + c[9].z);    r0.y = saturate(c[10].z + r0.w);
  r3.y = saturate(r0.w + c[11].z);   r0.z = saturate(c[3].z + r0.w);
  r3.w = saturate(r3.y * r2.w + r0.w);
  r6.x = r0.z * r5.w;                 r0.z = saturate(c[4].z + r0.w);
  r6.y = r0.z * r7.w;                 r0.z = saturate(c[5].z + r0.w);
  r6.z = r0.z * r8.w;                 r0.z = saturate(c[6].z + r0.w);
  r6.w = r0.z * r9.w;                 r0.z = saturate(c[7].z + r0.w);
  r11.x = r0.z * r10.w;               r0.z = saturate(c[8].z + r0.w);
  r11.z = r3.x * r1.w;                ps = r0.z;                 // maxs r0._,r0.zz
  r11.w = r0.y * r4.w;                r11.y = r12.w * ps; ps = r12.w * ps;
  r11 = saturate(r0.w + r11);         ps = r0.x;                 // maxs r0.x,r0.xx
  r6  = saturate(r0.w + r6);          ps = r11.w;                // maxs r0._,r11.ww

  // /*50-62*/ weighted color accumulation
  r5.w = (r6.w + r6.z + r6.x + r6.y) * c254.x;   r3.x = r4.x * ps; ps = r4.x * ps;
  { float nx = r11.z * r1.x, ny = r11.z * r1.y, nz = r11.z * r1.z; r1.x = nx; r1.y = ny; r1.z = nz; } ps = r11.w;
  r1.w = (r11.w + r11.z + r11.y + r11.x) * c254.x;  r3.y = r4.y * ps; ps = r4.y * ps;
  r0.y = r11.y * r12.x; r0.z = r11.y * r12.y; r0.w = r11.y * r12.z;   ps = r11.w;
  // /*54*/ mad r0._yzw, r11.xxxx, r10.xxzy, r0.yywz
  { float ny = r11.x * r10.x + r0.y, nz = r11.x * r10.z + r0.w, nw = r11.x * r10.y + r0.z; r0.y = ny; r0.z = nz; r0.w = nw; }
  // /*55*/ mad r0._yzw, r6.wwww, r9.xxyz, r0.yywz
  { float ny = r6.w * r9.x + r0.y, nz = r6.w * r9.y + r0.w, nw = r6.w * r9.z + r0.z; r0.y = ny; r0.z = nz; r0.w = nw; }
  // /*56*/ mad r0._yzw, r6.zzzz, r8.xxzy, r0.yywz
  { float ny = r6.z * r8.x + r0.y, nz = r6.z * r8.z + r0.w, nw = r6.z * r8.y + r0.z; r0.y = ny; r0.z = nz; r0.w = nw; }
  // /*57*/ mad r0._yzw, r6.yyyy, r7.xxyz, r0.yywz
  { float ny = r6.y * r7.x + r0.y, nz = r6.y * r7.y + r0.w, nw = r6.y * r7.z + r0.z; r0.y = ny; r0.z = nz; r0.w = nw; }
  // /*58*/ mad r5.xyz_, r6.xxxx, r5.xzyy, r0.ywzz
  { float nx = r6.x * r5.x + r0.y, ny = r6.x * r5.z + r0.w, nz = r6.x * r5.y + r0.z; r5.x = nx; r5.y = ny; r5.z = nz; }
  // /*59*/ add r1, r5.xzyw, r1  +  muls_prev r3.z, r4.z
  r1 = float4(r5.x + r1.x, r5.z + r1.y, r5.y + r1.z, r5.w + r1.w);
  r3.z = r4.z * ps; ps = r4.z * ps;
  // /*60*/ add r1, r1.xzyw, r3.xzyw  +  maxs r0.y, c254.ww
  r1 = float4(r1.x + r3.x, r1.z + r3.z, r1.y + r3.y, r1.w + r3.w);
  r0.y = c254.w; ps = c254.w;
  // /*61*/ rcp r0.z, r1.w
  r0.z = 1.0 / r1.w;
  // /*62*/ mad r1.xyz_, r3.wwww, r2.xyzz, r1.xzyy
  { float nx = r3.w * r2.x + r1.x, ny = r3.w * r2.y + r1.z, nz = r3.w * r2.z + r1.y; r1.x = nx; r1.y = ny; r1.z = nz; }
  // /*63*/ mul oC0.xyz_, r1.xyzz, r0.zzzz  +  maxs oC0.w, r0.xy
  float4 oC0;
  oC0.x = XMul(r1.x, r0.z);
  oC0.y = XMul(r1.y, r0.z);
  oC0.z = XMul(r1.z, r0.z);
  oC0.w = max(r0.x, r0.y);
  return oC0;
}

// ---------------------------------------------------------------------------
// bloom_dof_tap9dofPS (bit-exact): 9-tap variable-radius CoC blur.
float4 ps_dof(VsOut i) : SV_Target {
  float4 r0 = i.r0;
  float4 r2 = 0, r3 = 0, r4 = 0, r5 = 0;

  float4 r1 = t0.SampleLevel(s_lin, float2(r0.x, r0.y), 0);
  r0.z = c[0].x * r1.w;
  r0.z = max(r0.z, c[255].x);
  float rz = r0.z;

  r5.xy = rz * c[8].xy;
  r4.xy = rz * c[6].xy;
  r3.xy = rz * c[3].xy;   r5.z = c[9].x * rz;
  r2.xy = rz * c[1].xy;   r4.z = c[7].x * rz;
  r2.z = rz * c[2].x;     r2.w = rz * c[2].y;   r3.z = c[4].x * rz;
  r2 += r0.xyxy;          r3.w = c[4].y * rz;
  r3 += r0.xyxy;          r4.w = c[7].y * rz;
  r4 += r0.xyxy;          r5.w = c[9].y * rz;
  r0 = r5 + r0.xyxy;

  float4 r12 = t0.SampleLevel(s_lin, r0.zw, 0);
  float4 r11 = t0.SampleLevel(s_lin, r0.xy, 0);
  float4 r10 = t0.SampleLevel(s_lin, r4.zw, 0);
  float4 r9 = t0.SampleLevel(s_lin, r4.xy, 0);
  float4 r8 = t0.SampleLevel(s_lin, r3.zw, 0);
  float4 r7 = t0.SampleLevel(s_lin, r3.xy, 0);
  float4 r6 = t0.SampleLevel(s_lin, r2.zw, 0);
  r5 = t0.SampleLevel(s_lin, r2.xy, 0);

  r3.y = r8.w * c[4].z;   r0.x = r7.w;
  r3.x = r1.w * c[5].z;   r3.z = c[3].z * r0.x;
  r2.z = r10.w * c[7].z;  r0.y = r9.w;
  r2.x = r12.w * c[9].z;  r0.x = r11.w;
  r4.x = r2.x * r12.x; r4.y = r2.x * r12.y; r4.z = r2.x * r12.z;
  r2.y = c[8].z * r0.x;
  {
    float nx = r2.y * r11.x, nz = r2.y * r11.y, nw = r2.y * r11.z;
    r0.x = nx; r0.z = nz; r0.w = nw;
    r2.w = c[6].z * r0.y;
  }
  { float nx = r2.z * r10.x + r0.x, ny = r2.z * r10.z + r0.w, nz = r2.z * r10.y + r0.z; r0.x = nx; r0.y = ny; r0.z = nz; }
  { float nx = r2.w * r9.x + r0.x, ny = r2.w * r9.y + r0.z, nz = r2.w * r9.z + r0.y; r0.x = nx; r0.y = ny; r0.z = nz; }
  { float nx = r3.x * r1.x + r0.x, ny = r3.x * r1.z + r0.z, nz = r3.x * r1.y + r0.y; r0.x = nx; r0.y = ny; r0.z = nz; }
  { float nx = r3.y * r8.x + r0.x, ny = r3.y * r8.y + r0.z, nz = r3.y * r8.z + r0.y; r0.x = nx; r0.y = ny; r0.z = nz; }
  { float nx = r3.z * r7.x + r0.x, ny = r3.z * r7.z + r0.z, nz = r3.z * r7.y + r0.y; r0.x = nx; r0.y = ny; r0.z = nz; }
  r4.w = r6.w * c[2].z;   r5.w = c[1].z * r5.w;
  { float nx = r4.w * r6.x + r0.x, ny = r4.w * r6.y + r0.z, nz = r4.w * r6.z + r0.y; r0.x = nx; r0.y = ny; r0.z = nz; }
  { float nx = r5.w * r5.x + r0.x, ny = r5.w * r5.z + r0.z, nz = r5.w * r5.y + r0.y; r5.x = nx; r5.y = ny; r5.z = nz; }
  r0 = float4(r5.w + r4.w, r5.x + r4.x, r5.z + r4.y, r5.y + r4.z);
  r3.z = r0.x + r3.z;
  r3.y = r3.z + r3.y;
  r3.x = r3.y + r3.x;
  r2.w = r3.x + r2.w;
  r2.z = r2.w + r2.z;
  r2.y = r2.z + r2.y;
  r2.x = r2.y + r2.x;
  r0.x = 1.0 / r2.x;
  float4 oC0;
  oC0.x = XMul(r0.y, r0.x);
  oC0.y = XMul(r0.z, r0.x);
  oC0.z = XMul(r0.w, r0.x);
  oC0.w = r1.w;
  return oC0;
}

// ---------------------------------------------------------------------------
// postfx_uberPS, transcribed from THIS build's ucode (shader_dump
// ps_postfx_uberPS.bin; the previous port followed another build's uber,
// whose extra border term and c5 output multiplier read a stale c5 on the
// gameplay cinematic chain - bails/replays rendered black). Temporal-jitter
// combine + 3D grade LUT + grain + depth mask, output * c4 * 0.5.
// t0 = graded sharp scene, t1 = packed depth, t4 = 3D LUT, t6 = grain,
// t7 = DOF-blurred half buffer. Literals c253 = (2, 0.5, -1, 0),
// c254 = (1/64, 63/64, 31/32, -31/32), c255.x = 1.
float4 ps_uber(VsOut i) : SV_Target {
  float4 r0 = i.r0;
  float4 r1, r2 = 0, r3, r4 = 0, r5;
  // 4: mad r2.x, r0.x, c253.x, c253.z
  r2.x = r0.x * c[253].x + c[253].z;
  // 5: addsc r2.y, c1.x, r0.y
  r2.y = c[1].x + r0.y;
  // 6: mul r1, c4, c253.y  +  frcs r4.y, r2.y
  r1 = c[4] * c[253].y;
  r4.y = frac(r2.y);
  // 7: mad r2.y, r4.y, c253.x, c253.z
  r2.y = r4.y * c[253].x + c[253].z;
  // 8: mad r2.zw, r2.xy, c0.y, -r2.xy
  r2.z = r2.x * c[0].y - r2.x;
  r2.w = r2.y * c[0].y - r2.y;
  // 9-11: r2.xy = max((|r2.xy| - c0.z) * c0.z, c253.w)
  r2.x = max((abs(r2.x) - c[0].z) * c[0].z, c[253].w);
  r2.y = max((abs(r2.y) - c[0].z) * c[0].z, c[253].w);
  // 12: mul r4.zw, r2.zw, r2.xy  +  maxs r4.x, r0.x
  r4.z = r2.z * r2.x;
  r4.w = r2.w * r2.y;
  r4.x = r0.x;
  // 13-16: r3 = lerp(sharp, dof, dof.a)
  r3 = t7.SampleLevel(s_lin, r4.xy, 0);
  r2 = t0.SampleLevel(s_lin, r4.xy, 0);
  r5 = r3 - r2;
  r3 = r5 * r3.w + r2;
  // 17: mad r2.xyz, r3.yxz, c254.zwz, c254.xyx  (LUT coordinates)
  {
    float3 lc;
    lc.x = r3.y * c[254].z + c[254].x;
    lc.y = r3.x * c[254].w + c[254].y;
    lc.z = r3.z * c[254].z + c[254].x;
    r2.xyz = lc;
  }
  // 18: add r5.y, r4.w, r4.y  +  adds r5.x, r0.x + r0.x
  r5.y = r4.w + r4.y;
  r5.x = r0.x + r0.x;
  // 19-21: jittered sharp tap
  {
    float2 j = float2(r5.x + r4.z, r5.y + r4.y) * c[253].y;
    r0 = t0.SampleLevel(s_lin, j, 0);
  }
  // 22-24: LUT, grain, packed depth (tfetch2D r4.z___: r4.x <- s.z)
  r5 = t4.SampleLevel(s_lin, r2.xyz, 0);
  r2 = t6.SampleLevel(s_wrap, r4.xy, 0);
  r4.x = t1.SampleLevel(s_pt, r4.xy, 0).z;
  // 25: mul r4, r4.x, c3
  r4 = r4.x * c[3];
  // 26-27: grain -> (g * 2 - 1) * c3 + 1
  r2 = r2 * c[253].x + c[253].z;
  r2 = r2 * c[3] + c[255].x;
  // 28-30: LUT blend by c2.x, times grain
  r5 = r5 - r3;
  r5 = r5 * c[2].x + r3;
  r2 = r5 * r2;
  // 31-32: depth-masked mix back toward the ungrained colour
  r3 = r3 - r2;
  r2 = r4 * r3 + r2;
  // 33-35: temporal combine with the jittered tap
  r3 = r2 - r0;
  r0 = r3 * c[0].x + r0;
  r0 = r2 + r0;
  // 36: mul oC0, r1, r0
  return r1 * r0;
}

// ---------------------------------------------------------------------------
// postfx_basictex_fisheye_opaquePS (bit-exact): lens warp +
// vignette gradient + tint. t0 = uber output, t2 = the 512x2 vignette
// gradient texture (resolved from the game's own asset via fetch words).
float4 ps_fisheye(VsOut i) : SV_Target {
  float4 r0 = i.r0;
  float4 r1 = 0;

  r1.y = max(c[0].y, c[0].y);
  {
    float r0x_old = r0.x;
    float3 nr;
    nr.x = r0x_old * c[255].w + c[255].y;
    nr.y = r0.y * c[255].z + c[255].x;
    nr.z = r0x_old * c[255].z + c[255].x;
    r0.xyz = nr;
  }
  r1.x = r0.x * r0.x;
  r1.z = r0.y * r0.y;
  r1.w = r0.z * r0.z;
  float ps = c[0].x;
  {
    float r0_x = r1.x + r1.z;
    float r0_w = r1.w + r1.z;
    r0.x = r0_x; r0.w = r0_w;
  }
  r1.x = c[255].z + ps;
  r1.z = r0.w * c[1].x;
  r1.w = sqrt(abs(r0.x));
  r1.x = r1.w * (-c[1].z) + r1.x;
  r0.x = r1.z * r0.w + c[1].y;
  {
    float r0x_new = r0.x;
    float rX = r0.z * r0x_new + c[254].y;
    float rY = r0.y * r0x_new + c[254].y;
    r0.x = rX; r0.y = rY;
  }
  r0.xyz = t0.SampleLevel(s_lin, r0.xy, 0).xyz;
  r1.xyz = t2.SampleLevel(s_lin, r1.xy, 0).xyz;
  r1.xyz = saturate(r1.xyz + c[0].w);
  r1.xyz = r1.xyz + c[254].x;
  r1.xyz = r1.xyz * c[2].y + c[255].z;
  float4 oC0;
  oC0.xyz = r1.xyz * r0.xyz;
  oC0.w = 1.0;
  return oC0;
}

// ---------------------------------------------------------------------------
// postfx_basictex_fisheyePS (bit-exact): the camera-man / filmer lens. The
// same ucode as fisheye_opaquePS (same literal rows c254/c255) except that
// both fetches and the tint/vignette math keep all four channels and the
// output alpha is vignette.a * scene.a instead of 1.
float4 ps_fisheye_blend(VsOut i) : SV_Target {
  float4 r0 = i.r0;
  float4 r1 = 0;

  r1.y = max(c[0].y, c[0].y);
  {
    float r0x_old = r0.x;
    float3 nr;
    nr.x = r0x_old * c[255].w + c[255].y;
    nr.y = r0.y * c[255].z + c[255].x;
    nr.z = r0x_old * c[255].z + c[255].x;
    r0.xyz = nr;
  }
  r1.x = r0.x * r0.x;
  r1.z = r0.y * r0.y;
  r1.w = r0.z * r0.z;
  float ps = c[0].x;
  {
    float r0_x = r1.x + r1.z;
    float r0_w = r1.w + r1.z;
    r0.x = r0_x; r0.w = r0_w;
  }
  r1.x = c[255].z + ps;
  r1.z = r0.w * c[1].x;
  r1.w = sqrt(abs(r0.x));
  r1.x = r1.w * (-c[1].z) + r1.x;
  r0.x = r1.z * r0.w + c[1].y;
  {
    float r0x_new = r0.x;
    float rX = r0.z * r0x_new + c[254].y;
    float rY = r0.y * r0x_new + c[254].y;
    r0.x = rX; r0.y = rY;
  }
  r0 = t0.SampleLevel(s_lin, r0.xy, 0);
  r1 = t2.SampleLevel(s_lin, r1.xy, 0);
  r1 = saturate(r1 + c[0].w);
  r1 = r1 + c[254].x;
  r1 = r1 * c[2].y + c[255].z;
  return r1 * r0;
}

// ---------------------------------------------------------------------------
// postfx_basictex_fisheye_noisePS (bit-exact): the respawn "TV noise"
// variant of the fisheye pass. Lens warp + vignette gradient + tint, the
// scene scaled by c4.x, plus two scrolling taps of the noise texture
// (tiled 10x5.625 / 11x6.625, scrolled by c3) dotted with c5/c6 and added
// at c4.y. t0 = the finished frame, t2 = the vignette gradient, t3 = the
// noise texture (the game's tf5; t5 is the depth slot in this file).
// Literal rows from the shader asset footer:
//   c253 = (1, -0.888889, -1, 0), c254 = (6.625, 11, -0.5, 1.777778),
//   c255 = (-2, 0.5, 5.625, 10).
float4 ps_fisheye_noise(VsOut i) : SV_Target {
  const float4 k253 = float4(1.0, -0.888888896, -1.0, 0.0);
  const float4 k254 = float4(6.625, 11.0, -0.5, 1.77777779);
  const float4 k255 = float4(-2.0, 0.5, 5.625, 10.0);
  float4 r0 = i.r0;
  float4 r1 = 0, r2 = 0, r3, r4;
  float ps;

  r1.x = r0.x * k254.y + c[3].z;
  r1.y = r0.y * k254.x + c[3].w;
  r1.z = r0.x * k255.w + c[3].x;
  r1.w = r0.y * k255.z + c[3].y;
  r3 = t3.SampleLevel(s_wrap, r1.zw, 0);
  r4 = t3.SampleLevel(s_wrap, r1.xy, 0);
  r2.y = r0.y + k254.z;
  r2.z = r0.x + k254.z;
  r2.x = r0.x * k254.w + k253.y;

  r1.z = dot(r4, c[6]);
  r1.y = dot(r4, c[6]);
  ps = c[0].x;
  r1.x = dot(r3, c[5]);
  r0.x = ps + k253.x;
  ps = r1.y;
  r0.y = r2.x * r2.x;
  r0.z = r2.y * r2.y;
  r0.w = r2.z * r2.z;
  {
    float nz = r0.y + r0.z;
    float nw = r0.w + r0.z;
    r0.z = nz;
    r0.w = nw;
  }
  r0.y = ps + r1.x;
  r1.w = r0.w * c[1].x;
  r1.y = sqrt(abs(r0.z));

  r0.z = r1.w * r0.w + c[1].y;
  {
    float s = r0.z;
    r0.z = r2.z * s;
    r0.w = r2.y * s;
  }
  ps = r1.y;
  r0.y = r0.y + k255.x;
  r0.z = r0.z + k255.y;
  r0.w = r0.w + k255.y;
  r1.y = ps * (-c[1].z);
  {
    float ny = r0.x + r1.y;
    float nw = r0.y + r1.z;
    r1.y = ny;
    r1.w = nw;
  }
  r1.z = c[0].y;
  r2 = t2.SampleLevel(s_lin, r1.yz, 0);
  r0 = t0.SampleLevel(s_lin, r0.zw, 0);

  r3.xyz = r0.yzw * c[4].x;
  r2 = saturate(r2 + c[0].w);
  r2 = r2 + k253.z;
  r2 = r2 * c[2].y + k253.x;
  r2.x = r2.x * c[4].x;
  ps = r2.x;
  r0.yzw = r3.xyz * r2.yzw;
  r1.x = saturate(r1.w + r1.x);
  r0.x = ps * r0.x;
  return r1.xxxx * c[4].y + r0;
}

// ===========================================================================
// The game's bloom chain + final postfx composite, transcribed from this
// build's own ucode (bloom_dof_*.fpo / postfx_fast_uberPS.fpo, identical to
// the shader dump). The PS rows c0..c10 are the game's own, captured at its
// draws each frame (FrameScene::GameBloom); literals are inline. Game pixel
// centers interpolate uv = i/W (its quad VSs add no half-texel offset on
// these passes), ours (i + 0.5)/W: GameUv() shifts back by half a
// destination texel (c[248].zw = 0.5 / dest size).
float2 GameUv(VsOut i) { return i.r0.xy - c[248].zw; }

// Our scene target -> the game's 1152x640 scene (our analogue of the game
// rendering at 1152x640): four bilinear taps inside the destination texel.
float4 ps_bloom_scene(VsOut i) : SV_Target {
  float2 q = c[248].zw * 0.5;
  float4 s = t0.SampleLevel(s_lin, i.r0.xy + float2(-q.x, -q.y), 0) +
             t0.SampleLevel(s_lin, i.r0.xy + float2(q.x, -q.y), 0) +
             t0.SampleLevel(s_lin, i.r0.xy + float2(-q.x, q.y), 0) +
             t0.SampleLevel(s_lin, i.r0.xy + float2(q.x, q.y), 0);
  return s * 0.25;
}

// bloom_dof_tap4_minusthresholdPS: 4 taps (c0..c3 offsets), squared
// average, minus c4.w, scaled by c4.z, plus c4.x, then the c5..c7 colour
// matrix; alpha = the tap average's luma (literals c255 = 0.3/0.4/0.25/1).
float4 ps_bloom_dof_tap4_minusthreshold(VsOut i) : SV_Target {
  float2 uv = GameUv(i);
  float3 a = t0.SampleLevel(s_lin, uv + c[0].xy, 0).xyz;
  float3 b = t0.SampleLevel(s_lin, uv + c[1].xy, 0).xyz;
  float3 d = t0.SampleLevel(s_lin, uv + c[2].xy, 0).xyz;
  float3 e = t0.SampleLevel(s_lin, uv + c[3].xy, 0).xyz;
  float3 s = e * e;
  s = d * d + s;
  s = b * b + s;
  s = a * a + s;
  float3 r0 = s * 0.25 - c[4].w;
  float3 r1 = saturate(s * 0.25);
  float4 o;
  o.w = saturate(r1.z * 0.3 + r1.x * 0.3 + r1.y * 0.4);
  float4 r = float4(saturate(r0 * c[4].z) + c[4].x, 1.0);
  o.x = saturate(dot(c[5], r));
  o.y = saturate(dot(c[6], r));
  o.z = saturate(dot(c[7], r));
  return o;
}

// bloom_dof_tap4: 4-tap box downsample (literal c255.x = 0.25), alpha 1.
float4 ps_bloom_dof_tap4(VsOut i) : SV_Target {
  float2 uv = GameUv(i);
  float3 a = t0.SampleLevel(s_lin, uv + c[0].xy, 0).xyz;
  float3 b = t0.SampleLevel(s_lin, uv + c[1].xy, 0).xyz;
  float3 d = t0.SampleLevel(s_lin, uv + c[2].xy, 0).xyz;
  float3 e = t0.SampleLevel(s_lin, uv + c[3].xy, 0).xyz;
  return float4((((a + b) + d) + e) * 0.25, 1.0);
}

// bloom_dof_tap9PS: 9 taps along the direction c1.xy (radial zoom blend by
// c1.w), tap k at offset c[k].x with weight c[k].z (k = 2..10), scaled by
// the level gain c0.w * (1 + c1.w). Literals c255 = (-0.75, 1, -2.5, 5).
float4 ps_bloom_dof_tap9(VsOut i) : SV_Target {
  float2 uv = GameUv(i);
  float2 d = (uv * 5.0 + (-2.5 - c[1].xy)) * c[1].w + c[1].xy;
  float2 p = d * -0.75 * c[6].x * c[1].w + uv;
  float4 acc = t0.SampleLevel(s_lin, p + d * c[9].x, 0) * c[9].z;
  acc = t0.SampleLevel(s_lin, p + d * c[8].x, 0) * c[8].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[7].x, 0) * c[7].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[6].x, 0) * c[6].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[5].x, 0) * c[5].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[4].x, 0) * c[4].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[3].x, 0) * c[3].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[2].x, 0) * c[2].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[10].x, 0) * c[10].z + acc;
  return float4(acc.xyz * (c[0].w * (c[1].w + 1.0)), acc.w);
}

// bloom_dof_tap9_combine3PS: the tap9 blur of t0 scaled by
// c0.x * (1 + c1.w) plus t1 (the next level) * c0.y; alpha = a radial
// mask 1 - sat(5 * |sat(|2uv - 1| - 0.6)| - 1) (literals c254 = (0, -2.5,
// -0.6, 0), c255 = (-0.75, 1, 5, -1)).
float4 ps_bloom_dof_tap9_combine3(VsOut i) : SV_Target {
  float2 uv = GameUv(i);
  float2 d = (uv * 5.0 + (-2.5 - c[1].xy)) * c[1].w + c[1].xy;
  float2 p = d * -0.75 * c[6].x * c[1].w + uv;
  float3 acc = t0.SampleLevel(s_lin, p + d * c[10].x, 0).xyz * c[10].z;
  acc = t0.SampleLevel(s_lin, p + d * c[9].x, 0).xyz * c[9].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[8].x, 0).xyz * c[8].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[7].x, 0).xyz * c[7].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[6].x, 0).xyz * c[6].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[5].x, 0).xyz * c[5].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[4].x, 0).xyz * c[4].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[3].x, 0).xyz * c[3].z + acc;
  acc = t0.SampleLevel(s_lin, p + d * c[2].x, 0).xyz * c[2].z + acc;
  float3 nxt = t1.SampleLevel(s_lin, uv, 0).xyz;
  float2 m = saturate(abs(uv * 2.0 - 1.0) - 0.6);
  float rad = sqrt(abs(dot(m, m)));
  float4 o;
  o.xyz = saturate(acc * ((c[1].w + 1.0) * c[0].x) + nxt * c[0].y);
  o.w = 1.0 - saturate(rad * 5.0 - 1.0);
  return o;
}

// bloom_dof_bloom_ghostPS (the lens flare): six ghosts sampled at
// (uv - 0.5) * scale + 0.5 (scales c0.xy/zw, c1.xy/zw, c2.xy/zw), each
// = luma(tap) * tap.a * its colour row (c3..c8), saturated and added onto
// the centre tap. Literals c255 = (0.5, 0.33).
float4 ps_bloom_dof_bloom_ghost(VsOut i) : SV_Target {
  float2 uv = GameUv(i);
  float2 o = uv - 0.5;
  float4 g3 = t0.SampleLevel(s_lin, o * c[0].xy + 0.5, 0);
  float4 g4 = t0.SampleLevel(s_lin, o * c[0].zw + 0.5, 0);
  float4 g5 = t0.SampleLevel(s_lin, o * c[1].xy + 0.5, 0);
  float4 g6 = t0.SampleLevel(s_lin, o * c[1].zw + 0.5, 0);
  float4 g7 = t0.SampleLevel(s_lin, o * c[2].xy + 0.5, 0);
  float4 g8 = t0.SampleLevel(s_lin, o * c[2].zw + 0.5, 0);
  float4 base = t0.SampleLevel(s_lin, uv, 0);
  const float3 k = float3(0.33, 0.33, 0.33);
  float4 acc = dot(g7.xyz, k) * c[7] * g7.w;
  acc = dot(g6.xyz, k) * c[6] * g6.w + acc;
  acc = dot(g5.xyz, k) * c[5] * g5.w + acc;
  acc = dot(g4.xyz, k) * c[4] * g4.w + acc;
  acc = dot(g3.xyz, k) * c[3] * g3.w + acc;
  acc = saturate(dot(g8.xyz, k) * c[8] * g8.w + acc);
  return acc + base;
}

// postfx_fast_uberPS / the postfx_visualfxPS front: the game's final scene
// composite, scene * 1.41 + bloom (literal c254.x = 1.41), written to an
// 8-bit target (saturate). c[0].x = 1 when the game ran its bloom chain
// this frame (t1 = its result), else the bloom term is absent as on the
// console. The tint-LUT blend (c0.x of fast_uber) is 0 in every capture.
float4 ps_postfx_fast_uber(VsOut i) : SV_Target {
  float4 s = t0.SampleLevel(s_pt, i.r0.xy, 0);
  float3 b = c[0].x > 0.5 ? t1.SampleLevel(s_lin, i.r0.xy, 0).xyz : 0.0;
  return float4(saturate(s.xyz * 1.41 + b), s.w);
}
