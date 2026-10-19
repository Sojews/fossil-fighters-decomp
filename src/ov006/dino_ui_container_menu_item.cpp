// DinoUiContainerMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_ui_fossil_entry.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_ui_container_menu_item_menu.h"
#include "game/dino_ui_parts_rock_name.h"
#include "game/dino_ui_parts_kaseki_button.h"
#include "game/dino_ui_container_menu_item.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

#pragma auto_inline off
DinoUiContainerMenu::DinoUiContainerMenu(int *choice, DinoTeam *team, DinoTeam *team2)
    : MMenu(gLcd->GetBottomDisplay(), NULL, 0), mResult(choice), mOldTeam(team), mTeam(team2), mUnk11C(0), mUnk120(0),
      mPage(0), mLastPage(0) {
}
#pragma auto_inline on

DinoUiContainerMenuItem::~DinoUiContainerMenuItem() {
}

DinoUiContainerMenu::~DinoUiContainerMenu() {
}

DinoUiContainerBackButton::~DinoUiContainerBackButton() {
}

DinoUiContainerOKButton::~DinoUiContainerOKButton() {
}

DinoUiContainerPageLButton::~DinoUiContainerPageLButton() {
}

DinoUiContainerPageRButton::~DinoUiContainerPageRButton() {
}

void DinoUiContainerMenu::OnIdle() {
}

MRect DinoUiContainerMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerOKButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiContainerPageRButton::OnDecide() {
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

void DinoUiContainerPageLButton::OnDecide() {
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

void DinoUiContainerOKButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnOKDecided();
    DinoSoundPlaySe(0x9A, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MCallTask0<DinoUiContainerMenu, DinoUiContainerMenu>;
template MTask *MCreateCallTask(DinoUiContainerMenu *obj, void (DinoUiContainerMenu::*func)());
#pragma auto_inline reset

BOOL DinoUiContainerMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(mCursor->GetPosition());
    return TRUE;
}

BOOL DinoUiContainerMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(mCursor->GetPosition());
    return TRUE;
}

extern "C" void func_ov006_021759b8(DinoUiContainerMenu *menu);

void DinoUiContainerMenu::FinishDiscard() {
    if (mUnk120 == 1) {
        if (mLastPage > 0) {
            if (mPage >= 2 && mPage * 8 + 1 == gDinoBackup->FindFreeUnk98()) {
                mPage--;
                mFraction->SetNumerator(mPage + 1);
            } else if (gDinoBackup->FindFreeUnk98() == 9) {
                mPageL->MMenuItem::Disable();
                mPageL->mMotion->Hide();
                mPageR->MMenuItem::Disable();
                mPageR->mMotion->Hide();
                mPage = 0;
                mFraction->SetNumerator(mPage + 1);
            }
        }
        DinoBackupEntry entry;
        MI_CpuFill8(&entry, 0, sizeof(entry));
        gDinoBackup->SetUnk98(mUnk11C, &entry);
        gDinoBackup->CompactUnk98();
        func_ov006_021759b8(this);
        ClearItems();
        RefreshPage2();
        FocusNearestItem(mCursor->GetPosition());
    }
}

void DinoUiContainerMenu::ClearItems() {
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

// NONMATCHING: register allocation only (the game keeps the item index in r1, Decide's argument register).
void DinoUiContainerMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
    for (int i = 0; i < (mPage == mLastPage ? mUnk100.size() - mPage * 8 : 8); i++) {
        if (item == mItems[i]) {
            Decide(i);
            return;
        }
    }
}

extern const int data_02089a20; // fade frames
extern const int data_02089a88;

void DinoUiContainerMenu::SelectTab(int tab) {
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
    case 1:
        break;
    case 2: {
        *mResult = 3;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    }
}

// Where the entry's menu goes (filled in at start), and the TU's pooled constants around it.
extern "C" struct {
    u8 unk00[0x68];
    int x;      // 0x68: the first entry's
    int y;      // 0x6C
    u8 unk70[0x78 - 0x70];
    int colX;   // 0x78: to the next in a row
    int colY;   // 0x7C
} data_ov006_021915d4;
extern "C" struct {
    int rowX;   // 0x00: to the next row
    int rowY;   // 0x04
    MPoint menuOffset;  // 0x08: of the menu from the entry
} data_ov006_02191654;
extern "C" struct {
    void (DinoUiContainerMenu::*FinishDiscard_00)();
} data_ov006_02191674;
extern const u32 data_0208aac4, data_0208a8e0, data_0208aac0, data_0208a8dc;

#pragma auto_inline off
// Asks whether to discard the entry (index on the page).
void DinoUiContainerMenu::Decide(int index) {
    mCursor->Hide();
    mUnk11C = mPage * 8 + index;
    mUnk120 = -2;
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(
        &mUnk120,
        MPoint(data_ov006_021915d4.x + data_ov006_021915d4.colX * (index % 4) + data_ov006_02191654.rowX * (index / 4),
               data_ov006_021915d4.y + data_ov006_021915d4.colY * (index % 4) + data_ov006_02191654.rowY * (index / 4)) +
            data_ov006_02191654.menuOffset,
        NULL, 0);
    menu->AddChoice(1, gDinoResource->GetText(data_0208aac4), gDinoResource->GetText(data_0208a8e0));
    menu->AddChoice(-2, gDinoResource->GetText(data_0208aac0), gDinoResource->GetText(data_0208a8dc));
    GetSerial()->AddTail(menu, mCursor->CreateShowTransfer(),
                         MCreateCallTask(this, data_ov006_02191674.FinishDiscard_00));
}
#pragma auto_inline reset

extern const int data_02089a34;
extern const u32 data_0208aa3c; // "the team was changed" text
extern const int data_02089a30;
extern const int data_02089a2c;

void DinoUiContainerMenu::OnBackDecided() {
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

extern const u32 data_0208aae0; // "the team will be saved" text

void DinoUiContainerMenu::OnOKDecided() {
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

// Where the cursor sits on each tab.
extern MPoint data_020b5858[3];
extern "C" void func_ov003_02159a8c(void *panel, DinoBackupEntry *entry, int arg);

// NONMATCHING: register allocation only (i and mPage swap in the last loop, as in OnDecide).
void DinoUiContainerMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            frame.MoveTo(data_020b5858[i]);
            mCursor->SetMode(2);
        }
    }
    for (int i = 0; i < mItems.size(); i++) {
        if (item == mItems[i]) {
            frame.top += 16;
            frame.bottom += 16;
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
    for (int i = 0; i < (mPage == mLastPage ? mUnk100.size() - mPage * 8 : 8); i++) {
        if (item == mItems[i]) {
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98(mPage * 8 + i, &entry);
            func_ov003_02159a8c(mPanel, &entry, 0);
            return;
        }
    }
    mPanel->Clear();
}

#pragma auto_inline off

DinoUiContainerMenuItem::DinoUiContainerMenuItem(DinoUiContainerMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mMotion->GetUnit()->SetMode(1);
}

// The OK button is usable once the team has a first member.

void DinoLinkGrid(MMenu *menu, std::vector<MMenuItem *> *items, MMenuItem *below);

// Makes the page's buttons (blank past the last rock) and links them in a grid, the top row to mUnkB8 and the tabs, the
// OK button and mUnkB8 to the first.
void DinoUiContainerMenu::RefreshPage2() {
    mItems.clear();
    for (int i = 0; i < 8; i++) {
        DinoUiPartsKasekiButton *button;
        if (mUnk100.size() > i + mPage * 8) {
            button = new DinoUiPartsKasekiButton(
                mOam, MPoint(data_ov006_021915d4.x + data_ov006_021915d4.colX * (i % 4) + data_ov006_02191654.rowX * (i / 4),
                       data_ov006_021915d4.y + data_ov006_021915d4.colY * (i % 4) + data_ov006_02191654.rowY * (i / 4)),
                (const DinoKasekiButtonInfo &)mUnk100[i + mPage * 8], 0);
            mItems.push_back(button);
            AddItem(button);
        } else {
            DinoKasekiButtonInfo empty;
            empty.icon = 1;
            empty.color = 0;
            empty.type = 0;
            empty.level = 0;
            button = new DinoUiPartsKasekiButton(
                mOam, MPoint(data_ov006_021915d4.x + data_ov006_021915d4.colX * (i % 4) + data_ov006_02191654.rowX * (i / 4),
                       data_ov006_021915d4.y + data_ov006_021915d4.colY * (i % 4) + data_ov006_02191654.rowY * (i / 4)),
                empty, 0);
            button->HideMotion();
            mItems.push_back(button);
            AddItem(button);
        }
    }
    DinoLinkGrid(this, (std::vector<MMenuItem *> *)&mItems, mTabItems[1]);
    AddLink((MMenuItem *)mItems[0], 4, (MMenuItem *)mUnkB8);
    AddLink((MMenuItem *)mItems[1], 4, (MMenuItem *)mUnkB8);
    AddLink((MMenuItem *)mItems[2], 4, (MMenuItem *)mUnkB8);
    AddLink((MMenuItem *)mItems[3], 4, (MMenuItem *)mUnkB8);
    AddLink(mTabItems[0], 2, (MMenuItem *)mItems[0]);
    AddLink(mTabItems[1], 2, (MMenuItem *)mItems[0]);
    AddLink(mTabItems[2], 2, (MMenuItem *)mItems[0]);
    AddLink(mOKButton, 2, (MMenuItem *)mItems[0]);
    AddLink((MMenuItem *)mUnkB8, 2, (MMenuItem *)mItems[0]);
}

void DinoUiContainerMenu::RefreshOKButton() {
    if (mOldTeam->slots[0]) {
        mOKButton->mMotion->GetUnit()->SetMode(1);
    } else {
        mOKButton->mMotion->GetUnit()->SetMode(0);
    }
}

#pragma auto_inline off
// The fossil list's vector members the game emits in this file (made explicitly until the code that fills the list is
// decompiled).
template void std::__vec_deleter<DinoUiFossilEntry, std::allocator<DinoUiFossilEntry> >::sub_push_back(const DinoUiFossilEntry &x, Metrowerks::int2type<true>);
