// DinoCleaningStatus.
// Only the destructors are decompiled so far.

#pragma ipa file

#include "game/dino_cleaning_status.h"
#include "game/dino_cleaning_timer.h"
#include "game/dino_cleaning_score.h"
#include "m/smart_oam.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/picture.h"

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

// An 8-byte-element vector the game emits members of in this file.
#pragma auto_inline off
template class std::__vec_deleter<DinoCleaningStatusEntry, std::allocator<DinoCleaningStatusEntry> >;

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x04];
    const char *mTimerFramePath;    // 0x04
    const char *mFramePath;         // 0x08
    void (DinoCleaningStatus::*StateIdle_0C)();     // 0x0C
    void (DinoCleaningStatus::*StatePaused_14)();   // 0x14
} data_ov002_021649cc;

// The status bar on the top screen: its frames, the score and the timer.
DinoCleaningStatus::DinoCleaningStatus(DinoCleaning *cleaning, u32 frames, u32 unk, u8 flag)
    : MProcess(gLcd->GetTopDisplay()) {
    MDisplayScope scope(GetDisplay());
    mOam = new MSmartOam(30, 0, TRUE);
    mOam->SetUnk1C(7);
    mFrame = new MTextPicture(std::string(data_ov002_021649cc.mFramePath), 1, true);
    mFrame->Show();
    mFrame->SetPriority(3);
    mParallel.AddTail(mFrame);
    mTimerFrame = new MTextPicture(std::string(data_ov002_021649cc.mTimerFramePath), 1, true);
    mTimerFrame->Show();
    mTimerFrame->SetPriority(2);
    mParallel.AddTail(mTimerFrame);
    mScore = new DinoCleaningScore(cleaning, mOam, unk, flag);
    mParallel.AddTail(mScore);
    mTimer = new DinoCleaningTimer(cleaning, mTimerFrame->GetPlaneMask(), mOam, frames);
    mParallel.AddTail(mTimer);
    mParallel.AddTail(mOam);
    mState = data_ov002_021649cc.StateIdle_0C;
}

void DinoCleaningStatus::StateIdle() {
}

void DinoCleaningStatus::StatePause() {
    mTimer->Pause();
    mState = data_ov002_021649cc.StatePaused_14;
}

void DinoCleaningStatus::StatePaused() {
    GetSerial()->AddTail(mOam->CreateHideTransfer(), new MTimer(15), mOam->CreateShowTransfer(), new MTimer(15));
}

