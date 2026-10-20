#ifndef GAME_DINO_CLEANING_SAMPLE_MENU_RANDOM_ITEM_H
#define GAME_DINO_CLEANING_SAMPLE_MENU_RANDOM_ITEM_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoCleaningSampleMenuRandomItem : public MMenuItem {
public:
    virtual ~DinoCleaningSampleMenuRandomItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    MView *mView;   // 0x88
    u32 *mKaseki;   // 0x8C: the fossil chosen
    u32 *mUnk90;    // 0x90
    u32 *mUnk94;    // 0x94
};

#endif
