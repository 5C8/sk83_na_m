#pragma once

#include <cstdint>

namespace rex::runtime {
class FunctionDispatcher;
}

// Full bypass of the Xenia-derived GPU stack: no IGraphicsSystem, no
// CommandProcessor, no RegisterFile, no ring buffer / PM4. Every
// D3DDevice_* the guest calls is intercepted here and translated directly
// into real D3D11 calls against our own device/swapchain.
//
// Whatever RuntimeConfig your app builds should either leave `.graphics`
// unset or use tool_mode, so the SDK's own GPU backend never spins up
// alongside this one -- that's an app-setup config choice, not a rexglue
// source change.
void Skate3InitPcRenderer();

// Call once, between Runtime::PrepareModuleLaunch() and thread->Resume() --
// per runtime.h: "creates suspended main thread without resuming. Call
// thread->Resume() after any pre-launch hooks." That's the intended slot
// for exactly this: overriding function-table entries via
// FunctionDispatcher::SetFunction for guest addresses that already have a
// real recompiled body (game-specific code, unlike REX_HOOK_RAW targets
// such as rex_DebugBreak_82DAD568, which are recognized CRT thunks the codegen
// deliberately omits a body for). Example call site:
//
//   auto thread = runtime->PrepareModuleLaunch();
//   Skate3RegisterPcRendererHooks(runtime->function_dispatcher());
//   thread->Resume();
void Skate3RegisterPcRendererHooks(rex::runtime::FunctionDispatcher* dispatcher);
