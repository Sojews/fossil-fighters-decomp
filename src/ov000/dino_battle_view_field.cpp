// DinoBattleViewField.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/picture.h"
#include "game/dino_battle_view_field_player_bg.h"
#include "game/dino_battle_view_field.h"
#include "types.h"

DinoBattleViewField::~DinoBattleViewField() {
}

// The player BG's pooled constants (see ov000_0215ef04.cpp).
extern "C" struct {
    u8 unk00[0x88];
    void (DinoBattleViewFieldPlayerBG::*StateLoad_88)();
} data_ov000_02191b1c;
extern "C" const int data_ov000_0218fa74[2];

void DinoBattleViewFieldPlayerBG::StateHide() {
    MTask *exit = MTaskExitTransfer::Create(mPicture);
    MTask *slide = mPicture->CreateCoordAnime(MPoint(0, 0), MPoint(data_ov000_0218fa74[1 - mSide], 0), 40, 2);
    MTaskSet &serial = mSerial;
    serial.AddTail(slide);
    serial.AddTail(exit);
    mState = data_ov000_02191b1c.StateLoad_88;
}
