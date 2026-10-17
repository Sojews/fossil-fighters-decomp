#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_CHIPS_TYPE_ITEM_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_CHIPS_TYPE_ITEM_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoUiDebugKasekiRockGetChipsTypeItem : public MMenuItem {
public:
    virtual ~DinoUiDebugKasekiRockGetChipsTypeItem();
    virtual void Idle();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u32 mUnk88;
    MView *mView;           // 0x8C
    MMenuItem *mKeyItemA; // 0x90
    MMenuItem *mKeyItemB; // 0x94
};

#endif
