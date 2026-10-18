// MultiPlayErrorDialog.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/multi_play_error_dialog_menu.h"
#include "m/multi_play.h"
#include "m/ulti_play_error_dialog.h"

MultiPlayErrorDialog::~MultiPlayErrorDialog() {
}

void MultiPlayErrorDialog::Idle() {
    (this->*mState)();
}

DinoMpGame::~DinoMpGame() {
}

void DinoMpGame::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoMpGame::*Start_04)();
    u8 unk0C[0x44 - 0xC];
    void (MultiPlayErrorDialog::*ErrorStart_44)();
} data_020c02fc;

inline MultiPlayErrorDialog::MultiPlayErrorDialog() : MProcess(NULL) {
    mState = data_020c02fc.ErrorStart_44;
}

DinoMpGame::DinoMpGame(MMultiPlay *multiPlay) : MProcess(NULL), mMultiPlay(multiPlay) {
    mState = data_020c02fc.Start_04;
}

void DinoMpGame::StateFinish() {
    mSerial.AddTail(mMultiPlay->CreateFinishTask());
    if (mFailed) {
        mSerial.AddTail(new MultiPlayErrorDialog);
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}
