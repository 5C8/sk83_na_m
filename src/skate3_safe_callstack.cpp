// Safety hook for Sk8::Callstack::GetCallstack (guest 0x82D09EE8).
//
// This function walks the guest PPC "back chain" (the linked list of saved
// frame pointers stored at offset 0 of each stack frame) to build a
// backtrace whenever an assertion fires (FailAssert -> AssertCallback ->
// PrepareCallstack -> GetCallstack). The generated code validates each
// walked pointer with heuristics -- 16-byte alignment and "must be higher
// than the previous frame" -- but neither of those actually proves the
// address is backed by real, committed memory. TranslateVirtual() (see
// system/xmemory.h) is just `virtual_membase_ + guest_address`; it never
// returns null, so a corrupted or unusually-laid-out back-chain that
// happens to pass the alignment/ordering heuristics still gets dereferenced
// directly via REX_LOAD_U32, with no safety net -- producing exactly the
// access violation seen in loc_82D09F50's `lwz r31,0(r31)`.
//
// Fix: same shape as the rest of the generated function (preserved
// verbatim, including labels/gotos), but every walked-pointer dereference
// is preceded by an actual page-commit check (VirtualQuery, same tool
// skate3_dynamic_aspect.cpp's PageCommitted() already uses) and falls
// through to the existing "done" exit path instead of crashing if the
// address isn't safely readable.

#include "generated/sk83_na_m_init.h"

#include <rex/hook.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace {

// Is `addr`, translated to a host pointer, safe to read `size` bytes from?
// Guest addresses always translate to *some* host pointer (TranslateVirtual
// never returns null), so this is the only real validity check available --
// mirrors skate3_dynamic_aspect.cpp's PageCommitted().
bool GuestAddressReadable(uint8_t* base, uint32_t addr, size_t size) {
  if (addr == 0) return false;
  void* host = base + addr;
  MEMORY_BASIC_INFORMATION mbi{};
  if (!VirtualQuery(host, &mbi, sizeof(mbi))) return false;
  if (mbi.State != MEM_COMMIT) return false;
  if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return false;
  // Make sure the whole read (not just the first byte) lands in this region.
  const uintptr_t region_end =
      reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
  return reinterpret_cast<uintptr_t>(host) + size <= region_end;
}

}  // namespace

extern "C" REX_FUNC(rex_EA_Callstack_GetInstructionPointer_YAXAAPAX_Z_82D09E98);
extern "C" REX_FUNC(rex_savegprlr_27_83190C14);
extern "C" REX_FUNC(rex_restgprlr_27_83190C64);

extern "C" void rex_EA_Callstack_GetCallstack_YAIQAPAXIPBUCallstackContext_12_Z_82D09EE8(
    PPCContext& ctx, uint8_t* base) {
  REX_FUNC_PROLOGUE();
  uint32_t ea{};
  // mflr r12
  ctx.r12.u64 = ctx.lr;
  // bl 0x83190c14
  ctx.lr = 0x82D09EF0;
  rex_savegprlr_27_83190C14(ctx, base);
  // stwu r1,-144(r1)
  ea = -144 + ctx.r1.u32;
  REX_STORE_U32(ea, ctx.r1.u32);
  ctx.r1.u32 = ea;
  // mr r28,r5
  ctx.r28.u64 = ctx.r5.u64;
  // mr r27,r3
  ctx.r27.u64 = ctx.r3.u64;
  // mr r29,r4
  ctx.r29.u64 = ctx.r4.u64;
  // li r30,0
  ctx.r30.s64 = 0;
  // cmplwi cr6,r28,0
  ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
  // beq cr6,0x82d09f18
  if (ctx.cr6.eq) goto loc_82D09F18;
  // lwz r31,0(r28)  -- guarded: r28 is caller-supplied, may be stale
  if (!GuestAddressReadable(base, ctx.r28.u32 + 0, 4)) goto loc_82D09F84;
  ctx.r31.u64 = REX_LOAD_U32(ctx.r28.u32 + 0);
  // lwz r11,4(r28)
  if (!GuestAddressReadable(base, ctx.r28.u32 + 4, 4)) goto loc_82D09F84;
  ctx.r11.u64 = REX_LOAD_U32(ctx.r28.u32 + 4);
  // b 0x82d09f28
  goto loc_82D09F28;
loc_82D09F18:
  // addi r3,r1,80
  ctx.r3.s64 = ctx.r1.s64 + 80;
  // rotlwi r31,r1,0
  ctx.r31.u64 = ctx.r1.u32;
  // bl 0x82d09e98
  ctx.lr = 0x82D09F24;
  rex_EA_Callstack_GetInstructionPointer_YAXAAPAX_Z_82D09E98(ctx, base);
  // lwz r11,80(r1) -- our own stack, always safe
  ctx.r11.u64 = REX_LOAD_U32(ctx.r1.u32 + 80);
loc_82D09F28:
  // addic. r9,r29,-1
  ctx.xer.ca = ctx.r29.u32 > 0;
  ctx.r9.s64 = ctx.r29.s64 + -1;
  ctx.cr0.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
  // li r10,0
  ctx.r10.s64 = 0;
  // beq 0x82d09f84
  if (ctx.cr0.eq) goto loc_82D09F84;
  // addi r8,r27,-4
  ctx.r8.s64 = ctx.r27.s64 + -4;
loc_82D09F38:
  // cmplwi cr6,r28,0
  ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
  // bne cr6,0x82d09f48
  if (!ctx.cr6.eq) goto loc_82D09F48;
  // cmpwi cr6,r10,0
  ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
  // ble cr6,0x82d09f50
  if (!ctx.cr6.gt) goto loc_82D09F50;
loc_82D09F48:
  // stwu r11,4(r8)
  ea = 4 + ctx.r8.u32;
  REX_STORE_U32(ea, ctx.r11.u32);
  ctx.r8.u32 = ea;
  // addi r30,r30,1
  ctx.r30.s64 = ctx.r30.s64 + 1;
loc_82D09F50:
  // mr r11,r31
  ctx.r11.u64 = ctx.r31.u64;
  // lwz r31,0(r31)  -- THE CRASH SITE: guarded, bail to done instead of AV
  if (!GuestAddressReadable(base, ctx.r31.u32 + 0, 4)) goto loc_82D09F84;
  ctx.r31.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
  // cmplwi cr6,r31,0
  ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
  // beq cr6,0x82d09f84
  if (ctx.cr6.eq) goto loc_82D09F84;
  // clrlwi r7,r31,28
  ctx.r7.u64 = ctx.r31.u32 & 0xF;
  // cmplwi cr6,r7,0
  ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
  // bne cr6,0x82d09f84
  if (!ctx.cr6.eq) goto loc_82D09F84;
  // cmplw cr6,r31,r11
  ctx.cr6.compare<uint32_t>(ctx.r31.u32, ctx.r11.u32, ctx.xer);
  // blt cr6,0x82d09f84
  if (ctx.cr6.lt) goto loc_82D09F84;
  // lwz r11,-8(r31)  -- guarded: passes alignment/ordering but may still
  // land outside committed memory (this is the gap that let a bogus but
  // "plausible-looking" frame pointer through in the original code)
  if (!GuestAddressReadable(base, ctx.r31.u32 - 8, 4)) goto loc_82D09F84;
  ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + -8);
  // addi r10,r10,1
  ctx.r10.s64 = ctx.r10.s64 + 1;
  // cmplw cr6,r30,r9
  ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r9.u32, ctx.xer);
  // blt cr6,0x82d09f38
  if (ctx.cr6.lt) goto loc_82D09F38;
loc_82D09F84:
  // rlwinm r11,r30,2,0,29
  ctx.r11.u64 = (ctx.r30.u32 << 2) & 0xFFFFFFFCu;
  // li r10,0
  ctx.r10.s64 = 0;
  // mr r3,r30
  ctx.r3.u64 = ctx.r30.u64;
  // stwx r10,r11,r27
  REX_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
  // addi r1,r1,144
  ctx.r1.s64 = ctx.r1.s64 + 144;
  // b 0x83190c64
  rex_restgprlr_27_83190C64(ctx, base);
  return;
}
