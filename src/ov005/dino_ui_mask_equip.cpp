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
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_static_init.h"

#pragma auto_inline off
DinoUiMaskEquipMenu::DinoUiMaskEquipMenu() : MMenu(NULL, 0), mChoice(-1), mAnswer(0), mPage(0), mLastPage(0), mUnk104(0) {
}
#pragma auto_inline on

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
#pragma auto_inline off
template class MCallTask0<DinoUiMaskEquipMenu, DinoUiMaskEquipMenu>;
template MTask *MCreateCallTask(DinoUiMaskEquipMenu *obj, void (DinoUiMaskEquipMenu::*func)());
#pragma auto_inline reset

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

void DinoUiMaskEquipMenu::ClearItems() {
    RemoveLink(mLinkFrom, 2);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
    mItems.clear();
}

// The TU's pooled constants.
extern "C" struct {
    MPoint WindowPos_00;                            // 0x00: the yes/no window's
    u8 unk08[0x10 - 0x08];
    void (DinoUiMaskEquipMenu::*FinishEquip_10)();  // 0x10
} data_ov005_0216baa0;

extern "C" const u32 data_0208a9bc, data_0208a9c0;

// Asks whether to put the chosen mask on.
void DinoUiMaskEquipMenu::Equip() {
    if (mChoice == -1) {
        return;
    }
    mCursor->Hide();
    mAnswer = 0;
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(&mAnswer, data_ov005_0216baa0.WindowPos_00, NULL, 0);
    menu->AddChoice(1, gDinoResource->GetText(data_0208a9c0), gDinoResource->GetText(data_0208a9c0));
    menu->AddChoice(-2, gDinoResource->GetText(data_0208a9bc), gDinoResource->GetText(data_0208a9bc));
    GetSerial()->AddTail(menu, MCreateCallTask(this, data_ov005_0216baa0.FinishEquip_10), mCursor->CreateShowTransfer());
}

void DinoUiMaskEquipMenu::FinishEquip() {
    if (mAnswer == 1) {
        DinoPlayerData player;
        gDinoBackup->GetPlayer(&player, 0);
        player.unk14_0 = (u32)mUnkF0[mChoice];
        gDinoBackup->SetPlayer(&player);
        mChoice = -1;
        GetSerial()->AddTail(gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0), MTaskExitTransfer::Create(&(MTask &)*this));
    }
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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov005_0216ba34[];
extern "C" u32 data_ov005_0216bbb4[];
extern "C" u32 data_ov005_0216bab4[];
extern "C" u32 data_ov005_0216bc34[];
// NONMATCHING: register choice/scheduling of the pooled stores.
static struct DinoUiMaskEquipStaticInit {
    DinoUiMaskEquipStaticInit() {
        DINO_POOL_WORD(data_ov005_0216bbb4, 0x68) = DINO_POOL_WORD(data_ov005_0216ba34, 0x38);
        DINO_POOL_WORD(data_ov005_0216bbb4, 0x6c) = DINO_POOL_WORD(data_ov005_0216ba34, 0x58);
        DINO_POOL_WORD(data_ov005_0216bbb4, 0x70) = DINO_POOL_WORD(data_ov005_0216ba34, 0x48);
        DINO_POOL_WORD(data_ov005_0216bbb4, 0x74) = DINO_POOL_WORD(data_ov005_0216ba34, 0x8);
        DINO_POOL_WORD(data_ov005_0216bbb4, 0x78) = DINO_POOL_WORD(data_ov005_0216ba34, 0x10);
        DINO_POOL_WORD(data_ov005_0216bbb4, 0x7c) = DINO_POOL_WORD(data_ov005_0216ba34, 0x4c);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x0) = DINO_POOL_WORD(data_ov005_0216ba34, 0x24);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x4) = DINO_POOL_WORD(data_ov005_0216ba34, 0x44);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x8) = DINO_POOL_WORD(data_ov005_0216ba34, 0x34);
        DINO_POOL_WORD(data_ov005_0216bc34, 0xc) = DINO_POOL_WORD(data_ov005_0216ba34, 0x5c);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x10) = DINO_POOL_WORD(data_ov005_0216ba34, 0x60);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x14) = DINO_POOL_WORD(data_ov005_0216ba34, 0x18);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x18) = DINO_POOL_WORD(data_ov005_0216ba34, 0x54);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x1c) = DINO_POOL_WORD(data_ov005_0216ba34, 0x40);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x20) = DINO_POOL_WORD(data_ov005_0216ba34, 0x4);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x24) = DINO_POOL_WORD(data_ov005_0216ba34, 0xc);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x28) = DINO_POOL_WORD(data_ov005_0216ba34, 0x28);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x2c) = DINO_POOL_WORD(data_ov005_0216ba34, 0x2c);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x30) = DINO_POOL_WORD(data_ov005_0216ba34, 0x3c);
        DINO_POOL_WORD(data_ov005_0216bc34, 0x34) = DINO_POOL_WORD(data_ov005_0216ba34, 0x1c);
        DINO_POOL_WORD(data_ov005_0216bab4, 0x4) = 128;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x8) = 96;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x34) = 128;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x38) = 96;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x14) = 128;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x18) = 96;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x4c) = 53;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x50) = 35;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x54) = 50;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x58) = 0;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x5c) = 0;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x60) = 50;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x64) = -32;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x68) = 4;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x6c) = 241;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x70) = 103;
        DINO_POOL_WORD(data_ov005_0216ba34, 0x64) = 128;
        DINO_POOL_WORD(data_ov005_0216ba34, 0x68) = 96;
        DINO_POOL_WORD(data_ov005_0216ba34, 0x6c) = 128;
        DINO_POOL_WORD(data_ov005_0216ba34, 0x70) = 96;
        DINO_POOL_WORD(data_ov005_0216ba34, 0x74) = 24;
        DINO_POOL_WORD(data_ov005_0216ba34, 0x78) = 144;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x3c) = 156;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x40) = 144;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x44) = 36;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x48) = 174;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x1c) = 136;
        DINO_POOL_WORD(data_ov005_0216bab4, 0x20) = 174;
    }
} sDinoUiMaskEquipStaticInit;
