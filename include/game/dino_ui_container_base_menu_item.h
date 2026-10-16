#ifndef GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_H
#define GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiContainerBaseMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiContainerBaseMenuItem();
};

#endif
