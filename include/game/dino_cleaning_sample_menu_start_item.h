#ifndef GAME_DINO_CLEANING_SAMPLE_MENU_START_ITEM_H
#define GAME_DINO_CLEANING_SAMPLE_MENU_START_ITEM_H

#include "m/menu_item.h"

class DinoCleaningSampleMenuStartItem : public MMenuItem {
public:
    virtual ~DinoCleaningSampleMenuStartItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

#endif
