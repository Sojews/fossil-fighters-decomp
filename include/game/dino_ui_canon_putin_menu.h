#ifndef GAME_DINO_UI_CANON_PUTIN_MENU_H
#define GAME_DINO_UI_CANON_PUTIN_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_ui_container_base_menu_item_menu.h"
#include "game/dino_field_camera_hook.h"

class DinoUiCanonPutinMenu : public DinoUiContainerBaseMenu {
public:
    virtual ~DinoUiCanonPutinMenu();
};

class DinoUiCanonTakeoutMenu : public DinoUiContainerBaseMenu {
public:
    virtual ~DinoUiCanonTakeoutMenu();
};

#endif
