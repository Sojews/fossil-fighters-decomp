// DinoUiItemItemButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
#include "m/smart_oam.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_ui_item_item_button_menu.h"
#include "game/dino_ui_item_item_button.h"
#include "game/dino_ui_item_menu_item.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

DinoUiItemItemButton::~DinoUiItemItemButton() {
}

DinoUiItemMenuItem::~DinoUiItemMenuItem() {
}

DinoUiItemMenu::~DinoUiItemMenu() {
}

DinoUiItemBackButton::~DinoUiItemBackButton() {
}

DinoUiItemOKButton::~DinoUiItemOKButton() {
}

DinoUiItemPageLButton::~DinoUiItemPageLButton() {
}

DinoUiItemPageRButton::~DinoUiItemPageRButton() {
}

MRect DinoUiItemMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiItemMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiItemMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiItemMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiItemMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiItemOKButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiItemBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiItemBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiItemPageRButton::OnDecide() {
    mOwner->PageRight();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiItemPageLButton::OnDecide() {
    mOwner->PageLeft();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiItemOKButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnOKDecided();
    DinoSoundPlaySe(0x9A, 0);
}

void DinoUiItemItemButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

void DinoUiItemMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
}


void DinoUiItemMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    mFraction->SetNumerator(mPage + 1);
    RefreshPage();
    MPoint cursor = GetCursor();
    FocusItem(mUnkB8);
    FocusNearestItem(cursor);
}

void DinoUiItemMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    mFraction->SetNumerator(mPage + 1);
    RefreshPage();
    MPoint cursor = GetCursor();
    FocusItem(mUnkB8);
    FocusNearestItem(cursor);
}

MRect DinoUiItemItemButton::GetFrame() {
    return MRect(mPos.Offset(-80, -8), mPos.Offset(80, 8));
}

extern const int data_02089a20; // fade frames
extern const int data_02089a88;

void DinoUiItemMenu::SelectTab(int tab) {
    if (tab < 0 || tab > 2) {
        return;
    }
    switch (tab) {
    case 0: {
        *mResult = 1;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    case 1: {
        *mResult = 2;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    case 2:
        break;
    }
}

extern const int data_02089a34;
extern const u32 data_0208aa3c; // "the team was changed" text
extern const int data_02089a30;
extern const int data_02089a2c;

void DinoUiItemMenu::OnBackDecided() {
    *mResult = 9;
    mSerial.AddTail(new MTimer(data_02089a34));
    if (DinoTeamDiffers(mOldTeam, mTeam)) {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208aa3c), 4));
    }
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a30, data_02089a2c);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fade);
    serial->AddTail(exit);
}

extern const u32 data_0208aae0;

void DinoUiItemMenu::OnOKDecided() {
    *mResult = 0;
    if (mOldTeam->slots[0]) {
        if (DinoTeamDiffers(mOldTeam, mTeam)) {
            mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208aae0), 4));
        } else {
            mSerial.AddTail(new MTimer(30));
        }
        MTask *fade;
        MTask *exit;
        MTask &self = *this;
        exit = MTaskExitTransfer::Create(&self);
        fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
    } else {
        MTask *timer;
        MTask *fade;
        MTask *exit;
        MTask &self = *this;
        exit = MTaskExitTransfer::Create(&self);
        fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        timer = new MTimer(30);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(fade);
        serial->AddTail(exit);
    }
}

#pragma auto_inline off

DinoUiItemMenuItem::DinoUiItemMenuItem(DinoUiItemMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mMotion->GetUnit()->SetMode(1);
    mParallel.AddTail(mMotion);
}

// The OK button is usable once the team has a first member.
void DinoUiItemMenu::RefreshOKButton() {
    if (mOldTeam->slots[0]) {
        mOKButton->mMotion->GetUnit()->SetMode(1);
    } else {
        mOKButton->mMotion->GetUnit()->SetMode(0);
    }
}
