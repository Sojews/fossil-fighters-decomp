// DinoBattleViewPlayerField.

#include "m/picture.h"
#include "game/dino_battle_view_field_player_bg.h"
#include "game/dino_battle_view_player_field.h"
#include "m/smart_oam.h"
#include "m/text.h"

DinoBattleViewPlayerField::DinoBattleViewPlayerField(u32 display) {
    mUnk10 = 0;
    mOamOwner = NULL;
    mBg = NULL;
    mText = NULL;
    mDisplay = display;
}

DinoBattleViewPlayerField::~DinoBattleViewPlayerField() {
    if (mBg) {
        mBg->DeleteText(mText);
    }
}

void DinoBattleViewPlayerField::Execute() {
}

extern MPoint data_ov000_02191dc4[]; // the screen origin for each display

void DinoBattleViewPlayerField::SetPosition(MPoint pos) {
    if (mText) {
        MPoint &origin = data_ov000_02191dc4[mDisplay];
        mText->SetOffset(MPoint(origin.x - pos.x, origin.y - pos.y));
    }
    if (mOamOwner) {
        mOamOwner->mOam->SetOffset(MPoint(-pos.x, -pos.y));
    }
    mPos = pos;
}

MPoint DinoBattleViewPlayerField::GetPosition() {
    if (mText) {
        MPoint &origin = data_ov000_02191dc4[mDisplay];
        return MPoint(origin.x - mText->mOffset.x, origin.y - mText->mOffset.y);
    }
    return mPos;
}

void DinoBattleViewPlayerField::SetSize(MPoint size) {
}

MPoint DinoBattleViewPlayerField::GetSize() {
    return MPoint(0, 0);
}

void DinoBattleViewPlayerField::SetPriority(int priority) {
    if (mText) {
        mText->mBgCntBits.priority = priority;
    }
}

int DinoBattleViewPlayerField::GetPriority() {
    if (mText) {
        return mText->mBgCntBits.priority;
    }
    return 0;
}

void DinoBattleViewPlayerField::Show() {
    if (mText) {
        mText->Show();
    }
}

void DinoBattleViewPlayerField::Hide() {
    if (mText) {
        mText->Hide();
    }
}

BOOL DinoBattleViewPlayerField::IsHidden() {
    if (mText) {
        return mText->IsHidden();
    }
    return FALSE;
}

u32 DinoBattleViewPlayerField::GetPlaneMask() {
    if (mText) {
        return 1 << mText->mBg;
    }
    return 0;
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

// The picture is in place.
void DinoBattleViewFieldPlayerBG::StateDone() {
    mChanging = FALSE;
}
