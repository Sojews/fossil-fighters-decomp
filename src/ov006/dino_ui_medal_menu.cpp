// DinoUiMedalMenu.
// Only the destructor is decompiled so far.

#include "game/dino_ui_medal_menu.h"
#include "m/display.h"
#include "m/blend.h"

DinoUiMedalMenu::~DinoUiMedalMenu() {
    gCurrentDisplay->mBlend->Reset();
}
