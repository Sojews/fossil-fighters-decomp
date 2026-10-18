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
