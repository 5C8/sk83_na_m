// DIAGNOSTIC: trick guide exit assert (assetrefcounter.h:41).
//
// Leaving the trick guide (FrontEndState_TrickGuide::OnLeave, 0x82760120)
// fails the assert in assman::cAssetRefCounter::RemAsset (0x8289E390) on the
// presentation thread: PresThreadMsgMan::ReceiveDRegArenaMsg (0x8289E578)
// deregisters an asset ID the ref counter does not hold (a double
// deregistration, or one never registered). Original hardware asserts too,
// so this is the build's own logic. Retail's OnLeave differs: it only
// unloads the loose world when its OnEnter loaded one (+340) and only
// disables the NIS trick guide when it is enabled (NISManager +605); this
// build does both unconditionally.
//
// This file mirrors the ref counter (AddAsset / RemAsset per counter + ID),
// keeps a ring of the recent register / deregister events together with the
// trick guide exit steps, and when a RemAsset is about to fail logs that
// ID's history, so the first deregistration's origin is on record. Not
// compiled into Release.

#include "generated/sk83_na_m_init.h"

#include <rex/hook.h>
#include <rex/logging.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <stdlib.h>

namespace {

uint32_t LoadU32(uint8_t* base, uint32_t addr) {
  uint32_t v;
  std::memcpy(&v, REX_RAW_ADDR(addr), 4);
  return _byteswap_ulong(v);
}
uint64_t LoadU64(uint8_t* base, uint32_t addr) {
  uint64_t v;
  std::memcpy(&v, REX_RAW_ADDR(addr), 8);
  return _byteswap_uint64(v);
}
uint8_t LoadU8(uint8_t* base, uint32_t addr) { return *REX_RAW_ADDR(addr); }

enum EventKind : uint8_t {
  kAddAsset,
  kRemAsset,
  kPresRegArena,
  kPresDRegArena,
  kSimRegArena,
  kSimDRegArena,
  kEnableTrickGuide,
  kTrickGuideLeave,
};
const char* KindName(uint8_t k) {
  switch (k) {
    case kAddAsset: return "AddAsset";
    case kRemAsset: return "RemAsset";
    case kPresRegArena: return "PresRegArena";
    case kPresDRegArena: return "PresDRegArena";
    case kSimRegArena: return "SimRegArena";
    case kSimDRegArena: return "SimDRegArena";
    case kEnableTrickGuide: return "EnableTrickGuide";
    case kTrickGuideLeave: return "TrickGuide::OnLeave";
    default: return "?";
  }
}

struct Event {
  int64_t t_us;
  uint64_t id;
  uint32_t counter;
  uint32_t lr;
  uint32_t aux;
  uint32_t tid;
  uint8_t kind;
};

std::mutex g_mutex;
std::array<Event, 512> g_ring{};
uint32_t g_ring_pos = 0;
// (counter << 64 | id) mirrored as two keys: counter object, asset ID.
struct KeyHash {
  size_t operator()(const std::pair<uint32_t, uint64_t>& k) const {
    return std::hash<uint64_t>()(k.second * 0x9E3779B97F4A7C15ull ^ k.first);
  }
};
std::unordered_map<std::pair<uint32_t, uint64_t>, int32_t, KeyHash> g_count;

int64_t NowUs() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

uint32_t ThreadTag() {
  return uint32_t(std::hash<std::thread::id>()(std::this_thread::get_id()) & 0xFFFFu);
}

void Record(uint8_t kind, uint64_t id, uint32_t counter, uint32_t lr, uint32_t aux) {
  g_ring[g_ring_pos++ & 511u] = {NowUs(), id, counter, lr, aux, ThreadTag(), kind};
}

void DumpHistory(uint64_t id, const char* why) {
  const int64_t now = NowUs();
  REXLOG_ERROR("trickguide-diag: {} asset id {:016X} - ring history (newest last):", why, id);
  for (uint32_t i = 0; i < 512; ++i) {
    const Event& e = g_ring[(g_ring_pos + i) & 511u];
    if (e.t_us == 0) {
      continue;
    }
    const bool related = e.id == id || e.kind == kEnableTrickGuide || e.kind == kTrickGuideLeave;
    // The last 40 events of any kind, plus every event for this ID.
    if (!related && i < 512 - 40) {
      continue;
    }
    REXLOG_ERROR(
        "trickguide-diag:   {:>8}us ago {:<18} id={:016X} counter={:08X} aux={:08X} "
        "lr={:08X} thread={:04X}{}",
        now - e.t_us, KindName(e.kind), e.id, e.counter, e.aux, e.lr, e.tid,
        e.id == id ? "  <==" : "");
  }
}

}  // namespace

// assman::cAssetRefCounter::AddAsset(this, cAssetID): r3 = counter, r4 = ID.
extern "C" REX_FUNC(rex_f0IAA_0BAAA_cAssetRefCounter_AddAsset_assman_QAAHVcAssetID_2_Z_8289F400) {
  const uint32_t counter = ctx.r3.u32;
  const uint64_t id = ctx.r4.u64;
  const uint32_t lr = uint32_t(ctx.lr);
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kAddAsset, id, counter, lr, 0);
    ++g_count[{counter, id}];
  }
  __imp__rex_f0IAA_0BAAA_cAssetRefCounter_AddAsset_assman_QAAHVcAssetID_2_Z_8289F400(ctx, base);
}

// assman::cAssetRefCounter::RemAsset(this, cAssetID): asserts (line 41) when
// the ID is not in the counter.
extern "C" REX_FUNC(rex_f0CAA_0EAA_cAssetRefCounter_RemAsset_assman_QAAHVcAssetID_2_Z_8289E390) {
  const uint32_t counter = ctx.r3.u32;
  const uint64_t id = ctx.r4.u64;
  const uint32_t lr = uint32_t(ctx.lr);
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kRemAsset, id, counter, lr, 0);
    auto it = g_count.find({counter, id});
    if (it == g_count.end() || it->second <= 0) {
      DumpHistory(id, it == g_count.end() ? "RemAsset of a NEVER-counted" : "RemAsset of an "
                                                                            "already-released");
    } else if (--it->second == 0) {
      g_count.erase(it);
    }
  }
  __imp__rex_f0CAA_0EAA_cAssetRefCounter_RemAsset_assman_QAAHVcAssetID_2_Z_8289E390(ctx, base);
}

// Arena register / deregister messages (asset ID at msg + 16).
extern "C" REX_FUNC(rex_Sk8_PresThreadMsgMan_ReceiveRegArenaMsg_AAAXABVcMsgAssetPresRegArena_Z_8289F5A0) {
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kPresRegArena, LoadU64(base, ctx.r4.u32 + 16), 0, uint32_t(ctx.lr),
           LoadU32(base, ctx.r4.u32 + 24));
  }
  __imp__rex_Sk8_PresThreadMsgMan_ReceiveRegArenaMsg_AAAXABVcMsgAssetPresRegArena_Z_8289F5A0(ctx,
                                                                                               base);
}

extern "C" REX_FUNC(rex_Sk8_PresThreadMsgMan_ReceiveDRegArenaMsg_AAAXABVcMsgAssetPresDRegArena_Z_8289E578) {
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kPresDRegArena, LoadU64(base, ctx.r4.u32 + 16), 0, uint32_t(ctx.lr),
           LoadU32(base, ctx.r4.u32 + 24));
  }
  __imp__rex_Sk8_PresThreadMsgMan_ReceiveDRegArenaMsg_AAAXABVcMsgAssetPresDRegArena_Z_8289E578(
      ctx, base);
}

extern "C" REX_FUNC(rex_Sk8_SimThreadMsgMan_ReceiveRegArenaMsg_AAAXABVcMsgAssetSimRegArena_Z_8289CDA8) {
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kSimRegArena, LoadU64(base, ctx.r4.u32 + 16), 0, uint32_t(ctx.lr),
           LoadU32(base, ctx.r4.u32 + 24));
  }
  __imp__rex_Sk8_SimThreadMsgMan_ReceiveRegArenaMsg_AAAXABVcMsgAssetSimRegArena_Z_8289CDA8(ctx,
                                                                                             base);
}

extern "C" REX_FUNC(rex_Sk8_SimThreadMsgMan_ReceiveDRegArenaMsg_AAAXABVcMsgAssetSimDRegArena_Z_8289CEA0) {
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kSimDRegArena, LoadU64(base, ctx.r4.u32 + 16), 0, uint32_t(ctx.lr),
           LoadU32(base, ctx.r4.u32 + 24));
  }
  __imp__rex_Sk8_SimThreadMsgMan_ReceiveDRegArenaMsg_AAAXABVcMsgAssetSimDRegArena_Z_8289CEA0(ctx,
                                                                                               base);
}

// NISManager::EnableTrickGuide(this, enable, arg): aux = enable | arg << 8 |
// the enable flag (+589) BEFORE the call << 16.
extern "C" REX_FUNC(rex_Sk8_NISManager_EnableTrickGuide_QAAX_N0_Z_829F37C0) {
  const uint32_t flag = LoadU8(base, ctx.r3.u32 + 589);
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kEnableTrickGuide, 0, ctx.r3.u32, uint32_t(ctx.lr),
           (ctx.r4.u32 & 0xFFu) | ((ctx.r5.u32 & 0xFFu) << 8) | (flag << 16));
  }
  REXLOG_INFO("trickguide-diag: NISManager::EnableTrickGuide(this={:08X}, enable={}, {}) "
              "flag(+589) before={} lr={:08X}",
              ctx.r3.u32, ctx.r4.u32 & 0xFFu, ctx.r5.u32 & 0xFFu, flag, uint32_t(ctx.lr));
  __imp__rex_Sk8_NISManager_EnableTrickGuide_QAAX_N0_Z_829F37C0(ctx, base);
}

// FrontEndState_TrickGuide::OnLeave: aux = the state object's +60 word.
extern "C" REX_FUNC(rex_Sk8_FE_FrontEndState_TrickGuide_OnLeave_UAAXXZ_82760120) {
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    Record(kTrickGuideLeave, 0, ctx.r3.u32, uint32_t(ctx.lr), LoadU32(base, ctx.r3.u32 + 60));
  }
  REXLOG_INFO("trickguide-diag: TrickGuide::OnLeave enter (this={:08X} +60={:08X})", ctx.r3.u32,
              LoadU32(base, ctx.r3.u32 + 60));
  __imp__rex_Sk8_FE_FrontEndState_TrickGuide_OnLeave_UAAXXZ_82760120(ctx, base);
  REXLOG_INFO("trickguide-diag: TrickGuide::OnLeave exit");
}
