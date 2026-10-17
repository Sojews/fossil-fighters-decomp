#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_DEC_ITEM_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_DEC_ITEM_H

#include "m/menu_item.h"

class DinoUiDebugKasekiRockGetMenuDecItem : public MMenuItem {
public:
    virtual ~DinoUiDebugKasekiRockGetMenuDecItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

#endif
