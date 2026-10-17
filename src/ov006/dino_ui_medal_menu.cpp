// DinoUiMedalMenu.
// Only the destructor is decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_medal_menu.h"
#include "m/display.h"
#include "m/blend.h"

DinoUiMedalMenu::~DinoUiMedalMenu() {
    gCurrentDisplay->mBlend->Reset();
}

DinoUiSortMenuItem::~DinoUiSortMenuItem() {
}

DinoUiMedalList::~DinoUiMedalList() {
}

DinoUiMedalMenuItem::~DinoUiMedalMenuItem() {
}

DinoUiMedalBackButton::~DinoUiMedalBackButton() {
}

DinoUiMedalPageLButton::~DinoUiMedalPageLButton() {
}

DinoUiMedalPageRButton::~DinoUiMedalPageRButton() {
}

DinoUiMedalSlotCreateTask::~DinoUiMedalSlotCreateTask() {
}
