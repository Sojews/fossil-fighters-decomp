// DinoCleaningTimer.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_cleaning_timer.h"
#include "types.h"

DinoCleaningTimer::~DinoCleaningTimer() {
}

#pragma auto_inline off

int DinoCleaningTimer::GetRemainingFrames() {
    return mLimit - mFrames;
}

int DinoCleaningTimer::GetSeconds() {
    return mFrames / 60;
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x20];
    void (DinoCleaningTimer::*Done_20)();
} data_ov002_02164c74;

// A second more per frame until the time is up.
void DinoCleaningTimer::StateExtend() {
    DinoSoundFunc_0201feec(0x2E);
    mFrames += 60;
    if (mFrames == mLimit) {
        mState = data_ov002_02164c74.Done_20;
    }
}
