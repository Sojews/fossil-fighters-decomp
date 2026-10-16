#ifndef GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_H
#define GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiPartsTabIconMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiPartsTabIconMenuItem();
};

#endif
