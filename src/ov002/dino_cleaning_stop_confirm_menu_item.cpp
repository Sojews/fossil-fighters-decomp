// DinoCleaningStopConfirmMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_stop_confirm_menu_item_menu.h"
#include "game/dino_cleaning_stop_confirm_menu_item.h"

DinoCleaningStopConfirmMenuItem::~DinoCleaningStopConfirmMenuItem() {
}

DinoCleaningStopConfirmMenu::~DinoCleaningStopConfirmMenu() {
}

DinoCleaningStopConfirm::~DinoCleaningStopConfirm() {
    mRock->SetUnk128(FALSE);
}

DinoCleaningSleep::~DinoCleaningSleep() {
    mRock->SetUnk128(FALSE);
}

MRect DinoCleaningStopConfirmMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCleaningStopConfirmMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}
