// DinoCommonYesNoMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#define DINO_COMMON_YES_NO_MENU_OWNER
#include "game/dino_common_yes_no_menu_item_menu.h"
#include "game/dino_common_yes_no_menu_item.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "m/lcd.h"

DinoCommonWindowYesNoDialog::~DinoCommonWindowYesNoDialog() {
}

void DinoCommonWindowYesNoDialog::SetLabels(std::string yes, std::string no) {
    mUnk8C = yes;
    mUnk80 = no;
}

DinoCommonYesNoMenuItem::~DinoCommonYesNoMenuItem() {
}

DinoCommonYesNoMenu::~DinoCommonYesNoMenu() {
}

MRect DinoCommonYesNoMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoCommonYesNoMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

MRect DinoCommonYesNoMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCommonYesNoMenuItem::GetDisplay() {
    return gLcd->GetBottomDisplay();
}

void DinoCommonYesNoMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(mIsYes ? 1 : 4, 0);
    } else {
        mMotion->Play(mIsYes ? 2 : 5, 0);
    }
}

#pragma auto_inline off

MMotion *DinoCommonYesNoMenuItem::GetMotion() {
    return mMotion;
}

u8 DinoCommonYesNoMenuItem::IsYes() {
    return mIsYes;
}

#pragma auto_inline on

void DinoCommonYesNoMenu::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help) {
        SetSelectedItem(help);
    }
}

void DinoCommonYesNoMenu::OnDecide(MMenuItem *item) {
    DinoCommonYesNoMenuItem *decided = static_cast<DinoCommonYesNoMenuItem *>(item);
    mSerial.AddTail(decided->GetMotion()->CreateWaitStopTask());
    Disable();
    if (mResult) {
        *mResult = item == mYes;
        mSerial.AddTail(MTaskExitTransfer::Create(this));
    }
}


void DinoCommonYesNoMenuItem::OnDecide() {
    mMotion->Play(mIsYes ? 3 : 6, 1);
}

#pragma auto_inline off

void DinoCommonWindowYesNoDialog::SetUnkBC(u8 value) {
    mUnkBC = value;
}
