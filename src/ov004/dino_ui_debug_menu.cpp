// DinoUiDebugMenu.
// Only the destructors are decompiled so far.

#include "game/dino_ui_debug_menu.h"
#include "m/lcd.h"

DinoUiDebugMenu::~DinoUiDebugMenu() {
}

void DinoUiDebugMenu::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x24];
    void (DinoUiDebugMenu::*Start_24)();
} data_ov004_02173af0;

DinoUiDebugMenu::DinoUiDebugMenu() : MProcess(gLcd->GetBottomDisplay()), mUnk7C(0) {
    mState = data_ov004_02173af0.Start_24;
}
