#ifndef GAME_DINO_UI_SHOP_SELL_H
#define GAME_DINO_UI_SHOP_SELL_H

#include "m/task.h"

class DinoUiShopSell : public MProcess {
public:
    DinoUiShopSell();
    virtual ~DinoUiShopSell();
    virtual void Idle();
};

#endif
