#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_INC_ITEM_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_INC_ITEM_H

#include "m/menu_item.h"
#include "m/view.h"

class MSmartOam;
class MFont;
class MMenu;

class DinoUiDebugKasekiRockGetMenuIncItem : public MMenuItem {
public:
    DinoUiDebugKasekiRockGetMenuIncItem(int *value, int min, int max, MSmartOam *oam, MFont *font, MPoint center);
    virtual ~DinoUiDebugKasekiRockGetMenuIncItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    int *mValue;    // 0x88: the number this item changes, wrapping between mMin and mMax
    int mMin;       // 0x8C
    int mMax;       // 0x90
    MView *mView;   // 0x94
};

#endif
