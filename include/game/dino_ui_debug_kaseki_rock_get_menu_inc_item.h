#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_INC_ITEM_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_INC_ITEM_H

#include "m/menu_item.h"

class DinoUiDebugKasekiRockGetMenuIncItem : public MMenuItem {
public:
    virtual ~DinoUiDebugKasekiRockGetMenuIncItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

#endif
