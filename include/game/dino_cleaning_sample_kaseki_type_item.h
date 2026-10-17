#ifndef GAME_DINO_CLEANING_SAMPLE_KASEKI_TYPE_ITEM_H
#define GAME_DINO_CLEANING_SAMPLE_KASEKI_TYPE_ITEM_H

#include "m/menu_item.h"

class DinoCleaningSampleKasekiTypeItem : public MMenuItem {
public:
    virtual ~DinoCleaningSampleKasekiTypeItem();
    virtual void Idle();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
