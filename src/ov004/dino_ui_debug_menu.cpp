// DinoUiDebugMenu.
// Only the destructors are decompiled so far.

#include "game/dino_ui_debug_kaseki_rock_get.h"
#include "game/dino_ui_debug_reviver_menu_menu.h"
#include "game/dino_ui_debug_flag_menu_menu.h"
#include "game/dino_ui_debug_menu.h"
#include "m/lcd.h"

DinoUiDebugMenu::~DinoUiDebugMenu() {
}

void DinoUiDebugMenu::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiDebugMenu::*Start_04)();
    void (DinoUiDebugMenu::*Start_0C)();
    void (DinoUiDebugMenu::*Start_14)();
    u8 unk1C[0x24 - 0x1C];
    void (DinoUiDebugMenu::*Start_24)();
    u8 unk2C[0x34 - 0x2C];
    void (DinoUiDebugMenu::*Start_34)();
} data_ov004_02173af0;

DinoUiDebugMenu::DinoUiDebugMenu() : MProcess(gLcd->GetBottomDisplay()), mUnk7C(0) {
    mState = data_ov004_02173af0.Start_24;
}

// The debug menu's choices: the flag menus, the reviver menu or kaseki rock get.
void DinoUiDebugMenu::StateChosen() {
    switch (mUnk7C) {
    default:
        Exit();
        break;
    case 0:
        mSerial.AddTail(new DinoUiDebugFlagMenu(0));
        mState = data_ov004_02173af0.Start_34;
        break;
    case 1:
        mSerial.AddTail(new DinoUiDebugFlagMenu(1));
        mState = data_ov004_02173af0.Start_14;
        break;
    case 2:
        mSerial.AddTail(new DinoUiDebugReviverMenu);
        mState = data_ov004_02173af0.Start_04;
        break;
    case 3:
        mSerial.AddTail(new DinoUiDebugKasekiRockGet);
        mState = data_ov004_02173af0.Start_0C;
        break;
    }
}
