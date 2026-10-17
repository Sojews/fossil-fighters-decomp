#ifndef GAME_DINO_UI_SHOP_BUY_H
#define GAME_DINO_UI_SHOP_BUY_H

#include "m/task.h"

class DinoUiShopBuy : public MProcess {
public:
    virtual ~DinoUiShopBuy();
    virtual void Idle();
};

#endif
