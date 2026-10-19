// DinoUiSaveOption, DinoUiSaveOptionAppendItemTask, DinoUiSaveOptionRemoveItemTask, DinoUiSaveOptionMenuSaveTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
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


u32 DinoUiSaveOptionMenu::GetUnkB8() {
    return mUnkB8;
}

DinoUiSaveOption::DinoUiSaveOption(u32 arg) : MProcess(NULL), mUnk74(arg) {
}
