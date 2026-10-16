#ifndef GAME_DINO_UI_MASK_BUY_MENU_ITEM_H
#define GAME_DINO_UI_MASK_BUY_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiMaskBuyMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMaskBuyMenuItem();
};

#endif
