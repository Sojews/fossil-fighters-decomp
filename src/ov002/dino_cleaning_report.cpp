// DinoCleaningReport.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_report.h"
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
} data_ov002_02163e60;

DinoCleaningReport::DinoCleaningReport(u32 arg) : MProcess(NULL), mUnk7C(arg) {
    mMessages = MResource<DinoMessageData>("/msg/msg_cleaning");
    mState = data_ov002_02163e60.State02155b28_14;
}
