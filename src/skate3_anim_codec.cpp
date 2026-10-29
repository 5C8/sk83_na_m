// ANIM_CODEC::VBRChannelTypeBlockInfo::PutData -- native replacement.
//
// WHY THIS FUNCTION. The animation codec chain profiled (self time, nested
// codec calls excluded) as:
//     DecompressTwoConsecFrames  83.22%  calls=4159537  avg=6395 cyc
//     UnPackHeaderBits            9.95%  calls=3610973
//     InitPerAnim                 6.47%  calls=3610960
//     DecompressFrame             0.26%  calls=  51007
//     DecompressFrameBlock        0.08%  calls=  51007
//     Vector_UnPackFrameBlock     0.05%  calls=  51007
// ~83% of the chain, ~5.7% of process CPU. DecompressTwoConsecFrames calls
// only PutData -- InitPerAnim/UnPackHeaderBits come from its caller and the
// decompression is inlined -- and PutData was NOT hooked during that profile,
// so its cost sits inside the 83%. Measured: 79,691,776 PutData calls in a
// single session, 6-48 per DecompressTwoConsecFrames invocation.
//
// The work per call is trivial (compute a destination, copy up to 4 floats);
// the cost is emulation -- __savegprlr/__restgprlr, branch dispatch and
// lfs/stfs pairs, 167 recompiled lines for a handful of native instructions.
//
// WHAT IT DOES (recomp.117:41630):
//     putLoc != 0 : dest = mStride*joint + mOffset + putLoc
//     putLoc == 0 : dest = mStride*joint + *(mData + frame*4) + mOffset
//     then copy mDataSize floats (guest unrolls 1..4, general loop above).
//   VBRChannelTypeBlockInfo: mOffset u16@0, mStride u16@2, mData u32@4,
//   mDataSize u8@8.
//
// NOTE for the AnimTest reversing project: in the putLoc == 0 path the guest
// indexes an ARRAY OF POINTERS by frame (rlwinm r8,r29,2,0,29 ; lwzx r9,r8,r9).
// AnimTest's PutData ignores `frame` and treats mData as a direct base, which
// is wrong for that path. The game never takes it -- every captured in-game
// call had putLoc != 0 -- but AnimTest's own offline extraction can.
//
// MEASURED: DecompressTwoConsecFrames went 6395 -> 5365 cycles/call with this
// native PutData in place (same probe, same metric; PutData time included in
// both). So PutData was ~16% of that function, not the bulk -- the remaining
// 5365 cycles are its inlined decompression and distribution logic.
//
// VERIFICATION. The destination formula was checked in-game against the
// guest's own writes: snapshot the source words, let the guest write, confirm
// the guest produced exactly those words at the computed address.
// 79,691,776 calls, 0 mismatches. Copying raw 32-bit words keeps this
// bit-exact (the guest's lfs/stfs float->double->float round trip is exact).
// The mData == 0 case still delegates, so the guest's assert is preserved.
//
// ALSO VERIFIED, retained as a record rather than as code: the PutData
// *distribution walk* inside DecompressTwoConsecFrames was reproduced natively
// and compared call-for-call against the guest -- 12,288 in-block calls, 0
// divergences. It is AnimTest's DecompressFrame loop extended to two frames,
// with fallback/isConst walked ONCE and shared while the two primary pointers
// differ by frame block (frameBlock = frame & 7). The header is *(*(work+0)),
// a double indirection AnimTest does not model. Calls where the two frames
// straddle a block (frame0 & 7 == 7) could not be verified that way: the guest
// decompresses f0's block, distributes it, then decompresses f1's block over
// the same buffer, so reading afterwards sees f1's data for both frames. That
// path needs the buffer snapshotted at each decompress, and is the remaining
// work for a full replacement of DecompressTwoConsecFrames.

#include "generated/sk83_na_m_init.h"

#include <cstdint>

// PutData lives in skate3_anim_probe.cpp while the full decode is under
// verification -- same verified native implementation, kept in one place.
