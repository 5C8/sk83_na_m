#pragma once

#include <cstdint>

// Wide-output factor the APT HUD is spread by: output aspect / (16/9) while
// skate3_apt_spread is on and the native renderer serves a wider-than-16:9
// output, else 1. The renderer widens the APT canvas clip by the same factor.
float Skate3AptSpreadFactor();

// Called from the renderer's hooks: AptRenderingIntegration::Render2D
// bracket, and each cFont::DrawstringLocal before it runs.
void Skate3AptSpreadRender2D(bool enter);
struct PPCContext;
void Skate3AptSpreadDrawstring(PPCContext& ctx, uint8_t* base);
