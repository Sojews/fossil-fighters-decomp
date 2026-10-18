// DinoCleaningReport.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_report.h"
#include "game/dino_cleaning.h"
#include "game/dino_backup.h"
#include "types.h"

DinoCleaningReport::~DinoCleaningReport() {
}

void DinoCleaningReport::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x14];
    void (DinoCleaningReport::*State02155b28_14)(); // 0x14
    u8 unk1C[0x34 - 0x1C];
    void (DinoCleaningReport::*State02156808_34)(); // 0x34
} data_ov002_02163e60;

DinoCleaningReport::DinoCleaningReport(u32 arg) : MProcess(NULL), mUnk7C(arg) {
    mMessages = MResource<DinoMessageData>("/msg/msg_cleaning");
    mState = data_ov002_02163e60.State02155b28_14;
}

extern const u32 data_0208acc8, data_0208ad24, data_0208ad28;

// With rocks waiting, asks about them; otherwise ends the report.
void DinoCleaningReport::State0215670c() {
    if (!gDinoBackup->HasInactiveUnk98()) {
        ((u32 *)((DinoCleaning *)mUnk7C)->mUnkA0)[4] = 3;
        Exit();
        return;
    }
    mAnswer = 0;
    ShowMessage(0x11, "", &mAnswer, "", std::string("*") + gDinoResource->GetText(data_0208acc8),
                gDinoResource->GetText(data_0208ad28), gDinoResource->GetText(data_0208ad24));
    mState = data_ov002_02163e60.State02156808_34;
}

// Ends the report with the answer.
void DinoCleaningReport::State02156808() {
    ((u32 *)((DinoCleaning *)mUnk7C)->mUnkA0)[4] = mAnswer == 1 ? 3 : 4;
    Exit();
}
