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

// Closes the window once the answer is no.
void DinoCleaningStopConfirm::StateClose() {
    if (*mResult == FALSE) {
        mMotion->Play(2, 0);
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(exit);
    }
}
