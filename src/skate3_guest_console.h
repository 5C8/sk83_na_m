#pragma once

// Guest debug-variable console (grave/tilde key).
//
// Skate 3 registers ~2500 debug variables through its cVar*Ref constructors
// at static-init time -- the same values the game's own debug menu edits and
// that startup.ini pre-seeds. Each registration leaves a cVar instance in
// guest memory laid out as:
//
//   +0x00 vtable   +0x04 value   +0x08 category*   +0x0C name*
//
// so a variable can be read and written LIVE by poking `object + 4`.
// tools/extract_guest_cvars.py recovers every instance's address from the
// recompiled static initializers (see src/generated/skate3_guest_cvars.h);
// this console searches them by name and applies edits in place.
//
// Usage: press the console key, type to filter (fuzzy, ranked), pick a
// match with the arrow keys or mouse to commit "<name>=" to the input, then
// type a value and press Enter. Bools and enums offer their own value
// suggestions, so a toggle needs no typing at all.

#include <rex/ui/imgui_dialog.h>

#include <cstdint>
#include <string>
#include <vector>

struct ImDrawList;

namespace skate3 {

// A console edit, queued for the guest thread.
//
// Values do NOT live in the cVar objects: the game's Set_cVar*Ref walks a
// keyed registry and writes a byte at +205 of the record it finds, then runs
// the variable's change callback. Poking the object directly changes nothing
// the game reads, so the console calls the game's OWN setter instead -- and
// that has to happen on a guest thread (GuestToHostFunction needs a live
// ThreadState), not on the UI thread the dialog draws from.
struct PendingSet {
  uint32_t object;   // the cVar instance (the setter's `this`)
  uint8_t kind;      // guest_cvars::Kind
  float value;       // float payload; integral kinds use the truncation
  uint32_t word = 0; // the exact value word to store (float bits / int)
};

// Queue an edit (UI thread) / apply everything queued (guest thread).
void QueueGuestCVarSet(const PendingSet& set);
void ApplyPendingGuestCVarSets(uint8_t* base);

class GuestConsoleDialog final : public rex::ui::ImGuiDialog {
 public:
  explicit GuestConsoleDialog(rex::ui::ImGuiDrawer* drawer)
      : ImGuiDialog(drawer) {}

  void Show();
  void Hide();
  void Toggle();
  bool visible() const { return visible_; }

 protected:
  void OnDraw(ImGuiIO& io) override;

 private:
  // Paints the console. OnDraw owns the host window and the font push; this
  // only draws, so its early returns are safe.
  void DrawContents(ImGuiIO& io, ImDrawList* dl);

  // One row of the live registry: the generated entry plus the name read
  // back from guest memory (authoritative -- one initializer can register
  // several cVars, so the generated label may belong to a sibling).
  struct Row {
    const char* label;  // resolved name
    uint32_t object;
    uint8_t kind;
    std::string desc;
  };

  void EnsureRegistry();
  void Apply(const Row& row, const std::string& value);
  void RebuildMatches();

  bool visible_ = false;
  bool registry_built_ = false;
  bool focus_input_ = false;
  std::vector<Row> rows_;
  // Names resolved from guest memory, and their lowercase forms for the
  // fuzzy filter. Separate storage so Row::label can point at a stable
  // string once the vector has stopped growing.
  std::vector<std::string> names_;
  std::vector<std::string> lower_;
  std::vector<int> matches_;   // indices into rows_
  int selected_ = 0;
  std::string input_;
  std::string status_;
  // Set once a name has been committed to the input ("<name>="), so the
  // suggestion list switches from names to that variable's values.
  int committed_ = -1;

  // Text-edit state for the input box (byte offsets into input_).
  // caret_ is where typing inserts; sel_ is the other end of the selection,
  // so caret_ == sel_ means no selection and the pair is unordered (a
  // selection can be dragged or shift-extended in either direction).
  // Value suggestions for the committed variable, as fixed-size rows rather
  // than std::string: the per-row draw reads these every frame, and the
  // name-row branch that uses plain char buffers has never been implicated
  // in the draw faults the string-based version hit.
  struct Hint {
    char value[24];  // the number to apply, e.g. "0" / "7"
    char name[48];   // the tag's name, e.g. "off" / "racing_easy"
  };
  std::vector<Hint> hints_;

  // Cached prefix widths for the input text (see DrawContents): measuring
  // these every frame hammered CalcTextSizeA, whose lazy glyph baking can
  // repack the font atlas mid-frame.
  std::vector<float> adv_;
  std::string adv_text_;
  float adv_font_size_ = 0.0f;

  int caret_ = 0;
  int sel_ = 0;
  // True while the mouse is down inside the box, extending a drag-selection.
  bool dragging_ = false;
  // Time of the last caret movement, so the caret shows solid while typing
  // instead of blinking out mid-keystroke.
  double caret_moved_at_ = 0.0;

  // Replace the selection (or insert at the caret) and keep the filter in
  // sync; the single place input_ is edited.
  void InsertText(const char* text);
  void DeleteSelection();
  bool HasSelection() const { return caret_ != sel_; }
  int SelMin() const { return caret_ < sel_ ? caret_ : sel_; }
  int SelMax() const { return caret_ < sel_ ? sel_ : caret_; }
  // Re-filter after input_ changed, and clamp the caret into range.
  void OnInputChanged();
  void SetCaret(int pos, bool extend_selection);
};

}  // namespace skate3
