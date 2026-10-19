// DinoUiSaveOption, DinoUiSaveOptionAppendItemTask, DinoUiSaveOptionRemoveItemTask, DinoUiSaveOptionMenuSaveTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "m/picture.h"
#include "m/smart_oam.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/disp_blower_menu.h"
#include "game/dino_ui_save_option_menu.h"
#include "game/dino_ui_save_option.h"
#include "game/dino_ui_save_option_append_item_task.h"
#include "game/dino_ui_save_option_remove_item_task.h"
#include "game/dino_ui_save_option_menu_save_task.h"
#include "game/dino_ui_save_option_menu_item.h"
#include "game/dino_sound.h"
#include "game/dino_ui_save_menu.h"

#pragma auto_inline off
DinoUiSaveOptionMenu::DinoUiSaveOptionMenu(u32 arg) : MMenu(0), mUnkB4(arg) {
}
#pragma auto_inline on

DinoUiSaveOption::~DinoUiSaveOption() {
}

DinoUiSaveOptionAppendItemTask::~DinoUiSaveOptionAppendItemTask() {
}

DinoUiSaveOptionRemoveItemTask::~DinoUiSaveOptionRemoveItemTask() {
}

DinoUiSaveOptionMenuSaveTask::~DinoUiSaveOptionMenuSaveTask() {
}

DinoUiSaveOptionMenuItem::~DinoUiSaveOptionMenuItem() {
}

DinoUiSaveOptionMenu::~DinoUiSaveOptionMenu() {
}

DinoUiSaveOptionSaveButton::~DinoUiSaveOptionSaveButton() {
}

DinoUiSaveOptionOptionButton::~DinoUiSaveOptionOptionButton() {
}

DinoUiSaveOptionBackButton::~DinoUiSaveOptionBackButton() {
}


// The TU's pooled constants (see menu.cpp): the buttons' motions and places.
extern "C" struct {
    u32 unk00;
    const char *optionPath; // 0x04
    const char *savePath;   // 0x08
    const char *backPath;   // 0x0C
    u8 unk10[0x20 - 0x10];
    MPoint optionPos;       // 0x20
    MPoint savePos;         // 0x28
    MPoint backPos;         // 0x30
} data_ov004_02174cb4;

// The buttons' help texts.
extern "C" const u32 data_0208abbc, data_0208abc0, data_0208abc4;


void DinoUiSaveOptionMenu::OnStart() {
    SetLinkFallback(TRUE);
    mSilentFocus = TRUE;
    if (mUnkB4 != 0) {
        *(u32 *)mUnkB4 = 0;
    }
    mParallel.AddTail(mBg = new MTextPicture(std::string("image/save_option_select_bg"), TRUE, false));
    mBg->Show();
    mBg->SetPriority(3);
    mOam = new MSmartOam(0x20, 0, 1);
    AppendItem(1);
    mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(30, 0));
    mParallel.AddTail(mOam);
}

MRect DinoUiSaveOptionMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiSaveOptionMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiSaveOptionMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiSaveOptionMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiSaveOptionMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiSaveOptionBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiSaveOptionRemoveItemTask::Execute() {
    mOwner->RemoveItem();
    Exit();
}

// Saves through the save menu, then goes back to the options or leaves.
void DinoUiSaveOptionMenuSaveTask::Idle() {
    switch (mStep++) {
    case 0:
        mMenu->RemoveItem();
        mSerial.AddTail(new DinoUiSaveMenu(&mResult, 0));
        break;
    case 1:
        switch (mResult) {
        case 2:
            if (mArg != 0) {
                *(u32 *)mArg = 1;
            }
            mMenu->OnBackDecided();
            Exit();
            break;
        case 1:
            mMenu->OnBackDecided();
            Exit();
            break;
        case 0:
            mMenu->AppendItem(1);
            Exit();
            break;
        }
        break;
    }
}

void DinoUiSaveOptionBackButton::OnDecide() {
    mMotion->Play(4, 1);
    DinoSoundPlaySe(0x38F, 0);
    mOwner->OnBackDecided();
}

void DinoUiSaveOptionMenu::OnFocus(MMenuItem *item) {
    if (mSilentFocus) {
        mSilentFocus = FALSE;
    } else {
        DinoSoundPlaySe(0x38E, 0);
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
}

void DinoUiSaveOptionOptionButton::OnDecide() {
    mMotion->Play(3, 1);
    mOwner->OnOptionDecided();
}

void DinoUiSaveOptionSaveButton::OnDecide() {
    mMotion->Play(3, 1);
    mOwner->OnSaveDecided();
}

MRect DinoUiSaveOptionOptionButton::GetFrame() {
    return MRect(mMotion->GetFrame().GetTopLeft(), mMotion->GetFrame().GetTopLeft().Offset(108, 76));
}

MRect DinoUiSaveOptionSaveButton::GetFrame() {
    return MRect(mMotion->GetFrame().GetTopLeft(), mMotion->GetFrame().GetTopLeft().Offset(108, 76));
}

void DinoUiSaveOptionAppendItemTask::Execute() {
    mMenu->AppendItem(mKind);
    Exit();
}

void DinoUiSaveOptionMenu::OnSaveDecided() {
    DinoSoundPlaySe(0x390, 0);
    mSilentFocus = TRUE;
    MTask *save = new DinoUiSaveOptionMenuSaveTask(this, mUnkB4);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(save);
}

void DinoUiSaveOptionMenu::RemoveItem() {
    RemoveLink(mItems[0], 2);
    RemoveLink(mItems[2], 3);
    MMenu::RemoveItem(mItems[0]);
    MMenu::RemoveItem(mItems[1]);
    MMenu::RemoveItem(mItems[2]);
    delete mItems[0];
    delete mItems[1];
    delete mItems[2];
}

void DinoUiSaveOptionMenu::OnOptionDecided() {
    DinoSoundPlaySe(0x390, 0);
    mSilentFocus = TRUE;
    MTask *append = new DinoUiSaveOptionAppendItemTask(0, this);
    MTask *option = new DinoUiOptionMenu();
    MTask *remove = new DinoUiSaveOptionRemoveItemTask(this);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(remove);
    serial->AddTail(option);
    serial->AddTail(append);
}

void DinoUiSaveOption::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->VSyncNow();
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiSaveOptionMenu(mUnk74);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

#pragma auto_inline off

// Leaves: to white (and the title) after a save that asked for it, else to black.
void DinoUiSaveOptionMenu::OnBackDecided() {
    if (mUnkB4 != 0 && *(u32 *)mUnkB4 == 1) {
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *mainFade;
        MTask *subFade;
        subFade = gLcd->mSub->mBright->CreateFadeOutToWhite(30, 0);
        mainFade = gLcd->mMain->mBright->CreateFadeOutToWhite(30, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(mainFade);
        fades->AddTail(subFade);
        MTask *stopBgm = DinoSoundCreateStopBgmTask(-1, 30);
        MTask *timer = new MTimer(30);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(stopBgm);
        serial->AddTail(fades);
        serial->AddTail(exit);
        return;
    }
    MTask &self = *this;
    MTask *fade;
    MTask *exit;
    exit = MTaskExitTransfer::Create(&self);
    fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

// Adds the buttons, the save one focused if kind is set, else the option one.
void DinoUiSaveOptionMenu::AppendItem(u8 kind) {
    mItems[0] = new DinoUiSaveOptionSaveButton(this, data_ov004_02174cb4.savePath, data_ov004_02174cb4.savePos,
                                               data_0208abc4);
    MMenu::AddItem(mItems[0]);
    mItems[1] = new DinoUiSaveOptionOptionButton(this, data_ov004_02174cb4.optionPath, data_ov004_02174cb4.optionPos,
                                                 data_0208abc0);
    MMenu::AddItem(mItems[1]);
    mItems[2] = new DinoUiSaveOptionBackButton(this, data_ov004_02174cb4.backPath, data_ov004_02174cb4.backPos,
                                               data_0208abbc);
    MMenu::AddItem(mItems[2]);
    if (kind) {
        FocusItem(mItems[0]);
    } else {
        FocusItem(mItems[1]);
    }
    AddLink(mItems[0], 2, mItems[2]);
    AddLink(mItems[2], 3, NULL);
    AddEntry((u32)(DinoHelpItem *)(DinoUiSaveOptionMenuItem *)mItems[0], 2,
             (u32)(DinoHelpItem *)(DinoUiSaveOptionMenuItem *)mItems[2]);
    AddEntry((u32)(DinoHelpItem *)(DinoUiSaveOptionMenuItem *)mItems[2], 3, 0);
}

DinoUiSaveOptionMenuItem::DinoUiSaveOptionMenuItem(DinoUiSaveOptionMenu *owner, const std::string &path, MPoint pos,
                                                   u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}


MSmartOam *DinoUiSaveOptionMenu::GetOam() {
    return mOam;
}

DinoUiSaveOption::DinoUiSaveOption(u32 arg) : MProcess(NULL), mUnk74(arg) {
}
