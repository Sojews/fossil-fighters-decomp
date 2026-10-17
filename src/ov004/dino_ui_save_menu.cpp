// DinoUiSaveMenu.
// Only the destructors are decompiled so far.

#include "game/dino_ui_save_menu.h"

DinoUiSaveMenu::~DinoUiSaveMenu() {
}

void DinoUiSaveMenu::Idle() {
    (this->*mState)();
}
