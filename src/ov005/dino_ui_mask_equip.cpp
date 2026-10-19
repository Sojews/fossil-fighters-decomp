// DinoUiMaskEquip, DinoUiMaskEquipTest, DinoUiMaskEquipMaskButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_resource.h"

#include "m/indicator.h"

#include "game/dino_ui3_parts_cursor.h"

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

// Moves the cursor to the item, and shows the mask's name.
// NONMATCHING: register allocation in the item loop (the list pointer and the item swap registers).
void DinoUiMaskEquipMenu::OnFocus(MMenuItem *item) {
    mUnk104 = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    if (item == mLinkFrom) {
        SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    }
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), item->GetFrame(), 10, -5));
    }
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            u32 index = mPage * 8 + i;
            if (index >= mUnkF0.size()) {
                continue;
            }
            MResource<DinoHeadMaskDefs> defs = gDinoResource->LoadHeadMaskDefs();
            u32 id = (u32)mUnkF0[mPage * 8 + i];
            const DinoHeadMaskDefs *data = defs.Get();
            mName->SetText(std::string(gDinoResource->GetText(
                ((const DinoHeadMaskDef *)((const u8 *)data + ((const u32 *)((const u8 *)data + data->tableOffset))[id]))
                    ->nameText)));
            return;
        }
    }
    mName->SetText(std::string(""));
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

// Hands out the odd-numbered masks, then opens the equip menu.
void DinoUiMaskEquipTest::Idle() {
    for (u32 i = 1; i <= 19; i++) {
        if (i & 1) {
            gDinoBackup->SetHeadMask(i, TRUE);
        }
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiMaskEquip();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

DinoUiMaskEquip::DinoUiMaskEquip() : MProcess(NULL) {
}


DinoUiMaskEquipTest::DinoUiMaskEquipTest() : MProcess(NULL) {
}

#pragma auto_inline off

DinoUiMaskEquipMenuItem::DinoUiMaskEquipMenuItem(DinoUiMaskEquipMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}
