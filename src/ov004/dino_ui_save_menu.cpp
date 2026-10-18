// DinoUiSaveMenu.
// Only the destructors are decompiled so far.

#include "game/dino_ui_save_menu.h"

DinoUiSaveMenu::~DinoUiSaveMenu() {
}

void DinoUiSaveMenu::Idle() {
    (this->*mState)();
}

// Ends the window, answering 1 for yes (mUnk8C 1) and 2 otherwise.
void DinoUiSaveMenu::StateFinish() {
    mUnk94->Exit();
    if (mUnk8C == 1) {
        *mResult = 1;
        Exit();
    } else {
        *mResult = 2;
        Exit();
    }
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0xC];
    void (DinoUiSaveMenu::*Start_0C)();
    u8 unk14[0x24 - 0x14];
    void (DinoUiSaveMenu::*Unk02169db8_24)();
} data_ov004_02175130;

DinoUiSaveMenu::DinoUiSaveMenu(u32 *result, u8 arg) : MProcess(NULL), mResult(result), mUnk90(arg) {
    mState = data_ov004_02175130.Start_0C;
}

void DinoUiSaveMenu::StateConfirmed() {
    if (mUnk8C == 1) {
        mState = data_ov004_02175130.Unk02169db8_24;
        return;
    }
    *mResult = 0;
    Exit();
}
