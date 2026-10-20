#ifndef GAME_DINO_CLEANING_DUMMY_ITEM_H
#define GAME_DINO_CLEANING_DUMMY_ITEM_H

#include "m/menu_item.h"

// An invisible item that can't be focused or touched; it only takes the keys.
class DinoCleaningDummyItem : public MMenuItem {
public:
    DinoCleaningDummyItem() {
        SetNoFocus(TRUE);
        SetNoTouch(TRUE);
        SetKeys(0xF0);
    }
    virtual ~DinoCleaningDummyItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

#endif
