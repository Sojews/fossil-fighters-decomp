// DinoUiContainerDropMenuPageIncTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_container_drop_menu_page_inc_task_menu.h"
#include "game/dino_ui_container_drop_menu_page_inc_task.h"
#include "game/dino_ui_container_drop_menu_item.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_yes_no_dialog.h"

#pragma auto_inline off

DinoUiContainerDropMenu::DinoUiContainerDropMenu(u32 arg0, u32 arg1, const std::vector<std::string> &names)
    : MMenu(NULL, 0), mUnkDC(arg0), mUnkE0(arg1), mUnkE4(names), mPage(0), mLastPage(0), mUnk120(-1), mStarted(0),
      mUnk128(0) {
}

DinoUiContainerDropMenuItem::DinoUiContainerDropMenuItem(DinoUiContainerDropMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

BOOL DinoUiContainerDropMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ShowPage();
    return TRUE;
}

BOOL DinoUiContainerDropMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ShowPage();
    return TRUE;
}

#pragma auto_inline on

DinoUiContainerDropMenuPageIncTask::~DinoUiContainerDropMenuPageIncTask() {
}

DinoUiContainerDropMenuItem::~DinoUiContainerDropMenuItem() {
}

DinoUiContainerDropMenu::~DinoUiContainerDropMenu() {
}

DinoUiContainerDropBackButton::~DinoUiContainerDropBackButton() {
}

DinoUiContainerDropPageLButton::~DinoUiContainerDropPageLButton() {
}

DinoUiContainerDropPageRButton::~DinoUiContainerDropPageRButton() {
}

MRect DinoUiContainerDropMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerDropMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerDropMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerDropMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerDropMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerDropBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerDropBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiContainerDropPageRButton::OnDecide() {
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

void DinoUiContainerDropPageLButton::OnDecide() {
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

void DinoUiContainerDropMenuPageIncTask::Execute() {
    mOwner->PageRight();
    Exit();
}


void DinoUiContainerDropMenu::OnIdle() {
    mStarted = TRUE;
    if (mUnk128 == 1) {
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
    }
    if (mUnk128) {
        mUnk128 = 0;
    }
}

extern const u32 data_0208a650, data_0208a654;

// Asks whether to leave.
void DinoUiContainerDropMenu::OnBackDecided() {
    *(int *)mUnkDC = -1;
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(mUnkE4[0], &mUnk128);
    dialog->SetLabels(mUnkE4[1], mUnkE4[2]);
    dialog->SetLabels2(std::string(gDinoResource->GetText(data_0208a654)),
                       std::string(gDinoResource->GetText(data_0208a650)));
    mSerial.AddTail(dialog);
}
