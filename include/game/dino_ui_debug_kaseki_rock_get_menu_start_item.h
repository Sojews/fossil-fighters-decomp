#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_START_ITEM_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_START_ITEM_H

#include "m/view.h"
#include "m/menu_item.h"

class MSmartOam;
class MFont;
class MMenu;

class DinoUiDebugKasekiRockGetMenuStartItem : public MMenuItem {
public:
    DinoUiDebugKasekiRockGetMenuStartItem(MSmartOam *oam, MFont *font, u32 textId, MPoint center);
    virtual ~DinoUiDebugKasekiRockGetMenuStartItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    MView *mView;   // 0x88
};

#endif
