#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_DEC_ITEM_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_DEC_ITEM_H

#include "m/menu_item.h"
#include "m/view.h"

class DinoUiDebugKasekiRockGetMenuDecItem : public MMenuItem {
public:
    virtual ~DinoUiDebugKasekiRockGetMenuDecItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    int *mValue;    // 0x88: the number this item changes, wrapping between mMin and mMax
    int mMin;       // 0x8C
    int mMax;       // 0x90
    MView *mView;   // 0x94
};

#endif
