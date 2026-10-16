#ifndef GAME_DINO_UI_TOP_MENU_ITEM_H
#define GAME_DINO_UI_TOP_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiTopMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiTopMenuItem();
};

#endif
