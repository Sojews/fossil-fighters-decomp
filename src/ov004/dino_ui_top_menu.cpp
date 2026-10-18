// DinoUiTopMenu.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_top_menu_loop.h"
#include "game/dino_backup.h"
#include "game/help.h"
#include "game/dino_ui_top_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_top_menu_item2.h"
#include "game/dino_ui_top_menu_item.h"
#include "game/dino_sound.h"
#include "game/dino_ui_save_slot_select.h"
#include "game/dino_ui_save_slot_select_menu_delete_item.h"
#include "game/dino_ui_ending.h"
#include "game/dino_resource.h"
#include "game/dino_ui_save_slot_confirm.h"
#include "game/dino_ui_save_slot_select_menu_confirm_delete_task.h"
#include "game/dino_ui_save_slot_select_menu_item.h"
#include "game/dino_ui_save_slot_select_menu_cancel_item.h"
#include "game/dino_ui_save_slot_select_menu.h"

DinoUiTopMenuLoop::~DinoUiTopMenuLoop() {
}

MRect DinoUiTopMenuItem2::GetRect() {
    return GetFrame();
}

MRect DinoUiTopMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiTopMenuItem2::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiTopMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiTopMenuBackButton::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiTopMenuItem2::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiTopMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiTopMenuLoop::Idle() {
    (this->*mState)();
}

void DinoUiTopMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiTopMenuBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiTopMenuBackButton::OnDecide() {
    DinoSoundPlaySe(0x38F, 0);
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
}

void DinoUiTopMenuSingleButton::OnDecide() {
    DinoSoundPlaySe(0x390, 0);
    mMotion->Play(3, 1);
    mOwner->OnSingleDecided();
}

void DinoUiTopMenuMultiButton::OnDecide() {
    DinoSoundPlaySe(0x390, 0);
    mMotion->Play(3, 1);
    mOwner->OnMultiDecided();
}

void DinoUiTopMenuPassButton::OnDecide() {
    DinoSoundPlaySe(0x390, 0);
    mMotion->Play(3, 1);
    mMotion2->Play(3, 1);
    mOwner->OnPassDecided();
}

void DinoUiTopMenuPassDarkButton::OnDecide() {
    DinoSoundPlaySe(0xA4, 0);
}

void DinoUiTopMenuOptionButton::OnDecide() {
    DinoSoundPlaySe(0x390, 0);
    mMotion->Play(3, 1);
    mMotion2->Play(3, 1);
    mOwner->OnOptionDecided();
}

void DinoUiTopMenuMultiDarkButton::OnDecide() {
    DinoSoundPlaySe(0xA4, 0);
}

void DinoUiTopMenuItem2::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
        mMotion2->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
        mMotion2->Play(1, 0);
    }
}

void DinoUiTopMenu::OnFocus(MMenuItem *item) {
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    if (!mSilentFocus) {
        DinoSoundPlaySe(0x38E, 0);
    }
    mSilentFocus = FALSE;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBackup>;
template MTask *MCreateBgCallTask(DinoBackup *obj, void (DinoBackup::*func)(), int thread);
template class MCallTask0<DinoUiTopMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiTopMenu *obj, void (DinoHelpBase::*func)());

void DinoUiTopMenu::OnBackDecided() {
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
    *mResult = 0;
}

void DinoUiTopMenu::OnSingleDecided() {
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
    *mResult = 1;
}

void DinoUiTopMenu::OnMultiDecided() {
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
    *mResult = 2;
}

void DinoUiTopMenu::OnPassDecided() {
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
    *mResult = 3;
}

void DinoUiTopMenu::OnOptionDecided() {
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
    *mResult = 4;
}

DinoUiTopMenu::~DinoUiTopMenu() {
}

DinoUiTopMenuItem::~DinoUiTopMenuItem() {
}

DinoUiTopMenuBackButton::~DinoUiTopMenuBackButton() {
}

DinoUiTopMenuSingleButton::~DinoUiTopMenuSingleButton() {
}

DinoUiTopMenuMultiButton::~DinoUiTopMenuMultiButton() {
}

DinoUiTopMenuMultiDarkButton::~DinoUiTopMenuMultiDarkButton() {
}

DinoUiTopMenuItem2::~DinoUiTopMenuItem2() {
}

DinoUiTopMenuPassButton::~DinoUiTopMenuPassButton() {
}

DinoUiTopMenuPassDarkButton::~DinoUiTopMenuPassDarkButton() {
}

DinoUiTopMenuOptionButton::~DinoUiTopMenuOptionButton() {
}

MRect DinoUiSaveSlotSelectMenuDeleteItem::GetFrame() {
    return mMotion->GetFrame();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoUiSaveSlotSelect>;
template MTask *MCreateBgCallTask(DinoUiSaveSlotSelect *obj, void (DinoUiSaveSlotSelect::*func)(), int thread);

DinoUiSaveSlotSelect::~DinoUiSaveSlotSelect() {
}

DinoUiSaveSlotConfirm::~DinoUiSaveSlotConfirm() {
}

DinoUiSaveSlotSelectMenuConfirmDeleteTask::~DinoUiSaveSlotSelectMenuConfirmDeleteTask() {
}

DinoUiSaveSlotSelectMenuItem::~DinoUiSaveSlotSelectMenuItem() {
}

DinoUiSaveSlotSelectMenuDeleteItem::~DinoUiSaveSlotSelectMenuDeleteItem() {
}

DinoUiSaveSlotSelectMenuCancelItem::~DinoUiSaveSlotSelectMenuCancelItem() {
}

DinoUiSaveSlotSelectMenu::~DinoUiSaveSlotSelectMenu() {
}

DinoUiSaveSlotConfirmMenu::~DinoUiSaveSlotConfirmMenu() {
}

MRect DinoUiSaveSlotSelectMenuCancelItem::GetRect() {
    return GetFrame();
}

MRect DinoUiSaveSlotSelectMenuDeleteItem::GetRect() {
    return GetFrame();
}

MRect DinoUiSaveSlotSelectMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiSaveSlotSelectMenuCancelItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiSaveSlotSelectMenuDeleteItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiSaveSlotSelectMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiSaveSlotSelectMenuCancelItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiSaveSlotSelectMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiSaveSlotSelectMenuDeleteItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiSaveSlotSelectMenuCancelItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

DinoUiEnding::~DinoUiEnding() {
}

void DinoUiEnding::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiOpening {};
template class MCallTask1<DinoUiSaveSlotSelectMenu, DinoUiSaveSlotSelectMenu, u32>;
template MTask *MCreateCallTask(DinoUiSaveSlotSelectMenu *obj, void (DinoUiSaveSlotSelectMenu::*func)(u32), u32 a0);
template class MCallTask0<DinoUiSaveSlotSelectMenu, DinoUiSaveSlotSelectMenu>;
template MTask *MCreateCallTask(DinoUiSaveSlotSelectMenu *obj, void (DinoUiSaveSlotSelectMenu::*func)());
template class MBgCallTask0<DinoUiOpening>;
template MTask *MCreateBgCallTask(DinoUiOpening *obj, void (DinoUiOpening::*func)(), int thread);

void DinoUiSaveSlotSelectMenu::OnFocus(MMenuItem *item) {
    if (mSilentFocus) {
        mSilentFocus = FALSE;
    } else {
        DinoSoundPlaySe(0x38E, 0);
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
}

void DinoUiSaveSlotSelectMenuCancelItem::OnDecide() {
    MTask *wait1 = mMotion->CreateWaitStopTask();
    MTask *play1 = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait2 = mMotion->CreateWaitStopTask();
    MTask *play2 = mMotion->CreatePlayTransfer(4, 0);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(play2);
    serial->AddTail(wait2);
    serial->AddTail(play1);
    serial->AddTail(wait1);
}

void DinoUiSaveSlotSelectMenuDeleteItem::OnDecide() {
    mMotion->Play(3, 1);
}

void DinoUiSaveSlotSelectMenuItem::OnDecide() {
    mMotion->Play(3, 1);
    if (!mUnkA4) {
        mMotion2->Play(3, 1);
        mMotion3->Play(3, 1);
    }
}

extern const u32 data_0208ad94; // a text number
const char *DinoUiSaveSlotSelectMenuDeleteItem::GetText() {
    return gDinoResource->GetText(data_0208ad94);
}

void DinoUiSaveSlotConfirm::Idle() {
    mMenu = new DinoUiSaveSlotConfirmMenu();
    mSerial.AddTail(mMenu);
}

extern u8 data_ov004_02177280;      // set when the screen saves rather than loads
extern const u32 data_0208ad88;
extern const u32 data_0208ad90;
extern const u32 data_0208ad8c;
extern const u32 data_0208ad98;

const char *DinoUiSaveSlotSelectMenuCancelItem::GetText() {
    if (data_ov004_02177280) {
        return gDinoResource->GetText(data_0208ad88);
    }
    return gDinoResource->GetText(data_0208ad90);
}

const char *DinoUiSaveSlotSelectMenuItem::GetText() {
    if (data_ov004_02177280) {
        return gDinoResource->GetText(data_0208ad8c + mSlot);
    }
    return gDinoResource->GetText(data_0208ad98 + mSlot);
}

DinoUiSaveSlotConfirm::DinoUiSaveSlotConfirm() : MProcess(NULL) {
}

#pragma auto_inline off

u32 DinoUiTopMenu::GetUnkB4() {
    return mUnkB4;
}
