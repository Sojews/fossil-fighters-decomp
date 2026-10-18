// DinoUi2ContainerBaseMenuItem.
// Only the destructors are decompiled so far.

// The buttons' destructors have the base destructor body inlined; auto-inlining the one defined above does that.
#pragma auto_inline on

#include "game/dino_ui2_parts_rock_name.h"

#include "game/dino_ui2_field_menu_cursor.h"

#include "m/input.h"

#include "game/dino_ui2_parts_kaseki_button.h"

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
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_common_window_message_box.h"

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

template MTask *MCreateCallTask(DinoUi2CannonShootMenu *obj, void (DinoUi2CannonShootMenu::*func)());

u8 DinoUi2ContainerBaseMenu::GetFlag(int index) {
    return mFlags[index];
}

// Entries that can't be marked now (and aren't) are greyed out.
void DinoUi2ContainerBaseMenu::RefreshMarks() {
    for (int i = 0; i < mItems.size(); i++) {
        DinoBackupEntry entry;
        GetEntry(mPage * 8 + i, &entry);
        if (!GetFlag(mPage * 8 + i) && (!IsMarkable(&entry) || !CanMark())) {
            ((DinoUi2PartsKasekiButton *)mItems[i])->SetActive(FALSE);
        } else {
            ((DinoUi2PartsKasekiButton *)mItems[i])->SetActive(TRUE);
        }
    }
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

// X jumps to the decide button; B unmarks the selected entry, or goes to (and then presses) the back button.
void DinoUi2ContainerBaseMenu::OnIdle() {
    mIdled = TRUE;
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi2PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
            return;
        }
        if (mBack->IsFocused()) {
            mBack->Decide();
        } else {
            FocusItem(mBack);
        }
    }
}

// Moves the cursor to the item, and shows the entry's name (marked if it can't be marked now).
void DinoUi2ContainerBaseMenu::OnFocus(MMenuItem *item) {
    mIdled = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
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
            if (index >= mUnk134.size()) {
                continue;
            }
            DinoBackupEntry entry;
            GetEntry(index, &entry);
            if (!GetFlag(mPage * 8 + i) && (!IsMarkable(&entry) || !CanMark())) {
                mRockName->SetEntry(&entry, TRUE);
            } else {
                mRockName->SetEntry(&entry, FALSE);
            }
            mSelected = mPage * 8 + i;
            return;
        }
    }
    mRockName->Clear();
    mSelected = 0xFF;
}

// An entry button: marks it (if it and the container allow) or unmarks it.
void DinoUi2ContainerBaseMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            if (!mIdled) {
                return;
            }
            u32 index = mPage * 8 + i;
            if (index >= mUnk134.size()) {
                return;
            }
            u8 flag = !GetFlag(index);
            if (flag) {
                DinoBackupEntry entry;
                GetEntry(mPage * 8 + i, &entry);
                if (!IsMarkable(&entry)) {
                    DinoSoundPlaySe(0xA4, 0);
                    return;
                }
                if (!CanMark()) {
                    DinoSoundPlaySe(0xA4, 0);
                    mSerial.AddTail(CreateUnk2C());
                    return;
                }
            }
            SetFlag(mPage * 8 + i, flag);
            ((DinoUi2PartsKasekiButton *)mItems[i])->SetMarked(flag);
            return;
        }
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

extern const u32 data_0208a9c4, data_0208aa04, data_0208aa08;

// Asks whether to load the marked fossils.
BOOL DinoUi2CannonShootMenu::TryDecide() {
    if (mUnkFC == 0) {
        DinoSoundPlaySe(0xA4, 0);
        return FALSE;
    }
    mCursor->Hide();
    mConfirmed = FALSE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208a9c4), &mConfirmed);
    dialog->SetLabels(gDinoResource->GetText(data_0208aa08), gDinoResource->GetText(data_0208aa04));
    MTask *finish = MCreateCallTask(this, &DinoUi2CannonShootMenu::FinishDecide);
    MTask *show = mCursor->CreateShowTransfer();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(dialog);
    serial->AddTail(show);
    serial->AddTail(finish);
    return TRUE;
}

extern const u32 data_0208a988; // text: no more can be loaded

MTask *DinoUi2CannonShootMenu::CreateUnk2C() {
    return new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208a988), 4);
}
