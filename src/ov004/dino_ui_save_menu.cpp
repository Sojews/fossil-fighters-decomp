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
} data_ov004_02175130;

DinoUiSaveMenu::DinoUiSaveMenu(u32 *result, u8 arg) : MProcess(NULL), mResult(result), mUnk90(arg) {
    mState = data_ov004_02175130.Start_0C;
}
