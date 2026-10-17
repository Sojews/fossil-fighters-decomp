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
