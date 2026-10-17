#ifndef GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUi2ContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUi2ContainerBaseMenu();
};

class DinoUi2CannonShootMenu : public DinoUi2ContainerBaseMenu {
public:
    virtual ~DinoUi2CannonShootMenu();
};

#endif
