// DinoUiFieldMenu.
// Only the destructors are decompiled so far.

#include "game/dino_ui_field_menu.h"

DinoUiFieldMenu::~DinoUiFieldMenu() {
}

void DinoUiFieldMenu::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x10];
    void (DinoUiFieldMenu::*Start_10)();
} data_ov006_02190da0;

DinoUiFieldMenu::DinoUiFieldMenu(u32 arg) : MProcess(NULL), mUnk7C(arg) {
    mState = data_ov006_02190da0.Start_10;
}
