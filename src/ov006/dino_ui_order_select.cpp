// DinoUiOrderSelect.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/blend.h"
#include "game/dino_sound.h"
#include "game/dino_ui_order_select.h"
#include "types.h"
#include "m/call_task.h"
#include "m/input.h"

DinoUiOrderSelect::~DinoUiOrderSelect() {
}

DinoUiOrderSelectMenuItem::~DinoUiOrderSelectMenuItem() {
}

DinoUiOrderSelectBackButton::~DinoUiOrderSelectBackButton() {
}

DinoUiOrderSelectDecideButton::~DinoUiOrderSelectDecideButton() {
}

MRect DinoUiOrderSelectMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOrderSelectMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOrderSelectMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiOrderSelectBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrderSelectMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

const char *DinoUiOrderSelectMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

extern "C" void func_ov006_0218d828(void *owner);

void DinoUiOrderSelectDecideButton::OnDecide() {
    mMotion->Play(3, 1);
    func_ov006_0218d828(mOwner);
    DinoSoundPlaySe(0x98, 0);
}

void DinoUiOrderSelectBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    func_ov006_0218d828(mOwner);
    DinoSoundPlaySe(0x90, 0);
}

// DinoUiOrderSelectMenu's OnStart/OnFocus/OnDecide (not decompiled yet) queue these call tasks.
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoUiOrderSelectMenu::*func)());
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoUiOrderSelectMenu::*func)(bool), bool a1);

#pragma auto_inline off

DinoUiOrderSelectMenu::~DinoUiOrderSelectMenu() {
    if (mPictures[0] != NULL) {
        for (int i = 0; i < 4; i++) {
            delete mPictures[i];
        }
    }
    gCurrentDisplay->mBlend->Reset();
}

void DinoUiOrderSelectMenu::OnIdle() {
    mUnk1F6 = TRUE;
    if (GetInput()->GetPressed() & 0x800) {
        PageLeft();
    } else if (GetInput()->GetPressed() & 0x400) {
        PageRight();
    }
}

void DinoUiOrderSelectMenu::PageLeft() {
    u8 count = mHasFourthTab ? 4 : 3;
    SetPage((mPage + count - 1) % count);
    if (mFocusTabs) {
        FocusItem(mTabs[mPage]);
    }
}

void DinoUiOrderSelectMenu::PageRight() {
    u8 count = mHasFourthTab ? 4 : 3;
    SetPage((mPage + 1) % count);
    if (mFocusTabs) {
        FocusItem(mTabs[mPage]);
    }
}

void DinoUiOrderSelectMenu::FinishPage() {
    mUnk1F0 = 0;
    ClearItems();
    CreateItems();
    RefreshItems();
    Enable();
    RefreshOKButton();
    FocusNearestItem(GetCursor());
}

void DinoUiOrderSelectMenu::FinishTab() {
    RefreshItems();
    FocusItem(mItems[mFocusIndex]);
    RefreshOKButton();
}
