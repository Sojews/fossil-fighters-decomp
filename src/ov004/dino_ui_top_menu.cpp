// DinoUiTopMenu.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/disp_blower_menu.h"
#include "m/irq.h"
#include "game/dino_3d_load_csv.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_ui_title_menu.h"
#include "game/dino_ui_exchange.h"
#include "game/dino_ui_multiplay_loop.h"
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
#include "m/pal.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_resource.h"
#include "game/dino_ui_save_slot_confirm.h"
#include "game/dino_ui_save_slot_select_menu_confirm_delete_task.h"
#include "game/dino_ui_save_slot_select_menu_item.h"
#include "game/dino_ui_save_slot_select_menu_cancel_item.h"
#include "game/dino_ui_save_slot_select_menu.h"
#include "game/common_window_message.h"
#include "game/dino_static_init.h"

DinoUiTopMenuLoop::~DinoUiTopMenuLoop() {
}

// Pooled file statics: the items' corners relative to their motions.
struct DinoUiTopMenuStatics {
    u8 unk00[0x2C];
    void (DinoUiTopMenuLoop::*mTopMenu_2C)();       // 0x2C: (pooled pointer-to-member constants, see menu.cpp)
    void (DinoUiTopMenuLoop::*mTopMenu_34)();       // 0x34
    u8 unk3C[0x44 - 0x3C];
    void (DinoUiTopMenuLoop::*mMultiplayEnd_44)();  // 0x44
    void (DinoUiTopMenuLoop::*mMultiplay_4C)();     // 0x4C
    void (DinoUiTopMenuLoop::*mTopMenuEnd_54)();    // 0x54
    u8 unk5C[0x64 - 0x5C];
    MPoint mItemTopLeft;        // 0x64
    void (DinoUiTopMenuLoop::*mMultiplay_6C)();     // 0x6C
    MPoint mItem2BottomRight;   // 0x74
};
struct DinoUiTopMenuStatics2 {
    u8 unk00[0x14];
    void (DinoUiTopMenuLoop::*mTopMenu_14)();   // 0x14: (pooled pointer-to-member constants, see menu.cpp)
    void (DinoUiTopMenuLoop::*mLoopStart)();    // 0x1C
    u8 unk24[0x2C - 0x24];
    void (DinoUiTopMenuLoop::*mExchangeEnd_2C)();   // 0x2C
    u8 unk34[0x3C - 0x34];
    void (DinoUiTopMenuLoop::*mMultiplay_3C)();     // 0x3C
    void (DinoUiTopMenuLoop::*mExchange_44)();      // 0x44
    MPoint mItemBottomRight;    // 0x4C
    u8 unk54[0x64 - 0x54];
    MPoint mItem2TopLeft;       // 0x64
};
extern "C" DinoUiTopMenuStatics data_ov004_0217206c;
extern "C" DinoUiTopMenuStatics2 data_ov004_021720ec;

MRect DinoUiTopMenuItem::GetFrame() {
    return MRect(mMotion->GetPosition() + data_ov004_0217206c.mItemTopLeft,
                 mMotion->GetPosition() + data_ov004_021720ec.mItemBottomRight);
}

MRect DinoUiTopMenuItem2::GetFrame() {
    return MRect(mMotion->GetPosition() + data_ov004_021720ec.mItem2TopLeft,
                 mMotion->GetPosition() + data_ov004_0217206c.mItem2BottomRight);
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

DinoUiTopMenu::DinoUiTopMenu(int *result) : MMenu(0) {
    mResult = result;
    mSilentFocus = TRUE;
    SetDeferStart(TRUE);
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

extern u8 data_ov004_02177280;      // set when the screen saves rather than loads
extern const u32 data_0208a978;

void DinoUiSaveSlotSelectMenu::StartDelete() {
    mDelete->MMenuItem::Disable();
    mDelete->DinoHelpItem::Disable();
    mDeleting = TRUE;
    data_ov004_02177280 = TRUE;
    mMessage = new DinoCommonWindowMessage(gLcd->GetBottomDisplay(), mDelete->GetFrame().GetCenter(),
                                           std::string(gDinoResource->GetText(data_0208a978)), 4, 0);
    for (u32 i = 0; i < 2; i++) {
        DinoSaveSummary summary;
        gDinoBackup->GetSummary(i, &summary);
        if (!summary.valid) {
            mSlots[i]->MMenuItem::Disable();
        }
    }
    mParallel.AddTail(mMessage);
    mSilentFocus = TRUE;
    FocusItem(mCancel);
    RemoveLink(mSlots[0], 4);
    RemoveLink(mDelete, 2);
    AddLink(mSlots[0], 4, mCancel);
    AddLink(mSlots[1], 2, mCancel);
    if (!mSlots[1]->MMenuItem::IsEnabled()) {
        AddLink(mSlots[0], 2, mCancel);
        RemoveLink(mCancel, 4);
        AddLink(mCancel, 4, mSlots[0]);
    }
    if (!mSlots[0]->MMenuItem::IsEnabled()) {
        AddLink(mSlots[1], 4, mCancel);
        RemoveLink(mCancel, 2);
        AddLink(mCancel, 2, mSlots[1]);
    }
    AddEntry((u32)(DinoHelpItem *)mCancel, 3, 0);
}

MRect DinoUiSaveSlotSelectMenuDeleteItem::GetFrame() {
    return mMotion->GetFrame();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoUiSaveSlotSelect>;
template MTask *MCreateBgCallTask(DinoUiSaveSlotSelect *obj, void (DinoUiSaveSlotSelect::*func)(), int thread);

#pragma auto_inline off

DinoUiTopMenuItem2::DinoUiTopMenuItem2(DinoUiTopMenu *owner, const std::string &path, const std::string &path2,
                                       MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion2 = new MMotion(std::string(path2), mOwner->GetOam(), 2, 1);
    mMotion2->Play(1, 0);
    mMotion2->SetPosition(pos);
    mMotion2->Show();
    mParallel.AddTail(mMotion2);
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

// Lights up (or not) and nudges the label (which sits a little left when focused).
// NONMATCHING: the game reads the size through a pointer, from other stack slots.
void DinoUiSaveSlotSelectMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
        if (!mUnkA4) {
            MPoint size = mLabel->GetSize();
            MPoint pos;
            pos.y = mPos.y - size.y / 2;
            pos.x = mPos.x - size.x / 2 - 4;
            mLabel->SetPosition(pos);
            mMotion2->Play(2, 0);
            mMotion3->Play(2, 0);
        }
    } else {
        mMotion->Play(1, 0);
        if (!mUnkA4) {
            MPoint size = mLabel->GetSize();
            MPoint pos;
            pos.y = mPos.y - size.y / 2;
            pos.x = mPos.x - size.x / 2 + 4;
            mLabel->SetPosition(pos);
            mMotion2->Play(1, 0);
            mMotion3->Play(1, 0);
        }
    }
}

DinoUiTopMenuItem::DinoUiTopMenuItem(DinoUiTopMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

// Starts loading the next scene of the opening (a battle field model, its camera and sounds).
void DinoUiOpening::LoadScene() {
    u32 id = (u32)mUnkA8[mScene++];
    char path[256];
    OS_SPrintf(path, "din%03d_04", id);
    mLoad = new Dino3DLoadCsv(mUnk8C, 0x7FFF, NULL, NULL);
    mLoad->LoadByName(path, 0, 0xFFFF);
    mLoad->LoadSoundNames("/model/battle/");
    OS_SPrintf(path, "/model/battle/cm_btl%03d_99.csv", id);
    mLoad->LoadFile(path, (void *)4, 0);
    OS_SPrintf(path, "/model/battle/snd_btl%03d_99.csv", id);
    mLoad->LoadFile(path, (void *)4, 0);
    MIrqLock lock;
    mLoadTasks->AddTail(mLoad);
}

// "Delete this file?" (text ids) and the answers.
extern "C" const u32 data_0208a96c, data_0208a970, data_0208a974;

// Asks, then deletes (runs the task) or not.
void DinoUiSaveSlotSelectMenuConfirmDeleteTask::Idle() {
    switch (mStep++) {
    case 0: {
        mAnswer = FALSE;
        DinoCommonWindowYesNoDialog *dialog =
            new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208a974), &mAnswer);
        dialog->SetLabels(gDinoResource->GetText(data_0208a970), gDinoResource->GetText(data_0208a96c));
        mSerial.AddTail(dialog);
        break;
    }
    case 1:
        if (mAnswer == TRUE) {
            mSerial.AddTail(mDelete);
        } else {
            delete mDelete;
        }
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        break;
    }
}

DinoUiSaveSlotSelect::DinoUiSaveSlotSelect(MInput *input, u32 *result) : MProcess(NULL) {
    mInput = input;
    mResult = result;
}

// The pooled pointer-to-member constant (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4C];
    void (DinoUiSaveSlotSelect::*Run_4C)();   // 0x4C
} data_ov004_02172844;

#pragma dont_inline on
void DinoUiSaveSlotSelect::Idle() {
    MTaskSet *serial = &mSerial;
    serial->mUseMutex = TRUE;
    mSerial.AddTail(MCreateBgCallTask(this, data_ov004_02172844.Run_4C, 1));
}
#pragma dont_inline reset

// (In the background.) Runs the menu once, then exits.
void DinoUiSaveSlotSelect::Run() {
    DinoUiSaveSlotSelectMenu *menu = new DinoUiSaveSlotSelectMenu(mInput, mResult);
    if (menu->MTask::mState == MTask::STATE_RUN) {
        menu->Execute();
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

#pragma auto_inline on

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

// Frees both screens' pictures and ends.
void DinoUiEnding::StateEnd() {
    for (u32 i = 0; i < 2; i++) {
        MDisplay *display = i == 0 ? gLcd->mMain : gLcd->mSub;
        display->mBg->mPal->Free(mScreens[i].palette);
        display->mBg->DeleteIndexBitmap(mScreens[i].bitmap);
        display->mBg->DeleteIndexBitmap(mScreens[i].bitmap2);
    }
    Exit();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MCallTask1<DinoUiSaveSlotSelectMenu, DinoUiSaveSlotSelectMenu, u32>;
template MTask *MCreateCallTask(DinoUiSaveSlotSelectMenu *obj, void (DinoUiSaveSlotSelectMenu::*func)(u32), u32 a0);
template class MCallTask0<DinoUiSaveSlotSelectMenu, DinoUiSaveSlotSelectMenu>;
template MTask *MCreateCallTask(DinoUiSaveSlotSelectMenu *obj, void (DinoUiSaveSlotSelectMenu::*func)());
#pragma auto_inline reset
template class MBgCallTask0<DinoUiOpening>;
template MTask *MCreateBgCallTask(DinoUiOpening *obj, void (DinoUiOpening::*func)(), int thread);

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoUiSaveSlotSelectMenu::*StartDelete_00)();
} data_ov004_02172880;
extern "C" struct {
    void (DinoUiSaveSlotSelectMenu::*DeleteSlot_00)(u32);
} data_ov004_021728b0;

// Cancel leaves delete mode (or the screen); delete enters it; a slot is loaded, or (in delete mode) deleted once the
// player confirms.
void DinoUiSaveSlotSelectMenu::OnDecide(MMenuItem *item) {
    if (item == mCancel) {
        if (mDeleting) {
            DinoSoundPlaySe(0x38F, 0);
            mDeleting = FALSE;
            data_ov004_02177280 = FALSE;
            mDelete->MMenuItem::Enable();
            mDelete->DinoHelpItem::Enable();
            mMessage->Exit();
            mSlots[0]->MMenuItem::Enable();
            mSlots[1]->MMenuItem::Enable();
            RemoveLink(mSlots[0], 4);
            RemoveLink(mSlots[1], 2);
            RemoveLink(mSlots[0], 2);
            RemoveLink(mSlots[1], 4);
            RemoveLink(mCancel, 4);
            AddLink(mCancel, 4, mSlots[1]);
            RemoveLink(mCancel, 2);
            AddLink(mCancel, 2, mSlots[0]);
            RemoveEntry((u32)(DinoHelpItem *)mCancel, 3);
            AddLink(mSlots[0], 4, mDelete);
            AddLink(mDelete, 2, mSlots[0]);
            mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&mCancel->mSerial));
        } else {
            DinoSoundPlaySe(0x38F, 0);
            *mResult = 1;
            MTask *exit = MTaskExitTransfer::Create(this);
            MTask *wait = MTaskSetWaitEmptyTask::Create(&mCancel->mSerial);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(wait);
            serial->AddTail(exit);
        }
        return;
    }
    if (mDelete != NULL && item == mDelete) {
        DinoSoundPlaySe(0x390, 0);
        MTask *start = MCreateCallTask(this, data_ov004_02172880.StartDelete_00);
        MTask *wait = mDelete->mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(start);
        return;
    }
    if (item == mSlots[0] || item == mSlots[1]) {
        DinoUiSaveSlotSelectMenuItem *slot = (DinoUiSaveSlotSelectMenuItem *)item;
        DinoSoundPlaySe(0x390, 0);
        if (mDeleting) {
            MTask *confirm = new DinoUiSaveSlotSelectMenuConfirmDeleteTask(
                MCreateCallTask(this, data_ov004_021728b0.DeleteSlot_00, slot->mSlot));
            MTask *wait = slot->mMotion->CreateWaitStopTask();
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(wait);
            serial->AddTail(confirm);
        } else {
            gDinoBackup->SetSlot(slot->mSlot);
            *mResult = 0;
            MTask *exit = MTaskExitTransfer::Create(this);
            MTask *wait = slot->mMotion->CreateWaitStopTask();
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(wait);
            serial->AddTail(exit);
        }
    }
}

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

MSmartOam *DinoUiTopMenu::GetOam() {
    return mOam;
}

DinoUiTopMenuLoop::DinoUiTopMenuLoop(int *result) : MProcess(NULL), mResult(result) {
    mState = data_ov004_021720ec.mLoopStart;
}

// The ending's pooled pointer-to-member constant (see menu.cpp).
extern "C" struct {
    u8 unk00[0x1C];
    void (DinoUiEnding::*Start_1C)();
} data_ov004_02172eb8;

DinoUiEnding::DinoUiEnding() : MProcess(NULL) {
    mState = data_ov004_02172eb8.Start_1C;
}


void DinoUiTopMenuLoop::StateStart() {
    mMode = 0;
    if (*mResult == 2) {
        mMode = 1;
        mState = data_ov004_0217206c.mMultiplay_4C;
    } else if (*mResult == 3) {
        mMode = 2;
        mState = data_ov004_0217206c.mMultiplay_6C;
    } else {
        mChoice = 0;
        mState = data_ov004_0217206c.mTopMenu_2C;
    }
}

// (The game calls the constructor defined above.)
#pragma dont_inline on
void DinoUiTopMenuLoop::StateTopMenu() {
    mSerial.AddTail(new DinoUiTopMenu(&mChoice));
    mState = data_ov004_0217206c.mTopMenuEnd_54;
}
#pragma dont_inline reset

// The top menu's choices: 0 and 1 leave, 2 multiplay, 3 exchange, 4 options.
void DinoUiTopMenuLoop::StateTopMenuEnd() {
    mState = data_ov004_0217206c.mTopMenu_34;
    switch (mChoice) {
    case 0:
        if (mResult != NULL) {
            *mResult = 0;
        }
        Exit();
        break;
    case 1:
        if (mResult != NULL) {
            *mResult = 1;
        }
        Exit();
        break;
    case 2:
        mState = data_ov004_021720ec.mMultiplay_3C;
        break;
    case 3:
        mState = data_ov004_021720ec.mExchange_44;
        break;
    case 4:
        mSerial.AddTail(new DinoUiOptionMenu);
        break;
    }
}

void DinoUiTopMenuLoop::StateMultiplay() {
    mSerial.AddTail(new DinoUiMultiplayLoop(&mMode));
    mState = data_ov004_0217206c.mMultiplayEnd_44;
}

void DinoUiTopMenuLoop::StateMultiplayEnd() {
    switch (mMode) {
    case 0:
        mState = data_ov004_021720ec.mTopMenu_14;
        break;
    case 1:
        *mResult = 2;
        Exit();
        break;
    case 2:
        *mResult = 3;
        Exit();
        break;
    }
}

void DinoUiTopMenuLoop::StateExchange() {
    mSerial.AddTail(new DinoUiExchange);
    mState = data_ov004_021720ec.mExchangeEnd_2C;
}

// The opening's states. (They use the title's pooled pointer-to-member constants, see dino_ui_title.cpp.)
extern "C" struct {
    u8 unk00[0x34];
    void (DinoUiOpening::*End_34)();
    u8 unk3C[0x4C - 0x3C];
    void (DinoUiOpening::*StateNextScene_4C)();
    u8 unk54[0x64 - 0x54];
    void (DinoUiOpening::*StateWaitScene_64)();
    u8 unk6C[0xFC - 0x6C];
    void (DinoUiOpening::*End_FC)();
    void (DinoUiOpening::*LoadScene_104)();
    void (DinoUiOpening::*StateShowScene_10C)();
} data_ov004_02172f78;

void DinoUiOpening::StateShowScene() {
    MTask *wait1 = new MTimer(1);
    MTask *exit = MTaskExitTransfer::Create(mLoad);
    MTask *wait = new MTimer(mSceneFrames);
    MTaskSet *tasks = mShowTasks;
    tasks->AddTail(wait);
    tasks->AddTail(exit);
    tasks->AddTail(wait1);
    mState = data_ov004_02172f78.StateWaitScene_64;
}

#pragma dont_inline on
void DinoUiOpening::StateNextScene() {
    if (mScene >= mSceneCount) {
        mState = data_ov004_02172f78.End_34;
        return;
    }
    mSerial.AddTail(MCreateBgCallTask(this, data_ov004_02172f78.LoadScene_104, 1));
    mState = data_ov004_02172f78.StateShowScene_10C;
}
#pragma dont_inline reset

void DinoUiOpening::StateWaitScene() {
    if (mSkipped) {
        mState = data_ov004_02172f78.End_FC;
        return;
    }
    if (mShowTasks->IsEmpty()) {
        mState = data_ov004_02172f78.StateNextScene_4C;
    }
}

// The file's static initialization (see dino_static_init.h).
// NONMATCHING: register choice/scheduling of the pooled stores.
static struct DinoUiTopMenuStaticInit {
    DinoUiTopMenuStaticInit() {
        DINO_POOL_WORD(data_ov004_021720ec, 0x54) = 128;
        DINO_POOL_WORD(data_ov004_021720ec, 0x58) = 96;
        DINO_POOL_WORD(data_ov004_021720ec, 0x6c) = 193;
        DINO_POOL_WORD(data_ov004_021720ec, 0x70) = 72;
        DINO_POOL_WORD(data_ov004_0217206c, 0x5c) = 63;
        DINO_POOL_WORD(data_ov004_0217206c, 0x60) = 72;
        DINO_POOL_WORD(data_ov004_021720ec, 0x5c) = 128;
        DINO_POOL_WORD(data_ov004_021720ec, 0x60) = 16;
        DINO_POOL_WORD(data_ov004_0217206c, 0x3c) = 128;
        DINO_POOL_WORD(data_ov004_0217206c, 0x40) = 130;
        DINO_POOL_WORD(data_ov004_0217206c, 0x7c) = 128;
        DINO_POOL_WORD(data_ov004_021720ec, 0x0) = 158;
        DINO_POOL_WORD(data_ov004_0217206c, 0x64) = -56;
        DINO_POOL_WORD(data_ov004_0217206c, 0x68) = -40;
        DINO_POOL_WORD(data_ov004_021720ec, 0x4c) = 56;
        DINO_POOL_WORD(data_ov004_021720ec, 0x50) = 40;
        DINO_POOL_WORD(data_ov004_021720ec, 0x64) = -104;
        DINO_POOL_WORD(data_ov004_021720ec, 0x68) = -14;
        DINO_POOL_WORD(data_ov004_0217206c, 0x74) = 104;
        DINO_POOL_WORD(data_ov004_0217206c, 0x78) = 14;
    }
} sDinoUiTopMenuStaticInit;
