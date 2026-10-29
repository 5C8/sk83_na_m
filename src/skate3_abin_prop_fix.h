// Converts this build's 133-bone CaS cutscene animations to the skater's
// 131-bone rig, in place, before the game parses them.
//
// 24 of the 27 player cutscene databases (data/scene/anim/*.abin, reference
// pose CAC_MALE / CAC_FEMALE) were exported on a skeleton with an extra PROP
// part -- LEFT_PROP / RIGHT_PROP, bones 66-67 -- that the skater's runtime rig
// (SkaterPresentationDB.abin, 131 bones, identical in retail) does not have.
// SceneCharacterAnim builds the pose on the database's own skeleton and then
// pairs it with the skater model's inverse bind pose and palette by bone
// index, so every bone from 66 up (SKATEBOARD_PROCEDURAL, HELPERBONES, all
// face parts) lands two slots off: broken shoulders, thighs and face. Retail
// re-exported these files on the 131-bone rig; this does the same conversion
// in memory: drop the PROP part from the hierarchy (parents, bone order
// table, part records, bone names), from the reference pose and from the
// clip, and renumber the parts after it.
//
// Header-only and free of runtime dependencies: it works on the raw
// big-endian bytes, so the same code runs on guest memory (the hook) and on
// files (SkateNisViewer's nis_selftest --propfix verifies it).

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace sk83::abinfix {

namespace detail {

inline uint32_t Be32(const uint8_t* p) {
  return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | p[3];
}
inline uint16_t Be16(const uint8_t* p) { return uint16_t((p[0] << 8) | p[1]); }
inline void Put32(uint8_t* p, uint32_t v) {
  p[0] = uint8_t(v >> 24);
  p[1] = uint8_t(v >> 16);
  p[2] = uint8_t(v >> 8);
  p[3] = uint8_t(v);
}
inline void Put16(uint8_t* p, uint16_t v) {
  p[0] = uint8_t(v >> 8);
  p[1] = uint8_t(v);
}

// Util::FastString: 4-byte words, each base-38 encoding up to 6 characters.
inline std::string DecodeFastString(const uint8_t* p, int words) {
  static const char kChars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_";
  std::string out;
  for (int w = 0; w < words; ++w) {
    uint32_t v = Be32(p + 4 * w);
    uint32_t seed = 79235168;
    while (v && seed) {
      const uint32_t i = v / seed;
      if (i == 0 || i > 37) return out;
      out += kChars[i - 1];
      v %= seed;
      seed /= 38;
    }
  }
  return out;
}

struct Record {
  uint8_t* p;
  uint32_t size;
  uint32_t type;
};

// The DataHeader chain (Andale::DataBase content): u32 size, u32 type, ...,
// name (FastString<36>) at +16, payload 16-aligned at +0x30. A type-1 header
// first means the payload starts at +0x30.
inline bool Walk(uint8_t* db, size_t max_size, std::vector<Record>& out) {
  if (db == nullptr || max_size < 0x30) return false;
  size_t p = Be32(db + 4) == 1 ? 0x30 : 0;
  while (p + 0x30 <= max_size) {
    const uint32_t size = Be32(db + p), type = Be32(db + p + 4);
    if (size == 0) return true;
    if ((size & 0xF) || size < 0x30 || type < 2 || type > 11 || p + size > max_size) return false;
    out.push_back({db + p, size, type});
    p += size;
  }
  return true;
}

// Drops the parts-table entry whose part header names skeleton part
// `removed`, renumbers later parts. `count` entries of 4 bytes at `table`,
// offsets relative to `base`. Returns the new count.
inline uint32_t DropPart(uint8_t* base, uint8_t* table, uint32_t count, uint32_t removed, size_t limit) {
  uint32_t out = 0;
  for (uint32_t i = 0; i < count; ++i) {
    const uint32_t entry = Be32(table + 4 * i);
    const uint32_t off = entry & 0xFFFFFF;
    if (off + 4 > limit) return count;  // malformed: leave it alone
    uint8_t* part = base + off;
    uint32_t header = Be32(part);
    const uint32_t k = (header >> 1) & 0x1F;
    if (k == removed) continue;
    if (k > removed) {
      header = (header & ~0x3Eu) | ((k - 1) << 1);
      Put32(part, header);
    }
    Put32(table + 4 * out++, entry);
  }
  for (uint32_t i = out; i < count; ++i) Put32(table + 4 * i, 0);
  return out;
}

}  // namespace detail

struct Result {
  bool applied = false;
  std::string name;   // hierarchy (rig) name, e.g. CAC_MALE
  std::string clip;   // first clip name
  int old_bones = 0, new_bones = 0;
};

// Converts the database at `db` if it is a CaS rig with a PROP part.
// `max_size` bounds the record walk. Idempotent: a converted (or any
// PROP-less) database is left untouched.
inline Result RemoveCasPropPart(uint8_t* db, size_t max_size) {
  using namespace detail;
  Result r;
  std::vector<Record> recs;
  if (!Walk(db, max_size, recs)) return r;

  Record* hier = nullptr;
  for (auto& rec : recs) {
    if (rec.type == 4 && !hier) hier = &rec;
  }
  if (!hier) return r;
  r.name = DecodeFastString(hier->p + 16, 6);
  for (auto& rec : recs) {
    if (rec.type == 2 && r.clip.empty()) r.clip = DecodeFastString(rec.p + 16, 6);
  }
  // Only the CaS skater rigs (CAC_MALE / CAC_FEMALE): NPC and prop
  // characters with a PROP part are rendered on models that have it.
  if (r.name.compare(0, 4, "CAC_") != 0) return r;

  uint8_t* b = hier->p + 0x30;
  const size_t hier_bytes = hier->size - 0x30;
  const uint32_t nb = Be16(b);
  const uint32_t np = Be32(b + 4);
  const size_t parts_at = 8 + 8 * size_t(nb), names_at = parts_at + 36 * size_t(np);
  if (nb == 0 || np == 0 || np > 31 || names_at + 20 * size_t(nb) > hier_bytes) return r;

  uint32_t prop = np, first = 0, count = 0;
  for (uint32_t i = 0; i < np; ++i) {
    const uint8_t* rec = b + parts_at + 36 * i;
    if (DecodeFastString(rec, 6) == "PROP") {
      prop = i;
      count = Be32(rec + 24);
      first = Be32(rec + 28);
    }
  }
  if (prop == np || count == 0 || first + count > nb) return r;
  r.old_bones = int(nb);

  // --- hierarchy: rebuild every per-bone table without [first, first+count).
  auto map_bone = [&](int32_t j) -> int32_t {
    if (j < int32_t(first)) return j;
    if (j < int32_t(first + count)) return -2;  // removed
    return j - int32_t(count);
  };
  std::vector<uint8_t> parents, order, parts, names;
  for (uint32_t j = 0; j < nb; ++j) {
    if (map_bone(int32_t(j)) == -2) continue;
    const int32_t parent = int32_t(Be32(b + 8 + 4 * j));
    const int32_t mp = parent < 0 ? parent : map_bone(parent);
    if (mp == -2) return r;  // something hangs off a PROP bone: not the expected rig
    uint8_t w[4];
    Put32(w, uint32_t(mp));
    parents.insert(parents.end(), w, w + 4);
    names.insert(names.end(), b + names_at + 20 * j, b + names_at + 20 * j + 20);
  }
  for (uint32_t j = 0; j < nb; ++j) {  // evaluation order: drop + remap values
    const int32_t v = map_bone(int32_t(Be32(b + 8 + 4 * nb + 4 * j)));
    if (v == -2) continue;
    uint8_t w[4];
    Put32(w, uint32_t(v));
    order.insert(order.end(), w, w + 4);
  }
  for (uint32_t i = 0; i < np; ++i) {
    if (i == prop) continue;
    const uint8_t* rec = b + parts_at + 36 * i;
    uint8_t copy[36];
    std::memcpy(copy, rec, 36);
    const uint32_t f = Be32(copy + 28);
    if (f >= first + count) Put32(copy + 28, f - count);
    parts.insert(parts.end(), copy, copy + 36);
  }
  const uint32_t new_nb = nb - count, new_np = np - 1;
  if (parents.size() != 4 * new_nb || order.size() != 4 * new_nb) return r;

  // --- pose and clip records: drop the PROP part's entry, renumber.
  for (auto& rec : recs) {
    uint8_t* base = rec.p + 0x30;
    const size_t limit = rec.size - 0x30;
    if (rec.type == 3) {
      const uint32_t n = Be32(base);
      if (n == 0 || 4 + 4 * size_t(n) > limit) continue;
      Put32(base, DropPart(base, base + 4, n, prop, limit));
    } else if (rec.type == 2) {
      const uint32_t w = Be32(base + 0x34);
      const uint32_t n = (w >> 8) & 0x1F;
      if (n == 0 || 0x38 + 4 * size_t(n) > limit) continue;
      const uint32_t left = DropPart(base, base + 0x38, n, prop, limit);
      if (left == n) continue;  // this clip had no PROP data
      // Bits 16+ hold 4 * (bones the clip covers - 1): one PROP bone less each.
      uint32_t upper = w >> 16;
      upper = upper >= 4 * count ? upper - 4 * count : upper;
      Put32(base + 0x34, (upper << 16) | (w & 0xE000u) | (left << 8) | (w & 0xFFu));
    }
  }

  // Write the hierarchy back in its own (now oversized) record.
  Put16(b, uint16_t(new_nb));
  Put32(b + 4, new_np);
  size_t at = 8;
  std::memcpy(b + at, parents.data(), parents.size());
  at += parents.size();
  std::memcpy(b + at, order.data(), order.size());
  at += order.size();
  std::memcpy(b + at, parts.data(), parts.size());
  at += parts.size();
  std::memcpy(b + at, names.data(), names.size());
  at += names.size();
  std::memset(b + at, 0, hier_bytes - at);

  r.applied = true;
  r.new_bones = int(new_nb);
  return r;
}

}  // namespace sk83::abinfix
