// DinoSeMotion.
// Also DinoWeightedRandom.


#include "game/dino_sound.h"
#include "game/dino_se_motion.h"
#include "game/dino_weighted_random.h"
#include "m/seed_generator.h"

DinoWeightedRandom::DinoWeightedRandom(MRandom *random) : mRandom(random), mTotal(0) {
}

void DinoWeightedRandom::Add(int weight) {
    mTotal += weight;
    mBounds.push_back(reinterpret_cast<void *&>(mTotal));
}

// NONMATCHING: the game reloads the count every pass; ours keeps it in a register.
u32 DinoWeightedRandom::Pick() {
    int value = mRandom->Next(mTotal);
    for (u32 i = 0; i < mBounds.size(); i++) {
        if (value < (int)mBounds[i]) {
            return i;
        }
    }
    return 0;
}

DinoSeMotion::~DinoSeMotion() {
}

// Template instances the game emits in this file.
template class std::__vec_deleter<void *, std::allocator<void *> &>;
template class std::__vec_deleter<void *, std::allocator<void *> >;

DinoSeMotion::DinoSeMotion(const std::string &path, MSmartOam *oam, int flags, int fromEnd)
    : MMotion(std::string(path), oam, flags, fromEnd) {
}

void DinoSeMotion::AddSe(u32 se) {
    mSes.push_back(reinterpret_cast<void *&>(se));
}

void DinoSeMotion::Unk44(int arg) {
    u32 i = arg - 1;
    if (i < mSes.size()) {
        DinoSoundPlaySe((u32)mSes[i], 0);
    }
}
