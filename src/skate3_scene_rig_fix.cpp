// Old-rig CaS cutscene animations: convert to the skater's skeleton on load.
//
// 24 of this build's 27 player cutscene animations (data/scene/anim/*.abin
// with reference pose CAC_MALE / CAC_FEMALE) were exported on a 133-bone rig
// with an extra PROP part (LEFT_PROP / RIGHT_PROP, bones 66-67). The skater
// model is the 131-bone rig without it (SkaterPresentationDB.abin, identical
// in retail). Sk8::Scene::SceneCharacterAnim builds the pose on the
// database's own skeleton, then pairs it with the model's inverse bind pose
// and matrix palette by bone index, so everything from bone 66 up --
// SKATEBOARD_PROCEDURAL, HELPERBONES (shoulder / upper-arm / thigh twists)
// and the whole face -- lands two slots off: broken shoulders, thighs and
// faces in those cutscenes, original hardware included. Retail re-exported
// these files on the 131-bone rig.
//
// The hook does the same conversion in memory, right before the game parses
// the database: sub_82FFE370 is Andale's DataBase content setup (it walks the
// DataHeader records into the clip / pose / hierarchy maps and runs before
// DataBaseManager::UpdateDatabase registers the part names), with the buffer
// in r6. skate3_abin_prop_fix.h removes the PROP part from the hierarchy,
// the reference pose and the clip. It only touches databases whose rig is a
// CaS rig (CAC_*) carrying a PROP part, so NPC / prop characters (rendered on
// models that have PROP) and the already-correct 131-bone animations are left
// alone; the conversion is idempotent for repeat registrations of a cached
// buffer. Verified offline on every cutscene database of this build with
// SkateNisViewer's `nis_selftest --propfix`: the 24 old-rig files convert to
// exactly the skater rig with all frame data preserved, the other 354 stay
// byte-identical.

#include "generated/sk83_na_m_init.h"

#include <rex/hook.h>
#include <rex/logging.h>

#include <cstdint>

#include "skate3_abin_prop_fix.h"

namespace {

// Upper bound for the record walk; the chain ends at a zero-size record well
// before this (the largest cutscene database is ~2 MB, OnBoard.abin ~10 MB).
constexpr size_t kMaxDatabaseBytes = 64u << 20;

}  // namespace

extern "C" REX_FUNC(sub_82FFE370) {
  const uint32_t buffer = ctx.r6.u32;
  if (buffer != 0) {
    const auto r = sk83::abinfix::RemoveCasPropPart(REX_RAW_ADDR(buffer), kMaxDatabaseBytes);
    if (r.applied) {
      REXLOG_INFO("scene-rig-fix: {} ({}) converted from the {}-bone PROP rig to the skater's {}-bone rig",
                  r.clip.empty() ? "<no clip>" : r.clip, r.name, r.old_bones, r.new_bones);
    }
  }
  __imp__sub_82FFE370(ctx, base);
}
