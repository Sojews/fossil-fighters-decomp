#ifndef GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_H
#define GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUi2ContainerBaseMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUi2ContainerBaseMenuItem();
};

#endif
