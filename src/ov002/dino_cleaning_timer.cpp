// DinoCleaningTimer.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_cleaning_timer.h"
#include "types.h"
#include "m/resource.h"
#include "game/dino_resource.h"

DinoCleaningTimer::~DinoCleaningTimer() {
}

#pragma auto_inline off

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x10];
    void (DinoCleaningTimer::*Stopped_10)();
    void (DinoCleaningTimer::*Paused_18)();
    void (DinoCleaningTimer::*Done_20)();
    u8 unk28[0x48 - 0x28];
    void (DinoCleaningTimer::*Running_48)();
} data_ov002_02164c74;

void DinoCleaningTimer::Start() {
    mState = data_ov002_02164c74.Running_48;
}

void DinoCleaningTimer::Stop() {
    DinoSoundPlaySe(0x45, 0);
    mState = data_ov002_02164c74.Stopped_10;
}

void DinoCleaningTimer::Pause() {
    mState = data_ov002_02164c74.Paused_18;
}

// Whether no time has passed yet.
BOOL DinoCleaningTimer::IsAtStart() {
    return mFrames == 0;
}

int DinoCleaningTimer::GetRemainingFrames() {
    return mLimit - mFrames;
}

int DinoCleaningTimer::GetSeconds() {
    return mFrames / 60;
}

// A second more per frame until the time is up.
void DinoCleaningTimer::StateExtend() {
    DinoSoundFunc_0201feec(0x2E);
    mFrames += 60;
    if (mFrames == mLimit) {
        mState = data_ov002_02164c74.Done_20;
    }
}

// (Emits the out-of-line copy of the resource constructor the game emits in this file.)
#pragma dont_inline on
void DinoCleaningTimerMessagesAnchor(std::string path) {
    MResource<DinoMessageData> messages(path);
}
#pragma dont_inline reset

// (Empty; nothing in the game calls it.)
extern "C" void func_ov002_0215c6e0() {
}
