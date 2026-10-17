#ifndef GAME_DINO_CLEANING_SAMPLE_MENU_RANDOM_ITEM_H
#define GAME_DINO_CLEANING_SAMPLE_MENU_RANDOM_ITEM_H

#include "m/menu_item.h"

class DinoCleaningSampleMenuRandomItem : public MMenuItem {
public:
    virtual ~DinoCleaningSampleMenuRandomItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

#endif
