// DinoUiMaskEquip, DinoUiMaskEquipTest, DinoUiMaskEquipMaskButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "game/dino_ui_mask_equip_menu.h"
#include "game/dino_ui_mask_equip.h"
#include "game/dino_ui_mask_equip_test.h"
#include "game/dino_ui_mask_equip_mask_button.h"
#include "game/dino_ui_mask_equip_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

DinoUiMaskEquip::~DinoUiMaskEquip() {
}

DinoUiMaskEquipTest::~DinoUiMaskEquipTest() {
}

DinoUiMaskEquipMaskButton::~DinoUiMaskEquipMaskButton() {
}

DinoUiMaskEquipMenuItem::~DinoUiMaskEquipMenuItem() {
}

DinoUiMaskEquipMenu::~DinoUiMaskEquipMenu() {
}

DinoUiMaskEquipBackButton::~DinoUiMaskEquipBackButton() {
}

DinoUiMaskEquipPageLButton::~DinoUiMaskEquipPageLButton() {
}

DinoUiMaskEquipPageRButton::~DinoUiMaskEquipPageRButton() {
}

void DinoUiMaskEquipMenuItem::OnSelectChanged(BOOL value) {
}

MRect DinoUiMaskEquipMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMaskEquipMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMaskEquipMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMaskEquipMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiMaskEquipMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMaskEquipBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiMaskEquipBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMaskEquipPageRButton::OnDecide() {
    if (mOwner->PageRight()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiMaskEquipPageLButton::OnDecide() {
    if (mOwner->PageLeft()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiMaskEquipMenu::OnBackDecided() {
    MTask *fade;
    MTask *exit;
    MTask &self = *this;
    exit = MTaskExitTransfer::Create(&self);
    fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMaskEquipMenu, DinoUiMaskEquipMenu>;
template MTask *MCreateCallTask(DinoUiMaskEquipMenu *obj, void (DinoUiMaskEquipMenu::*func)());

void DinoUiMaskEquipMaskButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(1, 0);
        mMotion->Show();
    } else {
        mMotion->Play(1, 0);
        mMotion->Hide();
    }
}

void DinoUiMaskEquipMaskButton::OnDecide() {
    mMotion->Show();
}

void DinoUiMaskEquipMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            int index = i + mPage * 8;
            if (index < mUnkF0.size()) {
                if (mUnk104) {
                    Decide(index);
                }
                return;
            }
        }
    }
}

MRect DinoUiMaskEquipMaskButton::GetFrame() {
    return MRect(mPos.Offset(-24, -24), mPos.Offset(24, 24));
}

BOOL DinoUiMaskEquipMenu::PageRight() {
    if (mPage == mLastPage) {
        mPage = 0;
    } else {
        mPage++;
    }
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

BOOL DinoUiMaskEquipMenu::PageLeft() {
    if (mPage == 0) {
        mPage = mLastPage;
    } else {
        mPage--;
    }
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

// NONMATCHING: the game reloads the item count for clear(); ours reuses the loop's last load.
void DinoUiMaskEquipMenu::ClearItems() {
    RemoveLink(mLinkFrom, 2);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
    mItems.clear();
}

void DinoUiMaskEquipMenu::Decide(int index) {
    mChoice = index;
    Equip();
}

void DinoUiMaskEquipMenu::OnIdle() {
    mUnk104 = TRUE;
}

void DinoUiMaskEquip::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->VSyncNow();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiMaskEquipMenu();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}
