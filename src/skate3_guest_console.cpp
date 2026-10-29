// Guest debug-variable console. See skate3_guest_console.h for the cVar
// memory layout this edits and how the registry was recovered.

#include "skate3_guest_console.h"

#include <algorithm>
#include <bit>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>   // skate3_native_scene_state.h uses std::ofstream
#include <string>
#include <utility>
#include <vector>

#include <imgui.h>
#include <rex/logging.h>

#include <mutex>

#include "generated/sk83_na_m_init.h"  // REX_RAW_ADDR
#include "native/skate3_native_guest_read.h"  // GuestTryCopy
#include "skate3_native_scene_state.h"

namespace skate3 {
namespace {

// tVarLabel layout, recovered from cVarSection::FindLabel (0x82D888F8) and
// the Set_cVar*Ref setters, then validated against startup.ini:
//
//   +0     type tag: 1 bool, 2 int, 4 float, 6 enum, 10 callback
//   +4     name, INLINE (FindLabel string-compares from here)
//   +196   next label in this section
//   +204   bool: default byte, LIVE value the byte at +205
//          int / float: DEFAULT
//   +208   int / float: min;   enum: LIVE value
//   +212   int / float: max;   enum: tag count
//   +216   int / float: LIVE value;   enum: tag table
//
// The live slots are exactly where the game's own setters store
// (Set_cVarBoolRef 82D89978: stb +205; Set_cVarIntRef 82D89AD8 and
// Set_cVarFloatRef 82D89C48: stw/stfs +216; Set_cVarEnumRef 82D89DE8:
// stw +208) and where the game's readers load them (float cVarRefs:
// lwz label,40(ref); lfs 216(label)). CreateFloatRef (82D88C20) seeds
// +204 AND +216 with the initial value, which is why the old +204 reads
// looked right until something changed: int / float / enum edits wrote the
// default slot, echoed back fine, and never reached the game.
//
// The old static table read the cVar OBJECT instead, whose +4 is a ctor
// constant -- it reported kbVignette = 1 where the ini (and this label)
// say 0, which is why values never looked live.
constexpr uint32_t kLabelType = 0;
constexpr uint32_t kLabelName = 4;
constexpr uint32_t kLabelNext = 196;
constexpr uint32_t kLabelValue = 204;  // bool word (default byte + live byte)
constexpr uint32_t kLabelBoolValue = 205;
constexpr uint32_t kLabelNumValue = 216;   // int / float live value
constexpr uint32_t kLabelEnumValue = 208;  // enum live value
constexpr uint32_t kLabelMin = 208;
constexpr uint32_t kLabelMax = 212;
// Enums instead use +212 as the TAG COUNT and +216 as a pointer to an array
// of {const char* name, int32_t value} pairs -- cVarSection::CreateEnumRef
// hands the "a=0,b=1,..." string to tVarEnum::ParseEnumTags (0x82D88390),
// which fills this table. 138 of the 145 enums in a live registry decode.
constexpr uint32_t kLabelEnumCount = 212;
constexpr uint32_t kLabelEnumTags = 216;

// cVarSection: +4 parent, +8 next sibling, +12 first label, +16 first
// child, +20 name (inline).
constexpr uint32_t kSectionNext = 8;
constexpr uint32_t kSectionLabels = 12;
constexpr uint32_t kSectionChild = 16;
constexpr uint32_t kSectionName = 20;

// The cVarMan global; its +4 is the root section.
constexpr uint32_t kVarManPtr = 0x83360D20u;

// Label type tags.
constexpr uint32_t kTypeBool = 1;
constexpr uint32_t kTypeInt = 2;
constexpr uint32_t kTypeFloat = 4;
constexpr uint32_t kTypeEnum = 6;

// The word holding a label's live value (bools: the word whose second byte
// is the live value, see the layout above).
uint32_t LiveValueOffset(uint8_t type) {
  return type == kTypeBool   ? kLabelValue
         : type == kTypeEnum ? kLabelEnumValue
                             : kLabelNumValue;
}

// Guest memory access. A guest address IS the offset into the mapping
// (REX_RAW_ADDR(x) == base + x + phys-mirror adjust), and every read goes
// through the project's fault-recovering helpers: a stale or mis-decoded
// registry address must return "unreadable", never fault the UI thread.
// (An earlier version subtracted 0x82000000 from the address before
// indexing, which pointed ~2 GB before the mapping and crashed on open.)
bool ReadU32(uint32_t addr, uint32_t* out) {
  uint8_t* base = native_scene::g_guest_base.load(std::memory_order_relaxed);
  if (base == nullptr || addr < 0x10000u) {
    return false;
  }
  return native_scene::GuestTryLoadU32(base, addr, out);
}

bool WriteU32(uint32_t addr, uint32_t value) {
  uint8_t* base = native_scene::g_guest_base.load(std::memory_order_relaxed);
  if (base == nullptr || addr < 0x10000u) {
    return false;
  }
  // Only write where a read already succeeded: proves the page is mapped
  // and writable-ish before poking the game's live state.
  uint32_t probe = 0;
  if (!native_scene::GuestTryLoadU32(base, addr, &probe)) {
    return false;
  }
  const uint32_t raw = __builtin_bswap32(value);
  std::memcpy(REX_RAW_ADDR(addr), &raw, 4);
  return true;
}

std::string ReadString(uint32_t addr, size_t limit = 96) {
  uint8_t* base = native_scene::g_guest_base.load(std::memory_order_relaxed);
  if (base == nullptr || addr < 0x10000u) {
    return {};
  }
  char buf[128];
  if (limit > sizeof(buf) - 1) {
    limit = sizeof(buf) - 1;
  }
  if (!native_scene::GuestTryCopy(buf, REX_RAW_ADDR(addr), limit)) {
    return {};
  }
  buf[limit] = 0;
  std::string s;
  for (size_t i = 0; i < limit && buf[i] != 0; ++i) {
    const uint8_t c = uint8_t(buf[i]);
    if (c < 0x20 || c > 0x7E) {
      return {};  // not a printable C string: treat as unreadable
    }
    s.push_back(char(c));
  }
  return s;
}

std::string LowerOf(std::string_view s) {
  std::string o;
  o.reserve(s.size());
  for (char c : s) {
    o.push_back(char(std::tolower(static_cast<unsigned char>(c))));
  }
  return o;
}

// Subsequence match with a score: a contiguous run and a prefix hit both
// rank above scattered letters, so "postfx" puts kbEnablePostFX on top.
bool FuzzyScore(std::string_view hay_lower, std::string_view needle_lower,
                int* score) {
  if (needle_lower.empty()) {
    *score = 0;
    return true;
  }
  size_t hi = 0;
  int s = 0;
  int run = 0;
  for (char nc : needle_lower) {
    bool found = false;
    while (hi < hay_lower.size()) {
      if (hay_lower[hi] == nc) {
        found = true;
        run = (hi > 0 && run > 0) ? run + 1 : 1;
        s += 10 + run * 5;
        if (hi == 0) {
          s += 40;  // match starts at the name's first character
        }
        ++hi;
        break;
      }
      run = 0;
      ++hi;
    }
    if (!found) {
      return false;
    }
  }
  // Prefer shorter names: an exact-ish hit shouldn't lose to a long name
  // that happens to contain the same letters.
  s -= int(hay_lower.size()) / 4;
  *score = s;
  return true;
}

// Read a label's LIVE value as a display string (see LiveValueOffset).
std::string ReadLiveValue(uint32_t label, uint8_t type) {
  if (type == kTypeBool) {
    uint32_t w = 0;
    if (!ReadU32(label + kLabelValue, &w)) {
      return "?";
    }
    // Big-endian word: +205 is the second byte.
    return ((w >> 16) & 0xFFu) != 0 ? "1" : "0";
  }
  uint32_t w = 0;
  if (!ReadU32(label + LiveValueOffset(type), &w)) {
    return "?";
  }
  if (type == kTypeFloat) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%g", std::bit_cast<float>(w));
    return buf;
  }
  return std::to_string(int32_t(w));
}

// As above, into a caller-provided buffer: used by the per-row draw, which
// runs for every visible row every frame and should not allocate.
void ReadLiveValueInto(uint32_t label, uint8_t type, char* out, size_t cap) {
  if (out == nullptr || cap == 0) {
    return;
  }
  out[0] = 0;
  uint32_t w = 0;
  if (!ReadU32(label + LiveValueOffset(type), &w)) {
    std::snprintf(out, cap, "?");
    return;
  }
  if (type == kTypeBool) {
    // Big-endian word: +205 is the second byte.
    std::snprintf(out, cap, "%s", ((w >> 16) & 0xFFu) != 0 ? "1" : "0");
  } else if (type == kTypeFloat) {
    std::snprintf(out, cap, "%g", std::bit_cast<float>(w));
  } else {
    std::snprintf(out, cap, "%d", int32_t(w));
  }
}

// An enum's allowed values, as {name, value} pairs. Empty when the tag
// table is absent or implausible.
std::vector<std::pair<std::string, int32_t>> ReadEnumTags(uint32_t label) {
  std::vector<std::pair<std::string, int32_t>> out;
  uint32_t count = 0;
  uint32_t array = 0;
  if (!ReadU32(label + kLabelEnumCount, &count) ||
      !ReadU32(label + kLabelEnumTags, &array) || array == 0 || count == 0 ||
      count > 64) {
    return out;
  }
  for (uint32_t i = 0; i < count; ++i) {
    uint32_t name_ptr = 0;
    uint32_t value = 0;
    if (!ReadU32(array + i * 8, &name_ptr) ||
        !ReadU32(array + i * 8 + 4, &value)) {
      break;
    }
    std::string name = ReadString(name_ptr, 48);
    if (name.empty()) {
      continue;
    }
    out.emplace_back(std::move(name), int32_t(value));
  }
  return out;
}

// Fallback font: the atlas default, used only when the drawer has no UI font
// of its own.
ImFont* ConsoleFont(ImGuiIO& io) {
  if (io.Fonts == nullptr || io.Fonts->Fonts.Size == 0) {
    return nullptr;
  }
  ImFont* f = io.Fonts->Fonts[0];
  return (f != nullptr && f->IsLoaded()) ? f : nullptr;
}

// Width at the currently pushed font/size. Safe only between PushFont and
// PopFont, which is where every caller sits.
float TextWidth(float size, const char* text) {
  ImFont* f = ImGui::GetFont();
  if (f == nullptr || text == nullptr || text[0] == 0) {
    return 0.0f;
  }
  return f->CalcTextSizeA(size, FLT_MAX, 0.0f, text).x;
}

const char* KindLabel(uint8_t k) {
  switch (k) {
    case kTypeBool: return "bool";
    case kTypeInt: return "int";
    case kTypeFloat: return "float";
    case kTypeEnum: return "enum";
  }
  return "?";
}

}  // namespace (anonymous: the file-local helpers above)

namespace {
std::mutex g_pending_mutex;
std::vector<PendingSet> g_pending;
}  // namespace

void QueueGuestCVarSet(const PendingSet& set) {
  std::lock_guard<std::mutex> lock(g_pending_mutex);
  if (g_pending.size() < 64) {
    g_pending.push_back(set);
  }
}

void ApplyPendingGuestCVarSets(uint8_t* /*base*/) {
  std::vector<PendingSet> todo;
  {
    std::lock_guard<std::mutex> lock(g_pending_mutex);
    if (g_pending.empty()) {
      return;
    }
    todo.swap(g_pending);
  }
  for (const PendingSet& s : todo) {
    // Write the tVarLabel directly. The setters (Set_cVar*Ref) take the cVar
    // OBJECT, not the label, and reach the same field through
    // FindSection/FindLabel -- but they are also what made bools one-way:
    // the previous build drove them with the object address, so the lookup
    // found nothing on the way back up. Writing the label is the same store
    // the setter performs, and it can be verified by reading it back.
    uint32_t word = 0;
    bool ok = false;
    if (s.kind == kTypeBool) {
      // Live value is the BYTE at +205; preserve the default byte at +204.
      uint32_t cur = 0;
      if (ReadU32(s.object + kLabelValue, &cur)) {
        const uint32_t b = s.value != 0.0f ? 1u : 0u;
        word = (cur & 0xFF00FFFFu) | (b << 16);
        ok = WriteU32(s.object + kLabelValue, word);
      }
    } else {
      // int / float: +216, enum: +208 -- the setters' own stores.
      word = s.word;
      ok = WriteU32(s.object + LiveValueOffset(s.kind), word);
    }
    REXLOG_INFO("guest-console: {} label {:08X} = {} (now {})",
                ok ? "applied" : "FAILED", s.object, s.value,
                ReadLiveValue(s.object, s.kind));
  }
}

void GuestConsoleDialog::Show() {
  visible_ = true;
  focus_input_ = true;
}
void GuestConsoleDialog::Hide() {
  visible_ = false;
  hints_.clear();  // rebuilt on the next draw; nothing should outlive a close
}
void GuestConsoleDialog::Toggle() {
  visible_ = !visible_;
  focus_input_ = visible_;
}

// ---- text editing -------------------------------------------------------
//
// All edits funnel through these so the fuzzy filter, the committed-variable
// state and the caret can never drift apart from input_.

void GuestConsoleDialog::OnInputChanged() {
  // An edit that removes the '=' puts us back to picking a name.
  if (input_.find('=') == std::string::npos) {
    committed_ = -1;
  }
  const int len = int(input_.size());
  caret_ = caret_ < 0 ? 0 : (caret_ > len ? len : caret_);
  sel_ = sel_ < 0 ? 0 : (sel_ > len ? len : sel_);
  selected_ = 0;
  RebuildMatches();
  caret_moved_at_ = ImGui::GetTime();
}

void GuestConsoleDialog::SetCaret(int pos, bool extend_selection) {
  const int len = int(input_.size());
  caret_ = pos < 0 ? 0 : (pos > len ? len : pos);
  if (!extend_selection) {
    sel_ = caret_;
  }
  caret_moved_at_ = ImGui::GetTime();
}

void GuestConsoleDialog::DeleteSelection() {
  if (!HasSelection()) {
    return;
  }
  const int lo = SelMin();
  const int hi = SelMax();
  input_.erase(size_t(lo), size_t(hi - lo));
  caret_ = lo;
  sel_ = lo;
  OnInputChanged();
}

void GuestConsoleDialog::InsertText(const char* text) {
  if (text == nullptr || text[0] == 0) {
    return;
  }
  DeleteSelection();
  const int len = int(input_.size());
  if (caret_ < 0 || caret_ > len) {
    caret_ = len;
  }
  input_.insert(size_t(caret_), text);
  caret_ += int(std::strlen(text));
  sel_ = caret_;
  OnInputChanged();
}

void GuestConsoleDialog::EnsureRegistry() {
  if (registry_built_) {
    return;
  }
  // The guest image has to be mapped before any name can be resolved; until
  // then leave the registry unbuilt and retry on the next frame.
  if (native_scene::g_guest_base.load(std::memory_order_relaxed) == nullptr) {
    return;
  }
  // Walk the game's live cVar tree (cVarMan -> sections -> labels). This is
  // the structure the debug menu itself edits, so the values are real.
  uint32_t man = 0;
  if (!ReadU32(kVarManPtr, &man) || man == 0) {
    return;  // registry not constructed yet; retry next frame
  }
  uint32_t root = 0;
  if (!ReadU32(man + 4, &root) || root == 0) {
    return;
  }
  // Iterative walk with an explicit stack and a visited set: the tree is
  // built from guest memory, so a corrupt or mid-construction link must not
  // send this into infinite recursion.
  std::vector<std::pair<uint32_t, std::string>> stack{{root, std::string()}};
  std::vector<uint32_t> seen;
  seen.reserve(512);
  while (!stack.empty() && rows_.size() < 8192) {
    const auto [sec, parent_path] = stack.back();
    stack.pop_back();
    if (sec == 0 || std::find(seen.begin(), seen.end(), sec) != seen.end()) {
      continue;
    }
    seen.push_back(sec);
    const std::string sec_name = ReadString(sec + kSectionName, 60);
    const std::string path =
        sec_name.empty() ? parent_path : parent_path + "/" + sec_name;

    uint32_t label = 0;
    ReadU32(sec + kSectionLabels, &label);
    uint32_t guard = 0;
    while (label != 0 && guard++ < 4096) {
      const std::string name = ReadString(label + kLabelName, 60);
      uint32_t type = 0;
      ReadU32(label + kLabelType, &type);
      if (!name.empty() &&
          (type == kTypeBool || type == kTypeInt || type == kTypeFloat ||
           type == kTypeEnum)) {
        Row row;
        row.label = nullptr;
        row.object = label;  // the tVarLabel itself
        row.kind = uint8_t(type);
        row.desc = path;
        names_.push_back(name);
        rows_.push_back(std::move(row));
      }
      uint32_t next = 0;
      if (!ReadU32(label + kLabelNext, &next)) {
        break;
      }
      label = next;
    }

    uint32_t sibling = 0;
    if (ReadU32(sec + kSectionNext, &sibling) && sibling != 0) {
      stack.emplace_back(sibling, parent_path);
    }
    uint32_t child = 0;
    if (ReadU32(sec + kSectionChild, &child) && child != 0) {
      stack.emplace_back(child, path);
    }
  }
  // Bind labels after names_ has stopped reallocating.
  for (size_t i = 0; i < rows_.size(); ++i) {
    rows_[i].label = names_[i].c_str();
  }
  lower_.reserve(names_.size());
  for (const std::string& n : names_) {
    lower_.push_back(LowerOf(n));
  }
  registry_built_ = true;
  REXLOG_INFO("guest-console: {} debug variables resolved", rows_.size());
}

void GuestConsoleDialog::RebuildMatches() {
  matches_.clear();
  // After a name is committed the suggestions become that variable's
  // values, so the name filter is not rebuilt.
  if (committed_ >= 0) {
    return;
  }
  const size_t eq = input_.find('=');
  const std::string needle =
      LowerOf(eq == std::string::npos ? input_ : input_.substr(0, eq));
  std::vector<std::pair<int, int>> scored;
  scored.reserve(64);
  for (size_t i = 0; i < rows_.size(); ++i) {
    int s = 0;
    if (FuzzyScore(lower_[i], needle, &s)) {
      scored.emplace_back(-s, int(i));
    }
  }
  std::partial_sort(scored.begin(),
                    scored.begin() + std::min<size_t>(scored.size(), 40),
                    scored.end());
  for (size_t i = 0; i < scored.size() && i < 40; ++i) {
    matches_.push_back(scored[i].second);
  }
  if (selected_ >= int(matches_.size())) {
    selected_ = matches_.empty() ? 0 : int(matches_.size()) - 1;
  }
}

void GuestConsoleDialog::Apply(const Row& row, const std::string& value) {
  if (value.empty()) {
    return;
  }
  uint32_t word = 0;
  {
    const char* first = value.c_str();
    char* end = nullptr;
    errno = 0;
    if (row.kind == kTypeFloat) {
      const float f = std::strtof(first, &end);
      if (end == first || errno == ERANGE) {
        status_ = "not a number: " + value;
        return;
      }
      word = std::bit_cast<uint32_t>(f);
    } else {
      const long v = std::strtol(first, &end, 0);
      if (end == first || errno == ERANGE) {
        status_ = "not a number: " + value;
        return;
      }
      word = uint32_t(v);
    }
  }
  // Queue for the guest thread: the setter must run with a live
  // ThreadState, which the UI thread does not have.
  PendingSet ps;
  ps.object = row.object;
  ps.kind = row.kind;
  ps.value = row.kind == kTypeFloat ? std::bit_cast<float>(word)
                                    : float(int32_t(word));
  ps.word = word;
  QueueGuestCVarSet(ps);
  status_ = std::string(row.label) + " = " + value + "  (queued)";
  REXLOG_INFO("guest-console: queue {} = {} (object {:08X})", row.label, value,
              row.object);
}

// Everything below is painted by hand onto ImGui's foreground draw list:
// no ImGui windows, widgets or theme, so the console shares nothing with the
// SDK's dialogs. Input is handled directly off ImGuiIO (character queue plus
// key presses), which also keeps the text caret ours to place.
// The drawing body. Called only through OnDraw, which guards it: a fault in
// here disables the console instead of killing the process, so a bad frame
// costs the console and not the session.
// Paints the console. Called from OnDraw, which owns the ImGui window, the
// font push and their matching pops -- so an early return in here cannot
// leave ImGui's stacks unbalanced, and this frame needs no scope guards.
void GuestConsoleDialog::DrawContents(ImGuiIO& io, ImDrawList* dl) {
  const float scale = ImGui::GetStyle().FontScaleDpi > 0.0f
                          ? ImGui::GetStyle().FontScaleDpi
                          : 1.0f;
  // The size ImGui resolved and baked for the pushed font, rather than one
  // computed here.
  const float kFont = ImGui::GetFontSize();
  const float kRow = kFont + 5.0f * scale;
  const float kPad = 10.0f * scale;
  const float kWidth = 720.0f * scale;
  const float kInputH = 30.0f * scale;
  const ImVec2 origin(16.0f * scale, 16.0f * scale);

  // Palette: a dark slate panel, a brighter input strip, and a suggestion
  // list that is deliberately translucent so the game stays readable behind
  // it (the list is the tallest element and would otherwise blank out a
  // third of the screen).
  const ImU32 kPanelBg = IM_COL32(16, 18, 24, 232);
  const ImU32 kInputBg = IM_COL32(28, 32, 42, 240);
  const ImU32 kListBg = IM_COL32(14, 16, 22, 150);
  const ImU32 kSelBg = IM_COL32(58, 118, 196, 190);
  const ImU32 kAccent = IM_COL32(104, 176, 255, 255);
  const ImU32 kText = IM_COL32(226, 232, 240, 255);
  const ImU32 kTextDim = IM_COL32(138, 148, 164, 255);
  const ImU32 kTextFaint = IM_COL32(100, 110, 126, 255);
  const ImU32 kValue = IM_COL32(126, 214, 160, 255);
  const ImU32 kBorder = IM_COL32(70, 80, 100, 120);

  if (!registry_built_) {
    const ImVec2 p = origin;
    dl->AddRectFilled(p, ImVec2(p.x + kWidth, p.y + kInputH), kPanelBg,
                      6.0f * scale);
    dl->AddText(ImVec2(p.x + kPad, p.y + 7.0f * scale),
                kTextDim, "waiting for the guest image...");
    return;
  }

  // Deliberately NOT setting io.WantTextInput / WantCaptureKeyboard: those
  // make imgui_drawer::OnKey (imgui_drawer.cpp, "if (is_down &&
  // io.WantTextInput)") mark every key-down as handled, which blocks the
  // game AND the console's own toggle bind.
  //
  // By design the console captures nothing -- the game stays fully playable
  // with it open, and keys the console reads still reach the game. The
  // consequence is that typing a variable name also drives the skater; that
  // is the accepted trade for zero interference.

  // ---- input ------------------------------------------------------------
  // Close on the same physical key that opens it. Both keys are checked for
  // the same reason the app registers two binds: the key left of "1" is
  // VK_OEM_3 (-> GraveAccent) on US/UK layouts and VK_OEM_5 (-> Backslash)
  // on Nordic ones.
  //
  // Skipped on the frame the console opened: the app bind runs before this
  // draw, so that same press is still "pressed" here and would close the
  // console in the act of opening it.
  if (!focus_input_ && (ImGui::IsKeyPressed(ImGuiKey_GraveAccent, false) ||
                        ImGui::IsKeyPressed(ImGuiKey_Backslash, false))) {
    Hide();
    return;
  }

  // Typed characters arrive in the IO queue. We only read it: the queue is
  // owned by the ImGui context, which clears it at end-of-frame, so clearing
  // it here would desync it from the pending input-event queue.
  const bool ctrl = io.KeyCtrl;
  const bool shift = io.KeyShift;
  if (!ctrl) {
    for (int i = 0; i < io.InputQueueCharacters.Size; ++i) {
      const ImWchar c = io.InputQueueCharacters[i];
      // Skip the toggle keys' own glyphs so pressing them never types into
      // the box. VK_OEM_3/VK_OEM_5 print '`' and '\' on US/UK; on Nordic
      // layouts the same physical key prints the paragraph sign, which is
      // outside the ASCII range this loop accepts anyway.
      if (c >= 0x20 && c < 0x7F && c != '`' && c != '\\' && !focus_input_) {
        const char text[2] = {char(c), 0};
        InsertText(text);
      }
    }
  }
  focus_input_ = false;

  const bool key_up = ImGui::IsKeyPressed(ImGuiKey_UpArrow, true);
  const bool key_down = ImGui::IsKeyPressed(ImGuiKey_DownArrow, true);
  const bool key_enter = ImGui::IsKeyPressed(ImGuiKey_Enter, false) ||
                         ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false);
  const bool key_esc = ImGui::IsKeyPressed(ImGuiKey_Escape, false);

  // ---- caret movement, selection and deletion ---------------------------
  const int len = int(input_.size());
  // Word-wise steps for Ctrl+Left/Right and Ctrl+Backspace/Delete: skip any
  // run of separators, then the word itself, matching typical text boxes.
  const auto is_word_char = [](char ch) {
    return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') ||
           (ch >= 'a' && ch <= 'z') || ch == '_';
  };
  const auto word_left = [&](int from) {
    int p = from;
    while (p > 0 && !is_word_char(input_[size_t(p - 1)])) {
      --p;
    }
    while (p > 0 && is_word_char(input_[size_t(p - 1)])) {
      --p;
    }
    return p;
  };
  // Reads input_.size() live rather than the `len` above: edits earlier in
  // the frame (a paste, a delete) change the length, and the word-wise mouse
  // selection below runs after them.
  const auto word_right = [&](int from) {
    int p = from;
    const int n = int(input_.size());
    while (p < n && !is_word_char(input_[size_t(p)])) {
      ++p;
    }
    while (p < n && is_word_char(input_[size_t(p)])) {
      ++p;
    }
    return p;
  };

  if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, true)) {
    if (HasSelection() && !shift) {
      SetCaret(SelMin(), false);
    } else {
      SetCaret(ctrl ? word_left(caret_) : caret_ - 1, shift);
    }
  }
  if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, true)) {
    if (HasSelection() && !shift) {
      SetCaret(SelMax(), false);
    } else {
      SetCaret(ctrl ? word_right(caret_) : caret_ + 1, shift);
    }
  }
  if (ImGui::IsKeyPressed(ImGuiKey_Home, false)) {
    SetCaret(0, shift);
  }
  if (ImGui::IsKeyPressed(ImGuiKey_End, false)) {
    SetCaret(len, shift);
  }
  if (ctrl && ImGui::IsKeyPressed(ImGuiKey_A, false)) {
    sel_ = 0;
    SetCaret(len, true);
  }

  if (ImGui::IsKeyPressed(ImGuiKey_Backspace, true)) {
    if (HasSelection()) {
      DeleteSelection();
    } else if (caret_ > 0) {
      const int to = ctrl ? word_left(caret_) : caret_ - 1;
      input_.erase(size_t(to), size_t(caret_ - to));
      caret_ = to;
      sel_ = to;
      OnInputChanged();
    }
  }
  if (ImGui::IsKeyPressed(ImGuiKey_Delete, true)) {
    if (HasSelection()) {
      DeleteSelection();
    } else if (caret_ < int(input_.size())) {
      const int to = ctrl ? word_right(caret_) : caret_ + 1;
      input_.erase(size_t(caret_), size_t(to - caret_));
      OnInputChanged();
    }
  }

  // ---- clipboard ---------------------------------------------------------
  if (ctrl && (ImGui::IsKeyPressed(ImGuiKey_C, false) ||
               ImGui::IsKeyPressed(ImGuiKey_X, false))) {
    const bool cut = ImGui::IsKeyPressed(ImGuiKey_X, false);
    if (HasSelection()) {
      ImGui::SetClipboardText(
          input_.substr(size_t(SelMin()), size_t(SelMax() - SelMin())).c_str());
      if (cut) {
        DeleteSelection();
      }
    }
  }
  if (ctrl && ImGui::IsKeyPressed(ImGuiKey_V, false)) {
    if (const char* clip = ImGui::GetClipboardText(); clip != nullptr) {
      // Paste a single line of printable ASCII: a stray newline would commit
      // nothing useful here, and the box is one line.
      std::string filtered;
      for (const char* p = clip; *p != 0; ++p) {
        const unsigned char ch = static_cast<unsigned char>(*p);
        if (ch == 0x0A || ch == 0x0D) {
          break;
        }
        if (ch >= 0x20 && ch < 0x7F) {
          filtered.push_back(char(ch));
        }
      }
      InsertText(filtered.c_str());
    }
  }

  if (matches_.empty() && committed_ < 0) {
    RebuildMatches();
  }

  // ---- value suggestions for the committed variable ----------------------
  // Rebuilt rather than cached because committing a name below changes which
  // list the suggestion rows draw from: a stale count here would index an
  // empty hints_ list (int/float vars offer no hints at all).
  auto add_hint = [this](const char* value, const char* name) {
    Hint h;
    std::snprintf(h.value, sizeof(h.value), "%s", value);
    std::snprintf(h.name, sizeof(h.name), "%s", name != nullptr ? name : "");
    hints_.push_back(h);
  };
  auto build_hints = [&]() {
    hints_.clear();
    if (committed_ < 0 || committed_ >= int(rows_.size())) {
      return;
    }
    const uint8_t k = rows_[size_t(committed_)].kind;
    if (k == kTypeBool) {
      add_hint("0", "off");
      add_hint("1", "on");
    } else if (k == kTypeEnum) {
      for (const auto& tag : ReadEnumTags(rows_[size_t(committed_)].object)) {
        char num[24];
        std::snprintf(num, sizeof(num), "%d", tag.second);
        add_hint(num, tag.first.c_str());
      }
    }
  };
  build_hints();

  const int list_count =
      committed_ >= 0 ? int(hints_.size()) : int(matches_.size());
  if (selected_ < 0 || selected_ >= list_count) {
    selected_ = 0;
  }
  if (list_count > 0) {
    if (key_down) {
      selected_ = (selected_ + 1) % list_count;
    }
    if (key_up) {
      selected_ = (selected_ + list_count - 1) % list_count;
    }
  }

  if (key_enter) {
    const size_t eq = input_.find('=');
    if (committed_ >= 0 && eq != std::string::npos) {
      std::string val = input_.substr(eq + 1);
      if (val.empty() && selected_ >= 0 &&
          size_t(selected_) < hints_.size()) {
        val = hints_[size_t(selected_)].value;
      }
      if (!val.empty() && committed_ < int(rows_.size())) {
        Apply(rows_[size_t(committed_)], val);
        input_.clear();
        caret_ = 0;
        sel_ = 0;
        committed_ = -1;
        OnInputChanged();
      }
    } else if (!matches_.empty()) {
      if (selected_ < 0 || selected_ >= int(matches_.size())) {
        selected_ = 0;
      }
      const int row = matches_[size_t(selected_)];
      if (row >= 0 && row < int(rows_.size()) &&
          rows_[size_t(row)].label != nullptr) {
        committed_ = row;
        input_ = std::string(rows_[size_t(row)].label) + "=";
        // Caret after the '=' so the value can be typed straight away.
        caret_ = int(input_.size());
        sel_ = caret_;
        selected_ = 0;
        caret_moved_at_ = ImGui::GetTime();
      }
    }
  }
  if (key_esc) {
    if (committed_ >= 0 || !input_.empty()) {
      input_.clear();
      caret_ = 0;
      sel_ = 0;
      committed_ = -1;
      OnInputChanged();
    } else {
      Hide();
      return;
    }
  }

  // committed_ / matches_ may have changed in the handlers above, so the
  // suggestion list has to be re-derived before anything is drawn from it.
  build_hints();
  const int draw_count =
      committed_ >= 0 ? int(hints_.size()) : int(matches_.size());
  if (selected_ < 0 || selected_ >= draw_count) {
    selected_ = 0;
  }

  // ---- input strip -------------------------------------------------------
  const ImVec2 in_min = origin;
  const ImVec2 in_max(origin.x + kWidth, origin.y + kInputH);
  dl->AddRectFilled(in_min, in_max, kInputBg, 6.0f * scale);
  dl->AddRect(in_min, in_max, kBorder, 6.0f * scale, 0, 1.0f * scale);
  // A caret-coloured bar marks the prompt.
  dl->AddRectFilled(ImVec2(in_min.x + 1.0f, in_min.y + 1.0f),
                    ImVec2(in_min.x + 3.0f * scale, in_max.y - 1.0f), kAccent,
                    2.0f * scale);

  const float text_y = in_min.y + (kInputH - kFont) * 0.5f;
  const float text_x = in_min.x + kPad;

  // Cached prefix widths: byte offset -> x and its inverse, so the caret,
  // the selection rect and mouse hit-testing all agree with where the single
  // AddText below actually places each glyph.
  //
  // These are CACHED because measuring them every frame is what broke the
  // console when text editing was added. offset_of_x compared every prefix,
  // so a frame issued O(len) CalcTextSizeA calls; each can lazily bake a
  // glyph, and enough baking grows and REPACKS the atlas
  // (ImFontAtlasTextureRepack), which swaps atlas->TexRef mid-frame while
  // this draw list already holds commands pointing at the old texture.
  // Rebuilt only when the text or the font size changes, so a steady frame
  // measures nothing. Prefixes rather than per-character sums, so kerning
  // cannot make the caret drift from the drawn text.
  float advances[257];
  const int n_chars = int(input_.size()) > 256 ? 256 : int(input_.size());
  if (adv_text_ != input_ || adv_font_size_ != kFont) {
    adv_text_ = input_;
    adv_font_size_ = kFont;
    adv_.assign(size_t(n_chars) + 1, 0.0f);
    for (int i = 1; i <= n_chars; ++i) {
      adv_[size_t(i)] = TextWidth(kFont, input_.substr(0, size_t(i)).c_str());
    }
  }
  advances[0] = text_x;
  for (int i = 1; i <= n_chars; ++i) {
    advances[i] =
        text_x + (size_t(i) < adv_.size() ? adv_[size_t(i)] : adv_.back());
  }
  const auto x_of_offset = [&](int off) {
    const int clamped = off < 0 ? 0 : (off > n_chars ? n_chars : off);
    return advances[clamped];
  };
  const auto offset_of_x = [&](float x) {
    // Nearest character boundary, so clicking a glyph's right half puts the
    // caret after it.
    int best = 0;
    float best_d = std::fabs(x - advances[0]);
    for (int i = 1; i <= n_chars; ++i) {
      const float d = std::fabs(x - advances[i]);
      if (d < best_d) {
        best_d = d;
        best = i;
      }
    }
    return best;
  };

  // ---- mouse: click to place the caret, drag to select, double-click to
  // select a word ---------------------------------------------------------
  const bool over_input = io.MousePos.x >= in_min.x &&
                          io.MousePos.x <= in_max.x &&
                          io.MousePos.y >= in_min.y && io.MousePos.y <= in_max.y;
  if (over_input && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
    const int at = offset_of_x(io.MousePos.x);
    sel_ = word_left(at < int(input_.size()) ? at + 1 : at);
    SetCaret(word_right(at), true);
    dragging_ = false;
  } else if (over_input && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    SetCaret(offset_of_x(io.MousePos.x), io.KeyShift);
    dragging_ = true;
  } else if (dragging_ && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    // Extend while dragging, even once the pointer leaves the box.
    SetCaret(offset_of_x(io.MousePos.x), true);
  }
  if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    dragging_ = false;
  }

  if (input_.empty()) {
    dl->AddText(ImVec2(text_x, text_y), kTextFaint,
                "search a debug variable...");
  } else {
    // Selection behind the glyphs.
    if (HasSelection()) {
      const float sx = x_of_offset(SelMin());
      const float ex = x_of_offset(SelMax());
      dl->AddRectFilled(ImVec2(sx, in_min.y + 4.0f * scale),
                        ImVec2(ex, in_max.y - 4.0f * scale), kSelBg,
                        2.0f * scale);
    }
    // The committed name and the typed value in different colours so the
    // '=' boundary reads at a glance.
    const size_t eq = input_.find('=');
    if (eq == std::string::npos) {
      dl->AddText(ImVec2(text_x, text_y), kText, input_.c_str());
    } else {
      const std::string lhs = input_.substr(0, eq + 1);
      const std::string rhs = input_.substr(eq + 1);
      dl->AddText(ImVec2(text_x, text_y), kTextDim, lhs.c_str());
      if (!rhs.empty()) {
        // lhs is a prefix of input_, so its width is already cached.
        dl->AddText(ImVec2(x_of_offset(int(eq) + 1), text_y), kValue,
                    rhs.c_str());
      }
    }
  }

  // Caret at its real offset. Solid for a moment after any edit or movement,
  // so it does not blink out mid-keystroke, then blinking.
  const double since_move = ImGui::GetTime() - caret_moved_at_;
  if (since_move < 0.4 || std::fmod(float(since_move), 1.0f) < 0.5f) {
    const float cx = x_of_offset(caret_);
    dl->AddRectFilled(ImVec2(cx, text_y),
                      ImVec2(cx + 1.0f * scale + 1.0f, text_y + kFont), kText);
  }

  // ---- suggestion list ---------------------------------------------------
  const int shown = draw_count > 14 ? 14 : draw_count;
  if (shown <= 0) {
    float y = in_max.y + 6.0f * scale;
    // An int/float variable has no value suggestions, so show what it is and
    // where it currently sits instead of an empty panel.
    if (committed_ >= 0 && committed_ < int(rows_.size())) {
      const Row& row = rows_[size_t(committed_)];
      std::string line = std::string(KindLabel(row.kind)) + "  current = " +
                         ReadLiveValue(row.object, row.kind);
      if (row.kind == kTypeInt || row.kind == kTypeFloat) {
        uint32_t lo = 0;
        uint32_t hi = 0;
        if (ReadU32(row.object + kLabelMin, &lo) &&
            ReadU32(row.object + kLabelMax, &hi) && lo != hi) {
          char buf[64];
          if (row.kind == kTypeFloat) {
            std::snprintf(buf, sizeof(buf), "   range %g .. %g",
                          std::bit_cast<float>(lo), std::bit_cast<float>(hi));
          } else {
            std::snprintf(buf, sizeof(buf), "   range %d .. %d", int32_t(lo),
                          int32_t(hi));
          }
          line += buf;
        }
      }
      dl->AddText(ImVec2(in_min.x + kPad, y), kTextDim,
                  line.c_str());
      y += kRow;
      if (!row.desc.empty()) {
        dl->AddText(ImVec2(in_min.x + kPad, y), kTextFaint,
                    row.desc.c_str());
        y += kRow;
      }
    }
    if (!status_.empty()) {
      dl->AddText(ImVec2(in_min.x + kPad, y), kAccent,
                  status_.c_str());
    }
    return;
  }

  // Scroll so the selection stays visible without a scrollbar.
  int first = 0;
  if (selected_ >= shown) {
    first = selected_ - shown + 1;
  }

  const float list_top = in_max.y + 4.0f * scale;
  const float list_h = kRow * float(shown) + kPad;
  dl->AddRectFilled(ImVec2(in_min.x, list_top),
                    ImVec2(in_max.x, list_top + list_h), kListBg, 6.0f * scale);
  dl->AddRect(ImVec2(in_min.x, list_top), ImVec2(in_max.x, list_top + list_h),
              kBorder, 6.0f * scale, 0, 1.0f * scale);

  // `avail` is the real size of the container the rows index, taken straight
  // from the vector rather than from shown/first/selected_ arithmetic -- an
  // earlier version derived the row count before the key handlers ran and
  // then indexed an empty hints_ list after Enter committed a variable.
  // Hover and click only record their intent; the state changes are applied
  // after the loop so the bounds cannot move mid-iteration.
  const int avail =
      committed_ >= 0 ? int(hints_.size()) : int(matches_.size());
  int want_selected = selected_;
  int click_idx = -1;

  const ImVec2 mouse = io.MousePos;
  for (int i = 0; i < shown && i < avail; ++i) {
    const int idx = first + i;
    if (idx < 0 || idx >= avail) {
      continue;
    }
    const float row_y = list_top + kPad * 0.5f + kRow * float(i);
    const ImVec2 r_min(in_min.x + 3.0f * scale, row_y);
    const ImVec2 r_max(in_max.x - 3.0f * scale, row_y + kRow);
    const bool hovered = mouse.x >= r_min.x && mouse.x <= r_max.x &&
                         mouse.y >= r_min.y && mouse.y <= r_max.y;
    if (hovered) {
      want_selected = idx;
    }
    const bool sel = idx == selected_;
    if (sel) {
      dl->AddRectFilled(r_min, r_max, kSelBg, 4.0f * scale);
    }
    const float ty = row_y + (kRow - kFont) * 0.5f;

    if (committed_ >= 0) {
      // Value suggestions: "1    on" / "7    racing_easy". Both strings are
      // fixed-size char arrays inside hints_, so the row draw constructs,
      // copies and destroys nothing -- the string-based version of this
      // branch was the one faulting inside the draw.
      if (size_t(idx) < hints_.size()) {
        const Hint& h = hints_[size_t(idx)];
        if (h.value[0] != 0) {
          dl->AddText(ImVec2(r_min.x + kPad, ty), kValue, h.value);
        }
        if (h.name[0] != 0) {
          dl->AddText(ImVec2(r_min.x + kPad + 48.0f * scale, ty),
                      sel ? kText : kTextDim, h.name);
        }
      }
      if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        click_idx = idx;
      }
    } else {
      if (size_t(idx) >= matches_.size()) {
        continue;
      }
      const int ri = matches_[size_t(idx)];
      if (ri < 0 || ri >= int(rows_.size())) {
        continue;
      }
      const Row& row = rows_[size_t(ri)];
      dl->AddText(ImVec2(r_min.x + kPad, ty),
                  sel ? kText : kTextDim,
                  row.label != nullptr ? row.label : "?");
      // Type and live value, right-aligned. Into a fixed buffer rather than
      // a std::string: this runs for every visible row every frame, and it
      // keeps the row draw free of per-row allocation.
      char cur[40];
      ReadLiveValueInto(row.object, row.kind, cur, sizeof(cur));
      const float cur_w = TextWidth(kFont, cur);
      dl->AddText(ImVec2(r_max.x - kPad - cur_w, ty), kValue, cur);
      const char* kind = KindLabel(row.kind);
      const float kind_w =
          TextWidth(kFont, kind);
      dl->AddText(ImVec2(r_max.x - kPad - cur_w - 16.0f * scale - kind_w, ty),
                  kTextFaint, kind);
      if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        click_idx = idx;
      }
    }
  }

  // Apply what the loop observed, now that nothing is iterating.
  selected_ = want_selected;
  if (click_idx >= 0) {
    if (committed_ >= 0) {
      if (click_idx < int(hints_.size()) &&
          committed_ < int(rows_.size())) {
        Apply(rows_[size_t(committed_)], hints_[size_t(click_idx)].value);
        input_.clear();
        caret_ = 0;
        sel_ = 0;
        committed_ = -1;
        OnInputChanged();
      }
    } else if (click_idx < int(matches_.size())) {
      const int ri = matches_[size_t(click_idx)];
      if (ri >= 0 && ri < int(rows_.size()) &&
          rows_[size_t(ri)].label != nullptr) {
        committed_ = ri;
        input_ = std::string(rows_[size_t(ri)].label) + "=";
        caret_ = int(input_.size());
        sel_ = caret_;
        selected_ = 0;
        caret_moved_at_ = ImGui::GetTime();
      }
    }
  }
  if (selected_ < 0 || selected_ >= (committed_ >= 0
                                         ? int(hints_.size())
                                         : int(matches_.size()))) {
    selected_ = 0;
  }

  // ---- footer ------------------------------------------------------------
  float foot_y = list_top + list_h + 5.0f * scale;
  if (committed_ >= 0 && committed_ < int(rows_.size())) {
    const Row& row = rows_[size_t(committed_)];
    std::string line = row.desc;  // the section path, e.g. /Render/PostFx
    if (!line.empty()) {
      dl->AddText(ImVec2(in_min.x + kPad, foot_y), kTextFaint,
                  line.c_str());
      foot_y += kRow;
    }
  }
  if (!status_.empty()) {
    dl->AddText(ImVec2(in_min.x + kPad, foot_y), kAccent,
                status_.c_str());
  }
}

void GuestConsoleDialog::OnDraw(ImGuiIO& io) {
  if (!visible_) {
    return;
  }
  EnsureRegistry();

  ImFont* ui_font = imgui_drawer()->ui_font();
  ImFont* font = (ui_font != nullptr && ui_font->IsLoaded()) ? ui_font
                                                            : ConsoleFont(io);
  if (font == nullptr) {
    return;  // no atlas yet
  }

  // The console is painted by hand, but the painting is hosted in a real
  // (invisible, inert) ImGui window: text drawn with no current window has
  // no glyph bake behind it. This function owns Begin/End and
  // PushFont/PopFont as a straight-line sequence with no early returns
  // between them, so ImGui's stacks stay balanced without scope guards.
  const ImGuiViewport* vp = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(vp != nullptr ? vp->Size : io.DisplaySize);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  const bool host_open =
      ImGui::Begin("##guest_console_host", nullptr,
                   ImGuiWindowFlags_NoDecoration |
                       ImGuiWindowFlags_NoBackground |
                       ImGuiWindowFlags_NoSavedSettings |
                       ImGuiWindowFlags_NoFocusOnAppearing |
                       ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoMouseInputs |
                       ImGuiWindowFlags_NoBringToFrontOnFocus);
  if (host_open) {
    ImGui::PushFont(font, 15.0f);
    ImDrawList* dl = ImGui::GetWindowDrawList();
    if (dl != nullptr) {
      // TEMPORARY: the row draw in DrawContents faults on a LOCAL
      // std::string's empty(), which means the frame is damaged rather than
      // the data being wrong. Validate the console's own state before and
      // after the call so the corrupting step is identified instead of
      // inferred from whichever local is touched first.
      const size_t pre_rows = rows_.size();
      const size_t pre_names = names_.size();
      const size_t pre_lower = lower_.size();
      const size_t pre_matches = matches_.size();
      const size_t pre_input = input_.size();
      DrawContents(io, dl);
      if (rows_.size() != pre_rows || names_.size() != pre_names ||
          lower_.size() != pre_lower || input_.size() > 512u ||
          caret_ < 0 || caret_ > int(input_.size()) || sel_ < 0 ||
          sel_ > int(input_.size()) ||
          (committed_ >= 0 && committed_ >= int(rows_.size()))) {
        REXLOG_ERROR(
            "guest-console/corrupt: rows {}->{} names {}->{} lower {}->{} "
            "matches {}->{} input {}->{} caret={} sel={} committed={}",
            pre_rows, rows_.size(), pre_names, names_.size(), pre_lower,
            lower_.size(), pre_matches, matches_.size(), pre_input,
            input_.size(), caret_, sel_, committed_);
      }
    }
    ImGui::PopFont();
  }
  ImGui::End();
  ImGui::PopStyleVar(2);
}

}  // namespace skate3
