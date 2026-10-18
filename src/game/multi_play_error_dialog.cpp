// MultiPlayErrorDialog.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "nitro/mi.h"
#include "game/dino_battle.h"
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
    u8 unk0C[0x3C - 0xC];
    void (DinoMpGame::*Unk_3C)();
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

// A wireless battle: player 0 is the parent.
// NONMATCHING: the game keeps the 0 in a register (from the loop counter's start).
void DinoMpGame::StateStart() {
    mParallel.AddTail(mMultiPlay);
    DinoBattleParams params;
    MI_CpuFill8(&params, 0, sizeof(params));
    params.unk00 = 2;
    params.unk04 = (u32)mMultiPlay;
    for (int i = 0; i < 2; i++) {
        (&params.unk08)[i] = i != mMultiPlay->GetPlayerIndex() ? 1 : 0;
    }
    if (mMultiPlay->GetPlayerIndex() != 1) {
        params.unk10 = 0;
        params.unk14 = 1;
    } else {
        params.unk10 = 1;
        params.unk14 = 0;
    }
    params.unk18 = 1;
    params.unk1C = mMultiPlay->GetPlayerIndex() == 0 ? TRUE : FALSE;
    mSerial.AddTail(new DinoBattleDebugSelect(params, &mFailed));
    mState = data_020c02fc.Unk_3C;
}
