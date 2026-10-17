// DinoUiOrganizationMenu (team organization).
// Only the destructor is decompiled so far.

#include "game/dino_ui_organization_menu.h"
#include "m/display.h"
#include "m/blend.h"

DinoUiOrganizationMenu::~DinoUiOrganizationMenu() {
    gCurrentDisplay->mBlend->Reset();
}
