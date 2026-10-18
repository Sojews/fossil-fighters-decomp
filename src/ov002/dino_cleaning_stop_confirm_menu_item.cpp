// DinoCleaningStopConfirmMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_stop_confirm_menu_item_menu.h"
#include "game/dino_cleaning_stop_confirm_menu_item.h"

DinoCleaningStopConfirmMenuItem::~DinoCleaningStopConfirmMenuItem() {
}

DinoCleaningStopConfirmMenu::~DinoCleaningStopConfirmMenu() {
}

DinoCleaningStopConfirm::~DinoCleaningStopConfirm() {
    mCleaning->SetUnk128(FALSE);
}

DinoCleaningSleep::~DinoCleaningSleep() {
    mCleaning->SetUnk128(FALSE);
}

MRect DinoCleaningStopConfirmMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCleaningStopConfirmMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoCleaningStopConfirmMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoCleaningStopConfirm::Idle() {
    (this->*mState)();
}

void DinoCleaningSleep::Idle() {
    (this->*mState)();
}

void DinoCleaningStopConfirmMenu::OnFocus(MMenuItem *item) {
    if (mSilentFocus) {
        mSilentFocus = FALSE;
        return;
    }
    if (item == mYesItem) {
        mMotion->Play(2, 0);
    } else if (item == mNoItem) {
        mMotion->Play(3, 0);
    }
}

void DinoCleaningStopConfirmMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        Select();
    }
}

MRect DinoCleaningStopConfirmMenuItem::GetFrame() {
    return mMotion->GetCellRect(mCell);
}

void DinoCleaningStopConfirmMenu::OnDecide(MMenuItem *item) {
    Disable();
    if (item == mYesItem) {
        mMotion->Play(4, 0);
        *mResult = TRUE;
    } else if (item == mNoItem) {
        mMotion->Play(5, 0);
        *mResult = FALSE;
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

// Closes the window once the cleaning takes input again.
void DinoCleaningSleep::StateClose() {
    if (*mTimer == 0) {
        mMotion->Play(2, 0);
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(exit);
    }
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    const char *mMotion04;  // 0x04: the questions' motions
    const char *mMotion08;  // 0x08
    u8 unk0C[0x18 - 0xC];
    void (DinoCleaningStopConfirm::*Start_18)();
    u8 unk20[0x38 - 0x20];
    void (DinoCleaningStopConfirm::*Unk02156b08_38)();
    void (DinoCleaningStopConfirm::*StateDecided_40)();
    void (DinoCleaningSleep::*Start_48)();
    u8 unk50[0x60 - 0x50];
    void (DinoCleaningStopConfirm::*Unk02156a9c_60)();
} data_ov002_02163f40;

DinoCleaningStopConfirm::DinoCleaningStopConfirm(DinoCleaning *cleaning, u8 *result)
    : MProcess(NULL), mCleaning(cleaning), mResult(result) {
    mState = data_ov002_02163f40.Start_18;
}

// Leaves once the answer is no.
void DinoCleaningStopConfirm::StateDecided() {
    if (*mResult == FALSE) {
        Exit();
        return;
    }
    mState = data_ov002_02163f40.Unk02156a9c_60;
}

// The questions' text ids.
extern "C" const u32 data_0208ad14;
extern "C" const u32 data_0208ad18;
extern "C" const u32 data_0208ad1c;
extern "C" const u32 data_0208ad20;

DinoCleaningStopConfirmMenu::DinoCleaningStopConfirmMenu(std::string motion, u8 *result, u32 yesText, u32 noText)
    : MMenu(0), mUnkB8(motion), mResult(result), mYesText(yesText), mNoText(noText) {
}

void DinoCleaningStopConfirm::StateAsk() {
    mSerial.AddTail(new DinoCleaningStopConfirmMenu(data_ov002_02163f40.mMotion04, mResult, data_0208ad20, data_0208ad1c));
    mState = data_ov002_02163f40.StateDecided_40;
}

void DinoCleaningStopConfirm::StateAskAgain() {
    mSerial.AddTail(new DinoCleaningStopConfirmMenu(data_ov002_02163f40.mMotion08, mResult, data_0208ad18, data_0208ad14));
    mState = data_ov002_02163f40.Unk02156b08_38;
}

DinoCleaningSleep::DinoCleaningSleep(DinoCleaning *cleaning, u32 *timer) : MProcess(NULL), mCleaning(cleaning), mTimer(timer) {
    mState = data_ov002_02163f40.Start_48;
}
