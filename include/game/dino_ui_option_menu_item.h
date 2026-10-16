#ifndef GAME_DINO_UI_OPTION_MENU_ITEM_H
#define GAME_DINO_UI_OPTION_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiOptionMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiOptionMenuItem();
};

#endif
