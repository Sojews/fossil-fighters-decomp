// DinoUi2ContainerBaseMenuItem.
// Only the destructors are decompiled so far.

// The buttons' destructors have the base destructor body inlined; auto-inlining the one defined above does that.
#pragma auto_inline on

#include "m/blend.h"
#include "game/dino_ui2_container_base_menu_item_menu.h"
#include "game/dino_ui2_container_base_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_backup.h"

#pragma auto_inline off

BOOL DinoUi2ContainerBaseMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

#pragma auto_inline on

#pragma auto_inline off

BOOL DinoUi2ContainerBaseMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

#pragma auto_inline on

DinoUi2ContainerBaseMenuItem::~DinoUi2ContainerBaseMenuItem() {
}

DinoUi2ContainerBaseBackButton::~DinoUi2ContainerBaseBackButton() {
}

DinoUi2ContainerBasePageLButton::~DinoUi2ContainerBasePageLButton() {
}

DinoUi2ContainerBasePageRButton::~DinoUi2ContainerBasePageRButton() {
}

DinoUi2ContainerBaseDecideButton::~DinoUi2ContainerBaseDecideButton() {
}

#pragma auto_inline off

DinoUi2ContainerBaseMenu::~DinoUi2ContainerBaseMenu() {
    GetDisplay()->mBlend->Reset();
}

DinoUi2CannonShootMenu::~DinoUi2CannonShootMenu() {
}

#pragma auto_inline on

MRect DinoUi2ContainerBaseMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUi2ContainerBaseMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUi2ContainerBaseMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUi2ContainerBaseMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUi2ContainerBaseMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUi2ContainerBaseBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUi2ContainerBaseBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUi2ContainerBasePageRButton::OnDecide() {
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

void DinoUi2ContainerBasePageLButton::OnDecide() {
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

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUi2CannonShootMenu, DinoUi2CannonShootMenu>;
template MTask *MCreateCallTask(DinoUi2CannonShootMenu *obj, void (DinoUi2CannonShootMenu::*func)());

void DinoUi2ContainerBaseDecideButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mOwner->mUnkFC) {
            mMotion->Play(2, 0);
        } else {
            mMotion->Play(1, 0);
        }
    } else {
        mMotion->Play(1, 0);
        mSerial.Clear();
    }
}

// Pooled file statics: the button corners.
struct DinoUi2ContainerBaseStatics {
    u8 unk00[0x50];
    MPoint mDecideTopLeft;      // 0x50
    u8 unk58[0x20];
    MPoint mDecideBottomRight;  // 0x78
};
extern "C" DinoUi2ContainerBaseStatics data_ov004_02176b44;

MRect DinoUi2ContainerBaseDecideButton::GetFrame() {
    return MRect(data_ov004_02176b44.mDecideTopLeft, data_ov004_02176b44.mDecideBottomRight);
}

void DinoUi2ContainerBaseDecideButton::OnDecide() {
    if (mOwner->TryDecide()) {
        DinoSoundPlaySe(0x98, 0);
        mMotion->Play(3, 1);
        MTask *play = mMotion->CreatePlayTransfer(2, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
}

// NONMATCHING: the game reloads the item count for clear(); ours reuses the loop's last load.
void DinoUi2ContainerBaseMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
    mItems.clear();
}

extern MView *data_ov004_02177284; // the view the cross-fade keeps

void DinoUi2ContainerBaseMenu::OnBackDecided() {
    MTask &self = *this;
    MTask *timer;
    MTask *hide;
    MTask *fade;
    MTask *exit;
    exit = MTaskExitTransfer::Create(&self);
    MBlend *blend = gCurrentDisplay->mBlend;
    fade = blend->CreateCrossFadeOutTask(data_ov004_02177284->GetPlaneMask() ^ 0x3F, 5, 0);
    hide = mOam->CreateHideTransfer();
    timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(hide);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

#pragma auto_inline off

u8 DinoUi2ContainerBaseMenu::GetFlag(int index) {
    return mFlags[index];
}

void DinoUi2ContainerBaseMenu::SetFlag(int index, u8 value) {
    if (value != mFlags[index]) {
        mFlags[index] = value;
        if (value) {
            mUnkFC++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mUnkFC--;
            DinoSoundPlaySe(0x9D, 0);
        }
        RefreshMarks();
    }
}

void DinoUi2CannonShootMenu::GetEntry(u32 index, DinoBackupEntry *entry) {
    gDinoBackup->GetUnk14C(index, entry);
}

BOOL DinoUi2CannonShootMenu::CanMark() {
    return mUnkFC < 10;
}

BOOL DinoUi2CannonShootMenu::IsMarkable(DinoBackupEntry *entry) {
    return TRUE;
}
