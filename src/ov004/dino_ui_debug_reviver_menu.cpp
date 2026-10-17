// DinoUiDebugReviverMenu.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/font.h"
#include "game/dino_ui_debug_reviver_menu_menu.h"
#include "types.h"

DinoUiDebugReviverMenu::~DinoUiDebugReviverMenu() {
    mFont->Release();
}

DinoUiDebugReviverMenuItem::~DinoUiDebugReviverMenuItem() {
    mFont->Release();
}

void DinoUiDebugReviverMenuItem::OnDecide() {
}
