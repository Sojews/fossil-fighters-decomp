#ifndef GAME_DINO_UI_CONTAINER_MENU_ITEM_H
#define GAME_DINO_UI_CONTAINER_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiContainerMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiContainerMenuItem();
};

#endif
