// DinoCleaningStatus.
// Only the destructors are decompiled so far.

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
