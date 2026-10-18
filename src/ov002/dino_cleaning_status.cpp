// DinoCleaningStatus.
// Only the destructors are decompiled so far.

#pragma ipa file

#include "game/dino_cleaning_status.h"
#include "game/dino_cleaning_timer.h"
#include "game/dino_cleaning_score.h"

DinoCleaningStatus::~DinoCleaningStatus() {
}

void DinoCleaningStatus::Idle() {
    (this->*mState)();
}

DinoCleaningTimer *DinoCleaningStatus::GetTimer() {
    return mTimer;
}

DinoCleaningScore *DinoCleaningStatus::GetScore() {
    return mScore;
}

void DinoCleaningStatus::Start() {
    mTimer->Start();
    mScore->Show();
}

void DinoCleaningStatus::Stop() {
    mTimer->Stop();
    mScore->SetUnk9CC();
}

// An 8-byte-element vector the game emits members of in this file (the element type is unknown).
struct DinoCleaningStatusEntry {
    u32 unk0;
    u32 unk4;
};
#pragma auto_inline off
template class std::__vec_deleter<DinoCleaningStatusEntry, std::allocator<DinoCleaningStatusEntry> >;
