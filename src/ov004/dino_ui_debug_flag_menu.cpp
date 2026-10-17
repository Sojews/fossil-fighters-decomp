// DinoUiDebugFlagMenu.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/font.h"
#include "game/dino_ui_debug_flag_menu_menu.h"
#include "types.h"

DinoUiDebugFlagMenu::~DinoUiDebugFlagMenu() {
    mFont->Release();
}

DinoUiDebugFlagMenuItem::~DinoUiDebugFlagMenuItem() {
    mFont->Release();
}

// NONMATCHING: the game folds the offsets differently.
MRect DinoUiDebugFlagMenuItem::GetFrame() {
    MRect frame(0, 0, 16, 16);
    frame.Offset(mPos);
    frame.Offset(MPoint(-5, -2));
    return frame.Inflate(2);
}
