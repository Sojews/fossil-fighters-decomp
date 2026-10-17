#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_KASEKI_TYPE_ITEM_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_KASEKI_TYPE_ITEM_H

#include "m/menu_item.h"

class DinoUiDebugKasekiRockGetKasekiTypeItem : public MMenuItem {
public:
    virtual ~DinoUiDebugKasekiRockGetKasekiTypeItem();
    virtual void Idle();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
