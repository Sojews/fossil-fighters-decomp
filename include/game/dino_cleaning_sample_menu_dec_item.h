#ifndef GAME_DINO_CLEANING_SAMPLE_MENU_DEC_ITEM_H
#define GAME_DINO_CLEANING_SAMPLE_MENU_DEC_ITEM_H

#include "m/menu_item.h"

class DinoCleaningSampleMenuDecItem : public MMenuItem {
public:
    virtual ~DinoCleaningSampleMenuDecItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

#endif
