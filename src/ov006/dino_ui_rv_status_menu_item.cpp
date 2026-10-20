// DinoUiRvStatusMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "game/dino_ui_rv_status_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_ui_field_menu_fraction4.h"
#include "game/dino_ui_rv_information.h"
#include "game/dino_static_init.h"

#pragma auto_inline off

DinoUiRvStatusMenuItem::DinoUiRvStatusMenuItem(DinoUiRvStatusMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

void DinoUiRvStatusMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ShowPage();
}

void DinoUiRvStatusMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ShowPage();
}

#pragma auto_inline on

DinoUiRvStatusMenuItem::~DinoUiRvStatusMenuItem() {
}

DinoUiRvStatusMenu::~DinoUiRvStatusMenu() {
}

DinoUiRvStatusBackButton::~DinoUiRvStatusBackButton() {
}

DinoUiRvStatusPageLButton::~DinoUiRvStatusPageLButton() {
}

DinoUiRvStatusPageRButton::~DinoUiRvStatusPageRButton() {
}

MRect DinoUiRvStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiRvStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiRvStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiRvStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiRvStatusMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiRvStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiRvStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiRvStatusPageRButton::OnDecide() {
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

void DinoUiRvStatusPageLButton::OnDecide() {
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

void DinoUiRvStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 4; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MCallTask0<DinoUiRvStatusMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoHelpBase::*func)());
template class MCallTask1<DinoUiRvStatusMenu, DinoUiRvStatusMenu, u8>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoUiRvStatusMenu::*func)(u8), u8 a0);
#pragma auto_inline reset

// Where the cursor sits on each tab.
extern MPoint data_020b5890[4];

void DinoUiRvStatusMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 4; i++) {
        if (item == mTabItems[i]) {
            frame.MoveTo(data_020b5890[i]);
            mCursor->SetMode(2);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoUiRvStatusMenu::*SetInfoPage_00)(u8);
} data_ov006_021939f0;
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov006_02193a48;

extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov006_02193a10;

// Shows the page's creature, with help back once the information has.
void DinoUiRvStatusMenu::ShowPage() {
    mFraction->SetNumerator(mPage + 1);
    DinoCreatureState creature;
    gDinoBackup->GetCreature(mCreatures[mPage], &creature);
    mInfo->Unk02156ee8(*(DinoCreatureInfo *)&creature);
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov006_02193a10.Enable_00));
    Disable();
    RefreshTab();
}

void DinoUiRvStatusMenu::SelectTab(u8 tab) {
    if (tab > 3 || mTab == tab) {
        return;
    }
    mTab = tab;
    DinoSoundPlaySe(0x94, 0);
    GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov006_021939f0.SetInfoPage_00, tab));
    mTabItems[tab % 4]->SetKeys(0);
    mTabItems[(tab + 1) % 4]->SetKeys(0x400);
    mTabItems[(tab + 2) % 4]->SetKeys(0);
    mTabItems[(tab + 3) % 4]->SetKeys(0x800);
    Disable();
    RefreshTab();
}

void DinoUiRvStatusMenu::SetInfoPage(u8 page) {
    mInfo->SetPage(page);
    if (page == 3) {
        mFraction->SetPriority(3);
    } else {
        mFraction->SetPriority(2);
    }
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov006_02193a48.Enable_00));
    Disable();
}

extern const int data_02089a8c, data_02089a90, data_02089a94;

void DinoUiRvStatusMenu::OnBackDecided() {
    if (mResult != NULL) {
        *mResult = mPage;
    }
    GetSerial()->AddTail(new MTimer(data_02089a94), MTaskSetWaitEmptyTask::Create(mUnkF4),
                         gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(data_02089a90, data_02089a8c),
                         mInfo->CreateWaitModelTask(), MTaskExitTransfer::Create(&(MTask &)*this));
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov006_021939d4[];
extern "C" u32 data_ov006_02193a54[];
static struct DinoUiRvStatusMenuItemStaticInit {
    DinoUiRvStatusMenuItemStaticInit() {
        DINO_POOL_WORD(data_ov006_021939d4, 0x6c) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x70) = 176;
        DINO_POOL_WORD(data_ov006_021939d4, 0x44) = 8;
        DINO_POOL_WORD(data_ov006_021939d4, 0x48) = 150;
        DINO_POOL_WORD(data_ov006_021939d4, 0x14) = 64;
        DINO_POOL_WORD(data_ov006_021939d4, 0x18) = 112;
        DINO_POOL_WORD(data_ov006_02193a54, 0xc) = 19;
        DINO_POOL_WORD(data_ov006_02193a54, 0x10) = 179;
        DINO_POOL_WORD(data_ov006_021939d4, 0x54) = 27;
        DINO_POOL_WORD(data_ov006_021939d4, 0x58) = 0;
        DINO_POOL_WORD(data_ov006_021939d4, 0x34) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x38) = 96;
        DINO_POOL_WORD(data_ov006_021939d4, 0x5c) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x60) = 96;
        DINO_POOL_WORD(data_ov006_021939d4, 0x24) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x28) = 98;
        DINO_POOL_WORD(data_ov006_021939d4, 0x64) = 246;
        DINO_POOL_WORD(data_ov006_021939d4, 0x68) = 24;
        DINO_POOL_WORD(data_ov006_021939d4, 0x2c) = 230;
        DINO_POOL_WORD(data_ov006_021939d4, 0x30) = 24;
    }
} sDinoUiRvStatusMenuItemStaticInit;
