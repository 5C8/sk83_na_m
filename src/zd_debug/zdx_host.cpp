// zdx runtime: address map + pool, zd-only cvars, and the host side of zd's
// DebugOverlay (see zdx.h).

#include "zdx.h"
#include "zdx_generated.h"
#include "zd_debug.h"
#include "overlay.h"

#include <array>
#include <atomic>
#include <ctime>
#include <bit>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>


#include <Windows.h>

#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/system/kernel_state.h>
#include <rex/system/function_dispatcher.h>
#include <rex/system/xmemory.h>

REXCVAR_DEFINE_INT32(zdx_draw_type_mask, -1, "Skate 3 zd debug",
                     "Bit per DebugOverlay shape type to draw (diagnostics; -1 = all)");
REXCVAR_DEFINE_BOOL(zdx_dump_shapes, false, "Skate 3 zd debug",
                    "Log the DebugOverlay shape queue periodically (diagnostics)");
REXCVAR_DEFINE_BOOL(zdx_trace_blocks, false, "Skate 3 zd debug",
                    "Log the first run of each ported zd debug block (diagnostics)");
REXCVAR_DEFINE_BOOL(zdx_crash_probe, false, "Skate 3 zd debug",
                    "Log the zd call site of access violations inside transplanted zd code "
                    "(diagnostics; also reports the emulator's handled memory-watch faults)");

namespace zdx {
// TheCVarMan (m): cVarRef::Link(ref, cVarMan) needs it in r4, as the
// cVarBoolRef/... constructors pass it (m 82D8A478).
constexpr uint32_t kTheCVarMan = 0x83360D20u;
namespace {

std::once_flag g_init_once;
uint32_t g_pool = 0;       // guest address of the pool copy
uint32_t g_scratch = 0;    // 4 KB zero block for unmapped addresses

uint32_t GuestAlloc(uint32_t size, uint32_t align) {
  auto* ks = rex::system::kernel_state();
  return ks && ks->memory() ? ks->memory()->SystemHeapAlloc(size, align) : 0;
}

const MapEntry* FindSlow(uint32_t a) {
  // Exact m mappings (cvar objects, globals) take precedence over pool pages
  // that happen to contain them.
  const MapEntry* pool = nullptr;
  for (size_t i = 0; i < kMapEntryCount; ++i) {
    const MapEntry& e = kMapEntries[i];
    if (a >= e.zd_lo && a < e.zd_hi) {
      if (e.kind == kM) return &e;
      pool = &e;
    }
  }
  return pool;
}

// Every zd global access goes through Find; the linear scan over ~600 entries
// made heavy debug draws (kShowLocations: 3D text for every location) crawl
// so much that loading never finished. The map is constant, so a per-thread
// direct-mapped cache in front of the scan is exact.
const MapEntry* Find(uint32_t a) {
  constexpr uint32_t kSlots = 4096;
  struct Cache {
    uint32_t key[kSlots];
    const MapEntry* val[kSlots];
    Cache() { std::fill(std::begin(key), std::end(key), 0xFFFFFFFFu); }
  };
  thread_local Cache cache;
  const uint32_t slot = (a >> 2) & (kSlots - 1);
  if (cache.key[slot] == a) return cache.val[slot];
  const MapEntry* e = FindSlow(a);
  cache.key[slot] = a;
  cache.val[slot] = e;
  return e;
}

uint32_t Resolve(uint32_t a) {
  const MapEntry* e = Find(a);
  if (!e) return 0;
  return e->kind == kM ? e->value + (a - e->zd_lo) : g_pool + e->value + (a - e->zd_lo);
}

void Init(uint8_t* base) {
  g_pool = GuestAlloc(uint32_t(kPoolSize), 0x1000);
  g_scratch = GuestAlloc(0x1000, 0x1000);
  if (!g_pool || !g_scratch) {
    REXLOG_ERROR("zdx: pool allocation failed");
    return;
  }
  std::memcpy(base + g_pool, kPoolInit, kPoolSize);
  std::memset(base + g_scratch, 0, 0x1000);
  for (const PoolFixup* f = kPoolFixups; f->zd_addr; ++f) {
    const uint32_t slot = Resolve(f->zd_addr);
    if (!slot) continue;
    uint32_t v = f->poke ? f->value : REX_LOAD_U32(slot);
    if (f->reloc && v) v = Resolve(v);
    REX_STORE_U32(slot, v);
  }
  // zd vtables the transplanted code installs: point their entries at m's
  // functions, or at thunks that dispatch to the transplanted zd functions.
  auto* ks = rex::system::kernel_state();
  auto* fd = ks ? ks->function_dispatcher() : nullptr;
  for (const VtableFixup* v = kVtableFixups; v->zd_slot; ++v) {
    uint32_t target = v->m_addr;
    if (!target && v->host && fd) target = fd->AllocateThunk(v->host, 0);
    REX_STORE_U32(Resolve(v->zd_slot), target);
  }
  REXLOG_INFO("zdx: pool {} KB at 0x{:08X}, {} map entries", kPoolSize / 1024, g_pool,
              kMapEntryCount);
}

uint8_t* g_base = nullptr;

void EnsureInit(uint8_t* base) {
  std::call_once(g_init_once, [base] {
    g_base = base;
    Init(base);
  });
}

}  // namespace

uint32_t AllocZeroed(uint8_t* base, uint32_t size, uint32_t align) {
  const uint32_t a = GuestAlloc(size, align);
  if (a) std::memset(REX_RAW_ADDR(a), 0, size);
  return a;
}

uint32_t Map(uint32_t zd_addr) {
  const uint32_t a = Resolve(zd_addr);
  if (a && (g_pool || Find(zd_addr)->kind == kM)) return a;
  static std::atomic<int> warned{0};
  if (warned.fetch_add(1) < 20) REXLOG_WARN("zdx: unmapped zd address 0x{:08X}", zd_addr);
  return g_scratch;
}

uint32_t Overlay() { return Map(0x84904800u); }

// zd's PrintLogFile root folder ("d:\logs"; game\logs on the host): a guest
// copy of the string, made once (zdx_config PATCH on OpenLogFile's root loads).
uint32_t LogRoot() {
  static uint32_t s = 0;
  if (!s) {
    static const char kRoot[] = "d:\\logs";
    s = GuestAlloc(sizeof(kRoot), 16);
    if (s && g_base) std::memcpy(g_base + s, kRoot, sizeof(kRoot));
  }
  return s ? s : g_scratch;
}

uint32_t ExtSlow(uint32_t obj, uint32_t id);

uint32_t Ext(uint32_t obj, uint32_t id) {
  // blocks are never freed: a per-thread cache in front of the locked map
  constexpr uint32_t kSlots = 1024;
  thread_local uint64_t ckey[kSlots] = {};
  thread_local uint32_t cval[kSlots] = {};
  const uint64_t key = (uint64_t(id + 1) << 32) | obj;   // id + 1: key 0 = empty slot
  const uint32_t slot = uint32_t((obj >> 4) ^ (id * 0x9E37u)) & (kSlots - 1);
  if (ckey[slot] == key) return cval[slot];
  const uint32_t r = ExtSlow(obj, id);
  ckey[slot] = key;
  cval[slot] = r;
  return r;
}

uint32_t ExtSlow(uint32_t obj, uint32_t id) {
  static std::mutex mu;
  static std::unordered_map<uint64_t, uint32_t> blocks;
  std::lock_guard<std::mutex> lock(mu);
  uint32_t& b = blocks[(uint64_t(id) << 32) | obj];
  if (!b) {
    b = GuestAlloc(kExtSizes[id], 16);
    if (b && g_base) std::memset(g_base + b, 0, kExtSizes[id]);
    if (!b) b = g_scratch;
  }
  return b;
}

uint32_t CvarObject(const char* name) {
  for (const GameCvarName* c = kGameCvars; c->name; ++c) {
    if (std::strcmp(c->name, name) == 0) return c->zd_only ? Map(c->obj) : c->obj;
  }
  return 0;
}

bool OptionOn(uint8_t* base, uint32_t zd_obj, uint32_t var_off, uint32_t reg) {
  const uint32_t obj = Map(zd_obj);
  const uint32_t var = REX_LOAD_U32(obj + var_off);
  if (!var) return false;
  switch (reg) {
    case 0x83D91738u: return REX_LOAD_U32(var + 208) != 0;           // enum
    case 0x83D915F0u:
    case 0x83D91680u: return (REX_LOAD_U32(var + 216) & 0x7FFFFFFFu) != 0;  // int / float
    default: {                                                         // bool
      const uint32_t ovr = REX_LOAD_U32(var + 208);
      return (ovr ? REX_LOAD_U8(ovr) : REX_LOAD_U8(var + 205)) != 0;
    }
  }
}

uint32_t CvarVarOffset(const char* name) {
  for (const GameCvarName* c = kGameCvars; c->name; ++c) {
    if (std::strcmp(c->name, name) == 0) return c->var_off;
  }
  return 0;
}

void VirtualCall(PPCContext& ctx, uint8_t* base, uint32_t vtable, int32_t zd_off) {
  const int32_t slot = zd_off / 4;
  // Sorted by m_vtable (generator order).
  size_t lo = 0, hi = kVtMapCount;
  while (lo < hi) {
    const size_t mid = (lo + hi) / 2;
    if (kVtMaps[mid].m_vtable < vtable) lo = mid + 1; else hi = mid;
  }
  uint32_t target = 0;
  if (lo < kVtMapCount && kVtMaps[lo].m_vtable == vtable && slot >= 0 && uint32_t(slot) < kVtMaps[lo].count) {
    const int16_t ms = kVtMaps[lo].slots[slot];
    if (ms >= 0) {
      target = REX_LOAD_U32(vtable + uint32_t(ms) * 4);
    } else {
      // m dropped this method: run zd's own (if transplanted), else skip.
      const uint32_t zd_fn = kVtMaps[lo].zd_fns[slot];
      static std::atomic<int> warned{0};
      for (const Transplanted* t = kTransplanted; t->fn; ++t) {
        if (t->zd_addr == zd_fn) {
          t->fn(ctx, base);
          return;
        }
      }
      if (warned.fetch_add(1) < 20)
        REXLOG_WARN("zdx: virtual slot {} of zd vtable 0x{:08X} has no m method and no transplant",
                    slot, kVtMaps[lo].zd_vtable);
      ctx.r3.u64 = 0;
      return;
    }
  } else {
    target = REX_LOAD_U32(vtable + uint32_t(zd_off));  // same layout (or a zd pool vtable)
    // Unpaired m vtable: zd's slot offset is used as is. Report each once.
    static std::mutex mu;
    static std::unordered_set<uint32_t> seen;
    std::lock_guard<std::mutex> lock(mu);
    if (vtable >= 0x82000000u && vtable < 0x82480000u && seen.insert(vtable).second)
      REXLOG_WARN("zdx: virtual call through unmapped m vtable 0x{:08X} slot +{} (zd site lr 0x{:08X})",
                  vtable, zd_off, uint32_t(ctx.lr));
  }
  if (!target) {
    static std::atomic<int> n{0};
    if (n.fetch_add(1) < 10)
      REXLOG_ERROR("zdx: virtual call to null: vtable 0x{:08X} zd slot +{} (lr 0x{:08X})", vtable, zd_off,
                   uint32_t(ctx.lr));
    ctx.r3.u64 = 0;
    return;
  }
  ctx.ctr.u64 = target;
  REX_CALL_INDIRECT_FUNC(target);
}

void IndirectCall(PPCContext& ctx, uint8_t* base, uint32_t zd_fn) {
  const uint32_t target = ctx.ctr.u32;
  if (!target) {
    static std::atomic<int> n{0};
    if (n.fetch_add(1) < 10)
      REXLOG_ERROR("zdx: indirect call to null in zd 0x{:08X} (lr 0x{:08X})", zd_fn, uint32_t(ctx.lr));
    ctx.r3.u64 = 0;
    return;
  }
  REX_CALL_INDIRECT_FUNC(target);
}

uint32_t FnThunk(GuestFn fn) {
  static std::mutex mu;
  static std::unordered_map<GuestFn, uint32_t> thunks;
  std::lock_guard<std::mutex> lock(mu);
  uint32_t& t = thunks[fn];
  if (!t) {
    auto* ks = rex::system::kernel_state();
    auto* fd = ks ? ks->function_dispatcher() : nullptr;
    if (fd) t = fd->AllocateThunk(fn, 0);
  }
  return t;
}

// ---- crash attribution: the transplanted code keeps ctx.lr = the zd call
// site of its latest call, so a fault inside it can be pinned to zd code.
namespace {
thread_local PPCContext* t_active = nullptr;
LONG CALLBACK CrashProbe(EXCEPTION_POINTERS* ep) {
  if (t_active && ep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
    const PPCContext& c = *t_active;
    REXLOG_ERROR("zdx: fault in transplanted zd code: last zd call site lr=0x{:08X} r1=0x{:08X} "
                 "r3=0x{:08X} r4=0x{:08X} r10=0x{:08X} r11=0x{:08X} r29=0x{:08X} r30=0x{:08X} "
                 "r31=0x{:08X} ctr=0x{:08X} host addr 0x{:X} rip exe+0x{:X}",
                 uint32_t(c.lr), c.r1.u32, c.r3.u32, c.r4.u32, c.r10.u32, c.r11.u32, c.r29.u32,
                 c.r30.u32, c.r31.u32, c.ctr.u32, ep->ExceptionRecord->ExceptionInformation[1],
                 uint64_t(ep->ContextRecord->Rip) - uint64_t(GetModuleHandleW(nullptr)));
    rex::FlushLogging();
  }
  return EXCEPTION_CONTINUE_SEARCH;
}
}  // namespace

// ---- guest calls
GuestCall::GuestCall(PPCContext& ctx, uint8_t* b, uint32_t scratch_bytes) : c(ctx), base(b) {
  EnsureInit(b);
  // Leave a red zone below the caller's frame, then the scratch area, then
  // the callee stack.
  scratch = ((ctx.r1.u32 - 0x400 - scratch_bytes) & ~0xFu);
}
void Run(PPCContext& ctx, uint8_t* base, GuestFn fn) {
  static bool installed =
      // Last in the chain: the runtime's own handlers (GPU memory write
      // watches fault and resume) must see access violations first.
      REXCVAR_GET(zdx_crash_probe) && AddVectoredExceptionHandler(0, CrashProbe) != nullptr;
  (void)installed;
  PPCContext* prev = t_active;
  t_active = &ctx;
  fn(ctx, base);
  t_active = prev;
}
void GuestCall::operator()(GuestFn fn) {
  // The PPC context is a copy, but its FPSCR helpers change the real host
  // floating-point environment. Keep diagnostic math from leaking its
  // rounding/flush mode into the simulation that resumes after this call.
  struct RestoreHostFPSCR {
    uint32_t csr = PPCFPSCRRegister::Platform::getcsr();
    ~RestoreHostFPSCR() { PPCFPSCRRegister::Platform::setcsr(csr); }
  } restore_host_fpscr;
  c.r1.u64 = scratch - 0x100;
  Run(c, base, fn);
}

// ---- zd-only cvars + DebugOverlay::Update
namespace {

// Shape type ids (zd Shape+24; DebugShapeRenderer vtable slot 3 + id) and the
// size of each type's Info (zd allocation size - 32-byte Shape header).
constexpr uint32_t kInfoSize[21] = {48, 48, 80, 112, 112, 112, 112, 160, 112, 64, 112,
                                    112, 96, 176, 176, 176, 96, 96, 128, 48, 96};
const GuestFn kDraw[21] = {
    zdx_83627A28,  // 0  Line3D
    zdx_83628548,  // 1  Line2D
    zdx_8362AB70,  // 2  Curve3D
    zdx_83627A98,  // 3  Sphere
    zdx_83627BB0,  // 4  QuarterSphere
    zdx_83627CC8,  // 5  Circle
    zdx_83627DE0,  // 6  Box
    zdx_83627EF8,  // 7  NonUniformBox
    zdx_8362ACB8,  // 8  Rectangle
    zdx_83628008,  // 9  Arrow
    zdx_83628120,  // 10 Cylinder
    zdx_836293F8,  // 11 Capsule
    zdx_8362ADE0,  // 12 Axes
    zdx_836284D0,  // 13 Text3D
    zdx_836283E0,  // 14 Text2D
    zdx_83628450,  // 15 Text2DOutlined
    zdx_83628240,  // 16 Triangle
    zdx_8362AEC0,  // 17 WideLineSegment
    zdx_8362B0C8,  // 18 WideCurveSegment
    zdx_83628548,  // 19 Line2D (second slot)
    zdx_836285B8,  // 20 Triangle3D_Screen_01
};

struct Shape {
  uint32_t type;
  int32_t life;  // zd Shape+20: frames left
  std::array<uint8_t, 176> info;
};
std::mutex g_mu;
std::vector<Shape> g_shapes;

void AddShape(PPCContext& ctx, uint8_t* base, uint32_t type) {
  Shape s{type, int32_t(ctx.r5.u32), {}};
  const uint32_t info = ctx.r4.u32;
  for (uint32_t i = 0; i < kInfoSize[type]; ++i) s.info[i] = REX_LOAD_U8(info + i);
  if (type >= 13 && type <= 15) s.info[175] = 0;  // inline text: keep it terminated
  {
    std::lock_guard<std::mutex> lock(g_mu);
    // zd's DebugOverlay has a fixed shape pool and drops shapes when it is
    // full; displays that add thousands of long-lived shapes per frame
    // (kShowLocations) otherwise grow the list until drawing it stalls the game.
    constexpr size_t kMaxShapes = 16384;
    if (g_shapes.size() < kMaxShapes) {
      g_shapes.push_back(s);
    } else {
      static std::atomic<bool> warned{false};
      if (!warned.exchange(true)) REXLOG_WARN("zdx: debug overlay full ({} shapes): further shapes dropped", kMaxShapes);
    }
  }
  ctx.r3.u64 = Overlay();  // the Shape* zd returns; callers only test it
}

}  // namespace

// Set once the pool exists and the zd-only cvars are registered (first
// in-game frame): transplanted code may only run after that.
static std::atomic<bool> s_registered{false};

bool Ready() { return s_registered.load(std::memory_order_acquire); }
void TraceBlock(const char* name) {
  if (!REXCVAR_GET(zdx_trace_blocks)) return;
  static std::mutex mu;
  static std::unordered_set<const char*> seen;
  std::lock_guard<std::mutex> lock(mu);
  if (seen.insert(name).second) REXLOG_INFO("zdx: block {} runs", name);
}
bool PoolReady(uint8_t* base) {
  EnsureInit(base);
  return g_pool != 0;
}

void DebugObjectBandwidth(PPCContext& ctx, uint8_t* base, uint32_t bytes);


// zd-only options, ThePrintChannelMan and the DebugOverlay: registered once.
// Called at boot just before m reads startup.ini (Sk8AppMan::InitBootDebug,
// boot.cpp) -- zd created them before its ini read too, so startup.ini and the
// console see them like any other option -- and from BeginFrame as a fallback.
void RegisterZdOnly(PPCContext& ctx, uint8_t* base) {
  EnsureInit(base);
  if (REX_LOAD_U32(kTheCVarMan) == 0) return;   // cvar system not up yet
  if (s_registered.load(std::memory_order_acquire)) return;
  // Boot and the first enabled simulation frame can both request setup.
  // Publish Ready only after every shared debug service is constructed.
  static std::mutex registration_mutex;
  std::lock_guard<std::mutex> registration_lock(registration_mutex);
  if (s_registered.load(std::memory_order_relaxed) || !g_pool) return;
  // Starting with zd_debug off skipped GameModule's debug-manager hook.
  // Late initialization must create the manager before any DebugStringRef
  // constructor/reset can use it, just as the boot path does.
  if (!REX_LOAD_U32(Map(0x849043DCu))) {
    GuestCall manager(ctx, base, 0x200);
    manager(zdx_837B91F0);  // SingletonHolder<Debug::Manager>::Instance
    if (!REX_LOAD_U32(Map(0x849043DCu))) {
      REXLOG_ERROR("zdx: debug text manager unavailable; setup deferred");
      return;
    }
    REXLOG_INFO("zdx: debug text manager initialized before debug features");
  }
  // zd-only cvars: run zd's static initializers once (they register the
  // options with m's cvar system, as zd's static init did).
  if (!s_registered && g_pool) {
    int n = 0;
    for (const ZdCvarInit* c = kZdCvarInits; c->name; ++c, ++n) {
      GuestCall call(ctx, base);
      call(c->init);
      // The registration only queues the cVarRef; the game links every
      // queued ref to its cvar once the cvar system is up (cVarRef::Link,
      // which creates the value block from the ref's default). That pass ran
      // long before this, so link ours the same way.
      if (REX_LOAD_U32(Map(c->zd_obj) + c->var_off) == 0) {
        GuestCall link(ctx, base);
        link.c.r3.u64 = Map(c->zd_obj);
        link.c.r4.u64 = REX_LOAD_U32(kTheCVarMan);
        link(rex_cVarRef_Link_AAAXXZ_82D8A2A8);
      }
    }
    REXLOG_INFO("zdx: registered {} zd-only cvars with the game", n);
    // m's constructors link the ref before storing its default (at normal
    // startup the link happens later, defaults in place), so the variable a
    // late registration creates holds 0: give it the option's default (+28).
    for (const ZdCvarInit* c = kZdCvarInits; c->name; ++c) {
      const uint32_t o = Map(c->zd_obj), var = REX_LOAD_U32(o + c->var_off);
      if (!var) continue;
      switch (c->reg) {
        case 0x83D91558u: REX_STORE_U8(var + 205, REX_LOAD_U8(o + 28)); break;      // bool
        case 0x83D91738u: REX_STORE_U32(var + 208, REX_LOAD_U32(o + 28)); break;    // enum
        case 0x83D915F0u:                                                          // int
        case 0x83D91680u: REX_STORE_U32(var + 216, REX_LOAD_U32(o + 28)); break;    // float
        default: break;                                                            // command
      }
    }
    // zd startup code for objects m lacks (ThePrintChannelMan, ...).
    for (const GuestFn* f = kZdInits; *f; ++f) {
      GuestCall init(ctx, base, 0x200);
      init(*f);
    }
    // The DebugOverlay instance (zd's TheDebugOverlay, a pool block): zd's
    // constructor creates its inner object and the critical section the
    // scoped overlay helpers lock.
    {
      GuestCall ctor(ctx, base, 0x200);
      ctor.c.r3.u64 = Overlay();
      ctor(zdx_83A4CF10);
      REXLOG_INFO("zdx: DebugOverlay constructed, inner 0x{:08X}", REX_LOAD_U32(Overlay()));
    }
    s_registered.store(true, std::memory_order_release);
  }
}

void BeginFrame(PPCContext& ctx, uint8_t* base) {
  RegisterZdOnly(ctx, base);
  // ShapeCollection::UpdateShapes (zd 0x83A48510): a shape stays while its
  // frame count is above zero, counting down once per update.
  uint32_t bytes = 0;
  {
    std::lock_guard<std::mutex> lock(g_mu);
    size_t w = 0;
    for (Shape& s : g_shapes) {
      if (s.life > 0) {
        --s.life;
        g_shapes[w++] = s;
      }
    }
    g_shapes.resize(w);
    // DebugOverlay::Serialize (zd 0x83A48AA0) submits every live shape each
    // sim frame: a 4-byte type word plus the shape's Info.
    for (const Shape& s : g_shapes) bytes += 4 + kInfoSize[s.type];
  }
  DebugObjectBandwidth(ctx, base, bytes);
}

// "Debug Object Bandwidth: x KB/s" on the debug-strings panel. zd's
// DebugOverlayPresentation (which m does not have) named a DebugStringRef in
// its ctor and set it from the size of the overlay data it restored, x60/1024
// (DoSimRestore 0x8389E248); here the same sum comes from the host shape queue.
void DebugObjectBandwidth(PPCContext& ctx, uint8_t* base, uint32_t bytes) {
  if (!REX_LOAD_U32(Map(0x849043DCu))) return;  // no Debug::Manager yet
  const uint32_t ref = Ext(Overlay(), 8);
  if (!REX_LOAD_U32(ref)) {
    GuestCall c(ctx, base, 0x100);
    c.c.r3.u64 = ref;
    c.c.r4.u64 = Map(0x82338900u);  // "Debug Object Bandwidth: "
    c(zdx_839ACA10);                // DebugStringRef::DebugStringRef
    // zd created it first (DebugOverlayPresentation is built before the
    // other presentations): move its node to the head of the manager's
    // string list (eastl::list at +4, node {next, prev, value}; mutex +24).
    const uint32_t manager = REX_LOAD_U32(Map(0x849043DCu));
    const uint32_t ds = REX_LOAD_U32(ref), head = manager + 4;
    GuestCall lk(ctx, base, 0x100);
    lk.c.r3.u64 = manager + 24;
    lk.c.r4.u64 = Map(0x8218F7D0u);  // timeout: infinite (as zd's AquireTextString)
    lk(rex_EA_Thread_Mutex_Lock_QAAHABI_Z_82C39458);
    for (uint32_t node = REX_LOAD_U32(head); node && node != head; node = REX_LOAD_U32(node)) {
      if (REX_LOAD_U32(node + 8) != ds) continue;
      const uint32_t prev = REX_LOAD_U32(node + 4), next = REX_LOAD_U32(node);
      REX_STORE_U32(prev, next);
      REX_STORE_U32(next + 4, prev);
      const uint32_t first = REX_LOAD_U32(head);
      REX_STORE_U32(node, first);
      REX_STORE_U32(node + 4, head);
      REX_STORE_U32(first + 4, node);
      REX_STORE_U32(head, node);
      break;
    }
    GuestCall ul(ctx, base, 0x100);
    ul.c.r3.u64 = manager + 24;
    ul(rex_EA_Thread_Mutex_Unlock_QAAHXZ_82C394F0);
  }
  GuestCall c(ctx, base, 0x200);
  c.c.r3.u64 = ref;
  c.c.f1.f64 = double(float(bytes * 60u) / 1024.0f);
  c.c.r5.u64 = 1;
  c.c.r6.u64 = Map(0x82338A08u);    // "KB/s"
  c(zdx_839ABAA0);                  // DebugStringRef::SetValue(value, precision, unit)
}

void DrawShapes(PPCContext& ctx, uint8_t* base, bool screen_space) {
  std::vector<Shape> shapes;
  {
    std::lock_guard<std::mutex> lock(g_mu);
    shapes = g_shapes;
  }
  if (shapes.empty()) return;
  // Diagnostics: queue histogram + first Info of each type, every 600 draws.
  static int s_dumped = 0;
  if (REXCVAR_GET(zdx_dump_shapes) && (++s_dumped % 600) == 300) {
    int count[21] = {};
    for (const Shape& s : shapes) ++count[s.type];
    std::string h;
    for (int t = 0; t < 21; ++t) if (count[t]) h += fmt::format(" t{}x{}", t, count[t]);
    REXLOG_INFO("zdx: {} shapes:{}", shapes.size(), h);
    bool seen[21] = {};
    for (const Shape& s : shapes) {
      if (seen[s.type]) continue;
      seen[s.type] = true;
      std::string f;
      for (uint32_t k = 0; k < kInfoSize[s.type] && k < 112; k += 4) {
        uint32_t w = uint32_t(s.info[k]) << 24 | uint32_t(s.info[k + 1]) << 16 | uint32_t(s.info[k + 2]) << 8 | s.info[k + 3];
        f += fmt::format(" {:.3f}", std::bit_cast<float>(w));
      }
      REXLOG_INFO("zdx: type {} life {}:{}", s.type, s.life, f);
    }
  }
  // Gfx setup must exist (render states are created by the game at boot).
  if (!REX_LOAD_U32(0x83368E6Cu) || !REX_LOAD_U32(0x833689C4u)) return;
  // zd's debug text printer global (0x848FEBE8) is GraphicsMan's text
  // printer, *(*(gfxman + 0x429D0)); m's printer has the same vtable at the
  // slots zd's Text::Draw uses (+12/+24/+28/+36/+96, code-identical).
  {
    const uint32_t gfx = REX_LOAD_U32(0x83368E6Cu);
    const uint32_t holder = gfx ? REX_LOAD_U32(gfx + 0x429D0) : 0;
    const uint32_t printer = holder ? REX_LOAD_U32(holder) : 0;
    REX_STORE_U32(Map(0x848FEBE8u), printer);
  }
  GuestCall call(ctx, base, 0x100);
  const uint32_t info = call.scratch;
  // Shape::DrawAll (zd 0x83A47EE8): renderer->Begin(), then one virtual draw
  // per shape with its Info.
  call.c.r3.u64 = Overlay();
  call(zdx_836273B8);
  // 3D shapes are drawn from the world debug-draw callback, the screen-space
  // ones (Line2D, Rectangle, text) from the debug HUD text pass: with a
  // text-printer draw in the same callback the native renderer drops the 3D
  // SimpleDraw output. zd draws both from one stream; the image is the same
  // (2D lands on top).
  constexpr uint32_t k2dTypes = (1u << 1) | (1u << 8) | (1u << 13) | (1u << 14) | (1u << 15) | (1u << 19) |
                                (1u << 20);
  const uint32_t mask = uint32_t(REXCVAR_GET(zdx_draw_type_mask));
  for (const Shape& s : shapes) {
    const bool is2d = (k2dTypes >> s.type) & 1;
    if (is2d != screen_space || !(mask & (1u << s.type))) continue;
    for (uint32_t k = 0; k < kInfoSize[s.type]; ++k) REX_STORE_U8(info + k, s.info[k]);
    call.c.r3.u64 = Overlay();
    call.c.r4.u64 = info;
    call(kDraw[s.type]);
  }
}

// The debug-strings panel: zd's Debug::Manager (DebugTextManager, created at
// app init) draws its DebugStrings as a scene-graph render object. Here it is
// drawn from the HUD text pass instead (same printer, text on top; a printer
// draw inside the scene pass makes the native renderer drop the 3D output).
void DrawDebugStrings(PPCContext& ctx, uint8_t* base) {
  if (!g_pool) return;
  const uint32_t manager = REX_LOAD_U32(Map(0x849043DCu));
  const uint32_t gfx = REX_LOAD_U32(0x83368E6Cu);
  const uint32_t holder = gfx ? REX_LOAD_U32(gfx + 0x429D0) : 0;
  const uint32_t printer = holder ? REX_LOAD_U32(holder) : 0;
  if (!manager || !printer) return;
  REX_STORE_U32(Map(0x848FEBE8u), printer);
  GuestCall call(ctx, base, 0x100);
  call.c.r3.u64 = manager;
  call(zdx_83629558);  // DebugTextManager::Draw
}

}  // namespace zdx

// m's debug HUD text pass (SFPS/RFPS print through the same printer zd's
// Text::Draw uses): zd's screen-space shapes and debug strings go here.
extern "C" REX_FUNC(rex_Sk8_Render_DisplayFPS_YAXMMMMHH_Z_8297EFC0) {
  __imp__rex_Sk8_Render_DisplayFPS_YAXMMMMHH_Z_8297EFC0(ctx, base);
  if (zd_debug::overlay::Active()) {
    zdx::DrawShapes(ctx, base, true);
    zdx::DrawDebugStrings(ctx, base);
  }
}

// ---- host replacements called by the transplanted code

void zdx_host_FailAssert(PPCContext& __restrict ctx, uint8_t* base) {
  // xcore::FailAssert(expr, file, line): each distinct site logged a few times
  static std::mutex mu;
  static std::unordered_map<uint64_t, int> seen;
  bool log;
  {
    std::lock_guard<std::mutex> lock(mu);
    log = seen[(uint64_t(ctx.r3.u32) << 32) | uint32_t(ctx.lr)]++ < 3 && seen.size() < 500;
  }
  if (log) {
    auto str = [&](uint32_t a) -> std::string {
      std::string s;
      for (uint32_t i = 0; a && i < 200; ++i) {
        const char ch = char(REX_LOAD_U8(a + i));
        if (!ch) break;
        s += ch;
      }
      return s;
    };
    REXLOG_WARN("zdx: zd assert '{}' at {}({}), zd call site 0x{:08X}", str(ctx.r3.u32), str(ctx.r4.u32),
                ctx.r5.s32, uint32_t(ctx.lr) - 4);
  }
}
void zdx_host_DebugBreak(PPCContext& __restrict, uint8_t*) {}
void zdx_host_EAAssert(PPCContext& __restrict ctx, uint8_t*) { ctx.r3.u64 = 0; }
void zdx_host_Nop(PPCContext& __restrict ctx, uint8_t*) { ctx.r3.u64 = 0; }
// xcore::FmtAssertMessage(fmt, ...): formats the message zd's FailAssert gets;
// the format string itself is enough for zdx_host_FailAssert's log line.
void zdx_host_FmtAssertMessage(PPCContext& __restrict ctx, uint8_t*) {}

// cVarBoolRef::cVarBoolRef(name, group, label, default, callback) (zd
// 83D91558 = m 82D8A478). zd constructs some refs at startup, before the cvar
// system links queued refs (print channels: "kbDebugPrint_<channel>"). Here
// they are made late, so link each one now, as the startup pass would have
// (it attaches m's existing option of that name, or creates the value).
void zdx_host_cVarBoolRef(PPCContext& __restrict ctx, uint8_t* base) {
  const uint32_t ref = ctx.r3.u32;
  // m's ctor links the ref (creating the option's value block from the
  // default at ref+28) before it stores that default: store it first so a
  // new option starts at its default.
  if (ref) REX_STORE_U8(ref + 28, uint8_t(ctx.r7.u32 != 0));
  rex_f0cVarBoolRef_QAA_PBD00_NP6AXXZ_Z_82D8A478(ctx, base);
  // zd's static option objects (in the pool) are linked by BeginFrame after
  // their whole initializer ran; only heap-made refs are linked here.
  const bool in_pool = zdx::g_pool && ref >= zdx::g_pool && ref < zdx::g_pool + uint32_t(zdx::kPoolSize);
  if (ref && !in_pool && REX_LOAD_U32(ref + 32) == 0) {
    zdx::GuestCall link(ctx, base);
    link.c.r3.u64 = ref;
    link.c.r4.u64 = REX_LOAD_U32(zdx::kTheCVarMan);
    link(rex_cVarRef_Link_AAAXXZ_82D8A2A8);
  }
  ctx.r3.u64 = ref;
}

// _localtime64(const __time64_t*): zd's debug CRT version (m's release CRT
// has none). Same contract: a static struct tm (9 big-endian ints), or null.
void zdx_host_localtime64(PPCContext& __restrict ctx, uint8_t* base) {
  static uint32_t s_tm = 0;
  if (!s_tm) s_tm = zdx::GuestAlloc(9 * 4, 8);
  const uint32_t pt = ctx.r3.u32;
  ctx.r3.u64 = 0;
  if (!pt || !s_tm) return;
  const __time64_t t = __time64_t(REX_LOAD_U64(pt));
  std::tm tm{};
  if (_localtime64_s(&tm, &t) != 0) return;
  const int v[9] = {tm.tm_sec, tm.tm_min,  tm.tm_hour, tm.tm_mday, tm.tm_mon,
                    tm.tm_year, tm.tm_wday, tm.tm_yday, tm.tm_isdst};
  for (int i = 0; i < 9; ++i) REX_STORE_U32(s_tm + i * 4, uint32_t(v[i]));
  ctx.r3.u64 = s_tm;
}

// __savegprlr_14: m never emitted the r14 variant.
void zdx_savegprlr_14(PPCContext& __restrict ctx, uint8_t* base) {
  REX_STORE_U64(ctx.r1.u32 + -152, ctx.r14.u64);
  rex_savegprlr_15_83190BE4(ctx, base);
}

#define ZDX_ADDSHAPE(name, type) \
  void zdx_host_AddShape_##name(PPCContext& __restrict ctx, uint8_t* base) { zdx::AddShape(ctx, base, type); }
ZDX_ADDSHAPE(Line3D, 0)
ZDX_ADDSHAPE(Line2D, 1)
ZDX_ADDSHAPE(Curve3D, 2)
ZDX_ADDSHAPE(Sphere, 3)
ZDX_ADDSHAPE(Circle, 5)
ZDX_ADDSHAPE(Box, 6)
ZDX_ADDSHAPE(NonUniformBox, 7)
ZDX_ADDSHAPE(Rectangle, 8)
ZDX_ADDSHAPE(Arrow, 9)
ZDX_ADDSHAPE(Cylinder, 10)
ZDX_ADDSHAPE(Capsule, 11)
ZDX_ADDSHAPE(Axes, 12)
ZDX_ADDSHAPE(Text3D, 13)
ZDX_ADDSHAPE(Text2D, 14)
ZDX_ADDSHAPE(Text2DOutlined, 15)
ZDX_ADDSHAPE(Triangle, 16)
ZDX_ADDSHAPE(WideLineSegment, 17)
ZDX_ADDSHAPE(WideCurveSegment, 18)
