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
#include "game/dino_static_init.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "game/dino_guide_message.h"
#include "game/dino_ui_common.h"
#include "m/input.h"
#include "m/smart_oam.h"
#include "m/picture.h"

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

// Lists the rocks in the containers.
void DinoUiContainerMenu::RefreshEntries() {
    mUnk100.clear();
    for (int i = 0; i < 0x40; i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk98(i, &entry);
        if (entry.id) {
            mUnk100.push_back(DinoGetFossilEntry(&entry, mKasekiDefs));
        }
    }
    if (mUnk100.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mUnk100.size() - 1) / 8;
    }
}

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
        RefreshEntries();
        ClearItems();
        RefreshPage2();
        FocusNearestItem(mCursor->GetPosition());
    }
}

void DinoUiContainerMenu::OnDecide(MMenuItem *item) {
    int i;
    for (i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
    for (i = 0; i < (mPage == mLastPage ? mUnk100.size() - mPage * 8 : 8); i++) {
        if (item == mItems[i]) {
            Decide(i);
            return;
        }
    }
}

void DinoUiContainerMenu::ClearItems() {
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
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
    u8 unk00[0x50];
    MPoint mBackPos;    // 0x50: (the OK button's too)
    MPoint mPageLPos;   // 0x58
    MPoint mPageRPos;   // 0x60
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

void DinoUiContainerMenu::OnFocus(MMenuItem *item) {
    int i;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            frame.MoveTo(data_020b5858[i]);
            mCursor->SetMode(2);
        }
    }
    for (i = 0; i < mItems.size(); i++) {
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
    for (i = 0; i < (mPage == mLastPage ? mUnk100.size() - mPage * 8 : 8); i++) {
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

// The TU's pooled paths.
extern "C" struct {
    const char *mPageLPath;     // 0x00
} data_ov006_02191454;
extern "C" struct {
    u8 unk00[0x78];
    const char *mPageRPath;     // 0x78
} data_ov006_021914d4;
extern "C" struct {
    u8 unk00[0x10];
    const char *mBackPath;      // 0x10
    u8 unk14[0x30 - 0x14];
    const char *mOKPath;        // 0x30
} data_ov006_02191554;
extern "C" const MPoint data_ov006_02191664;   // the fraction's position
extern "C" const MPoint data_ov006_0219166c;   // the panel's
extern "C" const char data_ov006_021934a0[];   // the title's picture
extern const u32 data_0208a8e4, data_0208a8e8, data_0208aacc;
extern const u32 data_0208a8f4, data_0208a8f0, data_0208a8ec;
extern const u32 data_0208a900, data_0208a8fc, data_0208a8f8, data_0208a964;
extern const int data_02089a28, data_02089a24;
// The tabs' frames.
extern MRect data_020b58b0, data_020b58c0, data_020b58d0;

inline DinoUiContainerBackButton::DinoUiContainerBackButton(DinoUiContainerMenu *owner)
    : DinoUiContainerMenuItem(owner, data_ov006_02191554.mBackPath, data_ov006_021915d4.mBackPos, data_0208a8e4) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
}

inline DinoUiContainerOKButton::DinoUiContainerOKButton(DinoUiContainerMenu *owner)
    : DinoUiContainerMenuItem(owner, data_ov006_02191554.mOKPath, data_ov006_021915d4.mBackPos, data_0208a8e8) {
    SetKeys(8);
    mMotion->Play(2, 0);
}

inline DinoUiContainerPageLButton::DinoUiContainerPageLButton(DinoUiContainerMenu *owner)
    : DinoUiContainerMenuItem(owner, data_ov006_02191454.mPageLPath, data_ov006_021915d4.mPageLPos, data_0208aacc) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiContainerPageRButton::DinoUiContainerPageRButton(DinoUiContainerMenu *owner)
    : DinoUiContainerMenuItem(owner, data_ov006_021914d4.mPageRPath, data_ov006_021915d4.mPageRPos, data_0208aacc) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

void DinoUiContainerMenu::OnStart() {
    DinoPlayerData player;
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    DinoDimScreen();
    mParallel.AddTail(mTitle = new MTextPicture(std::string(data_ov006_021934a0), TRUE, false));
    mTitle->Show();
    mTitle->SetPriority(2);
    mOam = new MSmartOam(0x50, 0, TRUE);
    mTabItems[0] = new DinoUiPartsTabIconMenuItem(data_0208a8f4, data_020b58b0);
    AddItem(mTabItems[0]);
    mTabItems[1] = new DinoUiPartsTabIconMenuItem(data_0208a8f0, data_020b58c0);
    AddItem(mTabItems[1]);
    mTabItems[2] = new DinoUiPartsTabIconMenuItem(data_0208a8ec, data_020b58d0);
    AddItem(mTabItems[2]);
    mBack = new DinoUiContainerBackButton(this);
    AddItem(mBack);
    mOKButton = new DinoUiContainerOKButton(this);
    AddItem(mOKButton);
    RefreshOKButton();
    DinoHelpDummyItem *team;
    mParallel.AddTail(team = new DinoHelpDummyItem(GetDisplay(), MRect(0x19, 0x20, 0xE4, 0x87),
                                                   std::string(gDinoResource->GetText(data_0208a900))));
    DinoHelpDummyItem *panel;
    mParallel.AddTail(panel = new DinoHelpDummyItem(GetDisplay(), MRect(0xE7, 0x73, 0xFA, 0x8A),
                                                    std::string(gDinoResource->GetText(data_0208a8fc))));
    DinoHelpDummyItem *rocks;
    mParallel.AddTail(rocks = new DinoHelpDummyItem(GetDisplay(), MRect(0xF, 0x92, 0xF0, 0xA4),
                                                    std::string(gDinoResource->GetText(data_0208a8f8))));
    AddEntry((u32)(DinoHelpItem *)panel, 3, (u32)(DinoHelpItem *)team);
    AddEntry((u32)(DinoHelpItem *)rocks, 3, 0);
    AddEntry((u32)(DinoHelpItem *)rocks, 1, 0);
    AddEntry((u32)(DinoHelpItem *)rocks, 2, (u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[2]);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[0], 4, (u32)(DinoHelpItem *)rocks);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[1], 4, (u32)(DinoHelpItem *)rocks);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[2], 4, (u32)(DinoHelpItem *)rocks);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[2], 1, (u32)(DinoHelpItem *)mOKButton);
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    gDinoBackup->CompactUnk98();
    gDinoBackup->GetPlayer(&player, 0);
    RefreshEntries();
    if (mLastPage > 0) {
        mPageL = new DinoUiContainerPageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiContainerPageRButton(this);
        AddItem(mPageR);
    }
    AddLink(mTabItems[2], 1, mOKButton);
    AddLink(mTabItems[0], 3, mBack);
    AddLink(mBack, 1, mTabItems[0]);
    mFraction = new DinoUiFieldMenuFraction(mOam, data_ov006_02191664);
    mParallel.AddTail(mFraction);
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetDenominator(player.containerNum);
    RefreshPage2();
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUiFieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(1);
    mCursor->Hide();
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mPanel = new DinoUiPartsRockName(data_ov006_0219166c);
    mParallel.AddTail(mPanel);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a964))));
    mTabItems[0]->SetKeys(0x800);
    mTabItems[2]->SetKeys(0x400);
    FocusItem(mTabItems[1]);
    *mResult = 0;
    mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(data_02089a28, data_02089a24));
}

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

// Makes the page's buttons (blank past the last rock) and links them in a grid, the top row to the back button and the
// tabs, the OK button and the back button to the first.
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
    AddLink((MMenuItem *)mItems[0], 4, mBack);
    AddLink((MMenuItem *)mItems[1], 4, mBack);
    AddLink((MMenuItem *)mItems[2], 4, mBack);
    AddLink((MMenuItem *)mItems[3], 4, mBack);
    AddLink(mTabItems[0], 2, (MMenuItem *)mItems[0]);
    AddLink(mTabItems[1], 2, (MMenuItem *)mItems[0]);
    AddLink(mTabItems[2], 2, (MMenuItem *)mItems[0]);
    AddLink(mOKButton, 2, (MMenuItem *)mItems[0]);
    AddLink(mBack, 2, (MMenuItem *)mItems[0]);
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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov006_02191754[];
extern "C" u32 data_ov006_021913d4[];
extern "C" u32 data_ov006_021917d4[];
extern "C" u32 data_ov006_02191854[];
extern "C" u32 data_ov006_021918d4[];
extern "C" u32 data_ov006_02191954[];
static struct DinoUiContainerMenuItemStaticInit {
    DinoUiContainerMenuItemStaticInit() {
        DINO_POOL_WORD(data_ov006_021915d4, 0x70) = 128;
        DINO_POOL_WORD(data_ov006_021915d4, 0x74) = 176;
        DINO_POOL_WORD(data_ov006_02191654, 0x30) = 8;
        DINO_POOL_WORD(data_ov006_02191654, 0x34) = 150;
        DINO_POOL_WORD(data_ov006_02191654, 0x38) = 64;
        DINO_POOL_WORD(data_ov006_02191654, 0x3c) = 112;
        DINO_POOL_WORD(data_ov006_02191754, 0x78) = ((s32 *) &data_ov006_021915d4)[0x18 / 4];
        DINO_POOL_WORD(data_ov006_02191754, 0x7c) = ((s32 *) &data_ov006_021915d4)[0x18 / 4];
        DINO_POOL_WORD(data_ov006_021917d4, 0x0) = DINO_POOL_WORD(data_ov006_021913d4, 0xc);
        DINO_POOL_WORD(data_ov006_021917d4, 0x4) = DINO_POOL_WORD(data_ov006_021913d4, 0x58);
        DINO_POOL_WORD(data_ov006_021917d4, 0x8) = DINO_POOL_WORD(data_ov006_02191554, 0x7c);
        DINO_POOL_WORD(data_ov006_021917d4, 0xc) = DINO_POOL_WORD(data_ov006_021914d4, 0x48);
        DINO_POOL_WORD(data_ov006_021917d4, 0x10) = DINO_POOL_WORD(data_ov006_021914d4, 0x58);
        DINO_POOL_WORD(data_ov006_021917d4, 0x14) = DINO_POOL_WORD(data_ov006_021914d4, 0x68);
        DINO_POOL_WORD(data_ov006_021917d4, 0x18) = DINO_POOL_WORD(data_ov006_02191454, 0x78);
        DINO_POOL_WORD(data_ov006_021917d4, 0x1c) = DINO_POOL_WORD(data_ov006_02191554, 0x8);
        DINO_POOL_WORD(data_ov006_021917d4, 0x20) = DINO_POOL_WORD(data_ov006_02191554, 0x18);
        DINO_POOL_WORD(data_ov006_021917d4, 0x24) = DINO_POOL_WORD(data_ov006_02191554, 0x28);
        DINO_POOL_WORD(data_ov006_021917d4, 0x28) = DINO_POOL_WORD(data_ov006_02191554, 0x0);
        DINO_POOL_WORD(data_ov006_021917d4, 0x2c) = DINO_POOL_WORD(data_ov006_02191554, 0x48);
        DINO_POOL_WORD(data_ov006_021917d4, 0x30) = DINO_POOL_WORD(data_ov006_02191554, 0x58);
        DINO_POOL_WORD(data_ov006_021917d4, 0x34) = DINO_POOL_WORD(data_ov006_021913d4, 0x68);
        DINO_POOL_WORD(data_ov006_021917d4, 0x38) = DINO_POOL_WORD(data_ov006_02191554, 0x74);
        DINO_POOL_WORD(data_ov006_021917d4, 0x3c) = DINO_POOL_WORD(data_ov006_021915d4, 0x4);
        DINO_POOL_WORD(data_ov006_021917d4, 0x40) = DINO_POOL_WORD(data_ov006_021915d4, 0x10);
        DINO_POOL_WORD(data_ov006_021917d4, 0x44) = DINO_POOL_WORD(data_ov006_02191454, 0x7c);
        DINO_POOL_WORD(data_ov006_021917d4, 0x48) = DINO_POOL_WORD(data_ov006_021913d4, 0x5c);
        DINO_POOL_WORD(data_ov006_021917d4, 0x4c) = DINO_POOL_WORD(data_ov006_021915d4, 0x20);
        DINO_POOL_WORD(data_ov006_021917d4, 0x50) = DINO_POOL_WORD(data_ov006_021915d4, 0x14);
        DINO_POOL_WORD(data_ov006_021917d4, 0x54) = DINO_POOL_WORD(data_ov006_021915d4, 0x24);
        DINO_POOL_WORD(data_ov006_021917d4, 0x58) = DINO_POOL_WORD(data_ov006_021913d4, 0x4);
        DINO_POOL_WORD(data_ov006_021917d4, 0x5c) = DINO_POOL_WORD(data_ov006_021914d4, 0xc);
        DINO_POOL_WORD(data_ov006_021917d4, 0x60) = DINO_POOL_WORD(data_ov006_021914d4, 0x4);
        DINO_POOL_WORD(data_ov006_021917d4, 0x64) = DINO_POOL_WORD(data_ov006_021913d4, 0x24);
        DINO_POOL_WORD(data_ov006_021917d4, 0x68) = DINO_POOL_WORD(data_ov006_02191554, 0xc);
        DINO_POOL_WORD(data_ov006_021917d4, 0x6c) = DINO_POOL_WORD(data_ov006_021913d4, 0x28);
        DINO_POOL_WORD(data_ov006_021917d4, 0x70) = DINO_POOL_WORD(data_ov006_02191454, 0x48);
        DINO_POOL_WORD(data_ov006_021917d4, 0x74) = DINO_POOL_WORD(data_ov006_021914d4, 0x1c);
        DINO_POOL_WORD(data_ov006_021917d4, 0x78) = DINO_POOL_WORD(data_ov006_021913d4, 0x54);
        DINO_POOL_WORD(data_ov006_021917d4, 0x7c) = DINO_POOL_WORD(data_ov006_021913d4, 0x8);
        DINO_POOL_WORD(data_ov006_02191854, 0x0) = DINO_POOL_WORD(data_ov006_021914d4, 0x24);
        DINO_POOL_WORD(data_ov006_02191854, 0x4) = DINO_POOL_WORD(data_ov006_021914d4, 0x2c);
        DINO_POOL_WORD(data_ov006_02191854, 0x8) = DINO_POOL_WORD(data_ov006_02191454, 0x30);
        DINO_POOL_WORD(data_ov006_02191854, 0xc) = DINO_POOL_WORD(data_ov006_02191454, 0x10);
        DINO_POOL_WORD(data_ov006_02191854, 0x10) = DINO_POOL_WORD(data_ov006_02191454, 0xc);
        DINO_POOL_WORD(data_ov006_02191854, 0x14) = DINO_POOL_WORD(data_ov006_021913d4, 0x18);
        DINO_POOL_WORD(data_ov006_02191854, 0x18) = DINO_POOL_WORD(data_ov006_02191454, 0x28);
        DINO_POOL_WORD(data_ov006_02191854, 0x1c) = DINO_POOL_WORD(data_ov006_021913d4, 0x1c);
        DINO_POOL_WORD(data_ov006_02191854, 0x20) = DINO_POOL_WORD(data_ov006_021914d4, 0x3c);
        DINO_POOL_WORD(data_ov006_02191854, 0x24) = DINO_POOL_WORD(data_ov006_02191454, 0x20);
        DINO_POOL_WORD(data_ov006_02191854, 0x28) = DINO_POOL_WORD(data_ov006_021914d4, 0x4c);
        DINO_POOL_WORD(data_ov006_02191854, 0x2c) = DINO_POOL_WORD(data_ov006_02191454, 0x38);
        DINO_POOL_WORD(data_ov006_02191854, 0x30) = DINO_POOL_WORD(data_ov006_021913d4, 0x2c);
        DINO_POOL_WORD(data_ov006_02191854, 0x34) = DINO_POOL_WORD(data_ov006_02191454, 0x4c);
        DINO_POOL_WORD(data_ov006_02191854, 0x38) = DINO_POOL_WORD(data_ov006_021914d4, 0x6c);
        DINO_POOL_WORD(data_ov006_02191854, 0x3c) = DINO_POOL_WORD(data_ov006_021914d4, 0x5c);
        DINO_POOL_WORD(data_ov006_02191854, 0x40) = DINO_POOL_WORD(data_ov006_021914d4, 0x34);
        DINO_POOL_WORD(data_ov006_02191854, 0x44) = DINO_POOL_WORD(data_ov006_02191554, 0x70);
        DINO_POOL_WORD(data_ov006_02191854, 0x48) = DINO_POOL_WORD(data_ov006_02191454, 0x40);
        DINO_POOL_WORD(data_ov006_02191854, 0x4c) = DINO_POOL_WORD(data_ov006_02191454, 0x3c);
        DINO_POOL_WORD(data_ov006_02191854, 0x50) = DINO_POOL_WORD(data_ov006_02191554, 0x20);
        DINO_POOL_WORD(data_ov006_02191854, 0x54) = DINO_POOL_WORD(data_ov006_021914d4, 0x7c);
        DINO_POOL_WORD(data_ov006_02191854, 0x58) = DINO_POOL_WORD(data_ov006_02191454, 0x50);
        DINO_POOL_WORD(data_ov006_02191854, 0x5c) = DINO_POOL_WORD(data_ov006_02191554, 0x50);
        DINO_POOL_WORD(data_ov006_02191854, 0x60) = DINO_POOL_WORD(data_ov006_02191554, 0x3c);
        DINO_POOL_WORD(data_ov006_02191854, 0x64) = DINO_POOL_WORD(data_ov006_02191554, 0x44);
        DINO_POOL_WORD(data_ov006_02191854, 0x68) = DINO_POOL_WORD(data_ov006_02191554, 0x4c);
        DINO_POOL_WORD(data_ov006_02191854, 0x6c) = DINO_POOL_WORD(data_ov006_02191454, 0x60);
        DINO_POOL_WORD(data_ov006_02191854, 0x70) = DINO_POOL_WORD(data_ov006_02191554, 0x5c);
        DINO_POOL_WORD(data_ov006_02191854, 0x74) = DINO_POOL_WORD(data_ov006_02191454, 0x68);
        DINO_POOL_WORD(data_ov006_02191854, 0x78) = DINO_POOL_WORD(data_ov006_02191554, 0x68);
        DINO_POOL_WORD(data_ov006_02191854, 0x7c) = DINO_POOL_WORD(data_ov006_02191554, 0x6c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x0) = DINO_POOL_WORD(data_ov006_02191454, 0x70);
        DINO_POOL_WORD(data_ov006_021918d4, 0x4) = DINO_POOL_WORD(data_ov006_021913d4, 0x70);
        DINO_POOL_WORD(data_ov006_021918d4, 0x8) = DINO_POOL_WORD(data_ov006_02191454, 0x44);
        DINO_POOL_WORD(data_ov006_021918d4, 0xc) = DINO_POOL_WORD(data_ov006_02191454, 0x2c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x10) = DINO_POOL_WORD(data_ov006_02191454, 0x1c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x14) = DINO_POOL_WORD(data_ov006_02191454, 0x58);
        DINO_POOL_WORD(data_ov006_021918d4, 0x18) = DINO_POOL_WORD(data_ov006_021914d4, 0x40);
        DINO_POOL_WORD(data_ov006_021918d4, 0x1c) = DINO_POOL_WORD(data_ov006_02191454, 0x54);
        DINO_POOL_WORD(data_ov006_021918d4, 0x20) = DINO_POOL_WORD(data_ov006_021913d4, 0x4c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x24) = DINO_POOL_WORD(data_ov006_021913d4, 0x6c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x28) = DINO_POOL_WORD(data_ov006_021913d4, 0x30);
        DINO_POOL_WORD(data_ov006_021918d4, 0x2c) = DINO_POOL_WORD(data_ov006_02191454, 0x18);
        DINO_POOL_WORD(data_ov006_021918d4, 0x30) = DINO_POOL_WORD(data_ov006_02191454, 0x8);
        DINO_POOL_WORD(data_ov006_021918d4, 0x34) = DINO_POOL_WORD(data_ov006_021913d4, 0x3c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x38) = DINO_POOL_WORD(data_ov006_02191454, 0x74);
        DINO_POOL_WORD(data_ov006_021918d4, 0x3c) = DINO_POOL_WORD(data_ov006_021914d4, 0x60);
        DINO_POOL_WORD(data_ov006_021918d4, 0x40) = DINO_POOL_WORD(data_ov006_021914d4, 0x54);
        DINO_POOL_WORD(data_ov006_021918d4, 0x44) = DINO_POOL_WORD(data_ov006_02191454, 0x64);
        DINO_POOL_WORD(data_ov006_021918d4, 0x48) = DINO_POOL_WORD(data_ov006_021913d4, 0x74);
        DINO_POOL_WORD(data_ov006_021918d4, 0x4c) = DINO_POOL_WORD(data_ov006_021913d4, 0x14);
        DINO_POOL_WORD(data_ov006_021918d4, 0x50) = DINO_POOL_WORD(data_ov006_021914d4, 0x0);
        DINO_POOL_WORD(data_ov006_021918d4, 0x54) = DINO_POOL_WORD(data_ov006_021915d4, 0x0);
        DINO_POOL_WORD(data_ov006_021918d4, 0x58) = DINO_POOL_WORD(data_ov006_02191554, 0x2c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x5c) = DINO_POOL_WORD(data_ov006_021914d4, 0x70);
        DINO_POOL_WORD(data_ov006_021918d4, 0x60) = DINO_POOL_WORD(data_ov006_02191554, 0x40);
        DINO_POOL_WORD(data_ov006_021918d4, 0x64) = DINO_POOL_WORD(data_ov006_021915d4, 0x8);
        DINO_POOL_WORD(data_ov006_021918d4, 0x68) = DINO_POOL_WORD(data_ov006_021915d4, 0xc);
        DINO_POOL_WORD(data_ov006_021918d4, 0x6c) = DINO_POOL_WORD(data_ov006_021915d4, 0x1c);
        DINO_POOL_WORD(data_ov006_021918d4, 0x70) = DINO_POOL_WORD(data_ov006_021915d4, 0x28);
        DINO_POOL_WORD(data_ov006_021918d4, 0x74) = DINO_POOL_WORD(data_ov006_021914d4, 0x10);
        DINO_POOL_WORD(data_ov006_021918d4, 0x78) = DINO_POOL_WORD(data_ov006_021913d4, 0x10);
        DINO_POOL_WORD(data_ov006_021918d4, 0x7c) = DINO_POOL_WORD(data_ov006_021913d4, 0x60);
        DINO_POOL_WORD(data_ov006_02191954, 0x0) = DINO_POOL_WORD(data_ov006_021914d4, 0x20);
        DINO_POOL_WORD(data_ov006_02191954, 0x4) = DINO_POOL_WORD(data_ov006_021914d4, 0x28);
        DINO_POOL_WORD(data_ov006_02191954, 0x8) = DINO_POOL_WORD(data_ov006_021913d4, 0x20);
        DINO_POOL_WORD(data_ov006_02191954, 0xc) = DINO_POOL_WORD(data_ov006_021914d4, 0x30);
        DINO_POOL_WORD(data_ov006_02191954, 0x10) = DINO_POOL_WORD(data_ov006_021913d4, 0x40);
        DINO_POOL_WORD(data_ov006_02191954, 0x14) = DINO_POOL_WORD(data_ov006_02191454, 0x34);
        DINO_POOL_WORD(data_ov006_02191954, 0x18) = DINO_POOL_WORD(data_ov006_021913d4, 0x48);
        DINO_POOL_WORD(data_ov006_02191954, 0x1c) = DINO_POOL_WORD(data_ov006_021914d4, 0x44);
        DINO_POOL_WORD(data_ov006_02191954, 0x20) = DINO_POOL_WORD(data_ov006_02191454, 0x24);
        DINO_POOL_WORD(data_ov006_02191754, 0x0) = ((s32 *) &data_ov006_021914d4)[0x64 / 4];
        DINO_POOL_WORD(data_ov006_02191754, 0x4) = ((s32 *) &data_ov006_021914d4)[0x64 / 4];
        DINO_POOL_WORD(data_ov006_02191754, 0x8) = DINO_POOL_WORD(data_ov006_021914d4, 0x74);
        DINO_POOL_WORD(data_ov006_02191754, 0xc) = DINO_POOL_WORD(data_ov006_02191554, 0x4);
        DINO_POOL_WORD(data_ov006_02191754, 0x10) = DINO_POOL_WORD(data_ov006_02191554, 0x14);
        DINO_POOL_WORD(data_ov006_02191754, 0x14) = DINO_POOL_WORD(data_ov006_02191554, 0x24);
        DINO_POOL_WORD(data_ov006_02191754, 0x18) = DINO_POOL_WORD(data_ov006_02191554, 0x34);
        DINO_POOL_WORD(data_ov006_02191754, 0x1c) = DINO_POOL_WORD(data_ov006_02191454, 0x5c);
        DINO_POOL_WORD(data_ov006_02191754, 0x20) = DINO_POOL_WORD(data_ov006_02191554, 0x60);
        DINO_POOL_WORD(data_ov006_02191754, 0x24) = DINO_POOL_WORD(data_ov006_02191554, 0x64);
        DINO_POOL_WORD(data_ov006_02191754, 0x28) = DINO_POOL_WORD(data_ov006_02191454, 0x6c);
        DINO_POOL_WORD(data_ov006_02191754, 0x2c) = DINO_POOL_WORD(data_ov006_02191454, 0x14);
        DINO_POOL_WORD(data_ov006_02191754, 0x30) = DINO_POOL_WORD(data_ov006_02191454, 0x4);
        DINO_POOL_WORD(data_ov006_02191754, 0x34) = DINO_POOL_WORD(data_ov006_021913d4, 0x78);
        DINO_POOL_WORD(data_ov006_02191754, 0x38) = DINO_POOL_WORD(data_ov006_021913d4, 0x64);
        DINO_POOL_WORD(data_ov006_02191754, 0x3c) = DINO_POOL_WORD(data_ov006_02191554, 0x1c);
        DINO_POOL_WORD(data_ov006_02191754, 0x40) = DINO_POOL_WORD(data_ov006_021913d4, 0x34);
        DINO_POOL_WORD(data_ov006_02191754, 0x44) = DINO_POOL_WORD(data_ov006_021913d4, 0x44);
        DINO_POOL_WORD(data_ov006_02191754, 0x48) = DINO_POOL_WORD(data_ov006_02191554, 0x38);
        DINO_POOL_WORD(data_ov006_02191754, 0x4c) = DINO_POOL_WORD(data_ov006_021914d4, 0x8);
        DINO_POOL_WORD(data_ov006_02191754, 0x50) = DINO_POOL_WORD(data_ov006_021913d4, 0x7c);
        DINO_POOL_WORD(data_ov006_02191754, 0x54) = DINO_POOL_WORD(data_ov006_02191554, 0x54);
        DINO_POOL_WORD(data_ov006_02191754, 0x58) = DINO_POOL_WORD(data_ov006_021913d4, 0x38);
        DINO_POOL_WORD(data_ov006_02191754, 0x5c) = DINO_POOL_WORD(data_ov006_02191554, 0x78);
        DINO_POOL_WORD(data_ov006_02191754, 0x60) = DINO_POOL_WORD(data_ov006_021915d4, 0x2c);
        DINO_POOL_WORD(data_ov006_02191754, 0x64) = DINO_POOL_WORD(data_ov006_021914d4, 0x18);
        DINO_POOL_WORD(data_ov006_02191754, 0x68) = DINO_POOL_WORD(data_ov006_021914d4, 0x14);
        DINO_POOL_WORD(data_ov006_02191754, 0x6c) = DINO_POOL_WORD(data_ov006_021913d4, 0x50);
        DINO_POOL_WORD(data_ov006_02191754, 0x70) = DINO_POOL_WORD(data_ov006_021914d4, 0x38);
        DINO_POOL_WORD(data_ov006_02191754, 0x74) = DINO_POOL_WORD(data_ov006_021914d4, 0x50);
        DINO_POOL_WORD(data_ov006_021915d4, 0x38) = 19;
        DINO_POOL_WORD(data_ov006_021915d4, 0x3c) = 179;
        DINO_POOL_WORD(data_ov006_021915d4, 0x40) = 27;
        DINO_POOL_WORD(data_ov006_021915d4, 0x44) = 0;
        DINO_POOL_WORD(data_ov006_021915d4, 0x50) = 128;
        DINO_POOL_WORD(data_ov006_021915d4, 0x54) = 96;
        DINO_POOL_WORD(data_ov006_021915d4, 0x58) = 128;
        DINO_POOL_WORD(data_ov006_021915d4, 0x5c) = 96;
        DINO_POOL_WORD(data_ov006_021915d4, 0x60) = 128;
        DINO_POOL_WORD(data_ov006_021915d4, 0x64) = 96;
        DINO_POOL_WORD(data_ov006_021915d4, 0x68) = 53;
        DINO_POOL_WORD(data_ov006_021915d4, 0x6c) = 60;
        DINO_POOL_WORD(data_ov006_021915d4, 0x78) = 50;
        DINO_POOL_WORD(data_ov006_021915d4, 0x7c) = 0;
        DINO_POOL_WORD(data_ov006_02191654, 0x0) = 0;
        DINO_POOL_WORD(data_ov006_02191654, 0x4) = 50;
        DINO_POOL_WORD(data_ov006_02191654, 0x8) = -32;
        DINO_POOL_WORD(data_ov006_02191654, 0xc) = 4;
        DINO_POOL_WORD(data_ov006_02191654, 0x10) = 241;
        DINO_POOL_WORD(data_ov006_02191654, 0x14) = 128;
        DINO_POOL_WORD(data_ov006_02191654, 0x18) = 26;
        DINO_POOL_WORD(data_ov006_02191654, 0x1c) = 149;
    }
} sDinoUiContainerMenuItemStaticInit;
