// DinoSeMotion.
// Only the destructor is decompiled so far.

#pragma ipa file

#include "game/dino_sound.h"
#include "game/dino_se_motion.h"

DinoSeMotion::~DinoSeMotion() {
}

// Template instances the game emits in this file.
template class std::__vec_deleter<void *, std::allocator<void *> >;

void DinoSeMotion::Unk44(int arg) {
    u32 i = arg - 1;
    if (i < mSes.size()) {
        DinoSoundPlaySe((u32)mSes[i], 0);
    }
}
