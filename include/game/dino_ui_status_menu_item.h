#ifndef GAME_DINO_UI_STATUS_MENU_ITEM_H
#define GAME_DINO_UI_STATUS_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiStatusMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiStatusMenuItem();
};

#endif
