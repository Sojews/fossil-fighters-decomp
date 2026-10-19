// Ov0000215ef04.
// Only the destructors are decompiled so far.

#include "game/dino_battle_view_field_player_bg.h"
#include "types.h"
#include "m/bg_new_task.h"
#include "m/picture.h"

template MTask *MCreateBgNewTask(MTextPicture **result, const char *a1, int thread);

// The pooled constants of the TU (see menu.cpp; dsd splits the block at data_ov000_02191b9c).
extern "C" struct {
    u8 unk00[0x38];
    void (DinoBattleViewFieldPlayerBG::*StateHide_38)();
    u8 unk40[0x60 - 0x40];
    void (DinoBattleViewFieldPlayerBG::*StateDone_60)();
    const char *mPaths[2];      // 0x68: each side's picture
    u8 unk70[0x98 - 0x70];
    void (DinoBattleViewFieldPlayerBG::*StateLoad_98)();
    u8 unkA0[0xA8 - 0xA0];
    void (DinoBattleViewFieldPlayerBG::*StateShow_A8)();
} data_ov000_02191b1c;

// The pictures' offscreen x, by side: {-176, 176}.
extern "C" const int data_ov000_0218fa74[2];

BOOL DinoBattleViewFieldPlayerBG::SetSide(u32 side) {
    if (mFixed != 0) {
        return TRUE;
    }
    if (mChanging) {
        return FALSE;
    }
    if (mSide != side) {
        if (mSide == 2) {
            mState = data_ov000_02191b1c.StateLoad_98;
        } else {
            mState = data_ov000_02191b1c.StateHide_38;
        }
        mSide = side;
        mChanging = TRUE;
    }
    return TRUE;
}

void DinoBattleViewFieldPlayerBG::StateLoad() {
    mSerial.AddTail(MCreateBgNewTask(&mPicture, data_ov000_02191b1c.mPaths[mSide], 1));
    mState = data_ov000_02191b1c.StateShow_A8;
}

void DinoBattleViewFieldPlayerBG::StateShow() {
    mParallel.AddTail(mPicture);
    mPicture->Show();
    mPicture->SetPriority(3);
    mSerial.AddTail(mPicture->CreateCoordAnime(MPoint(data_ov000_0218fa74[mSide], 0), MPoint(0, 0), 40, -2));
    mState = data_ov000_02191b1c.StateDone_60;
}
