#ifndef GAME_DINO_UI_SHOP_SELL_MENU_ITEM_H
#define GAME_DINO_UI_SHOP_SELL_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiShopSellMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiShopSellMenuItem();
};

#endif
