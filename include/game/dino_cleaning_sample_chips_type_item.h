#ifndef GAME_DINO_CLEANING_SAMPLE_CHIPS_TYPE_ITEM_H
#define GAME_DINO_CLEANING_SAMPLE_CHIPS_TYPE_ITEM_H

#include "m/menu_item.h"

class DinoCleaningSampleChipsTypeItem : public MMenuItem {
public:
    virtual ~DinoCleaningSampleChipsTypeItem();
    virtual void Idle();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u8 unk88[0x8];
    MMenuItem *mKeyItemA; // 0x90
    MMenuItem *mKeyItemB; // 0x94
};

#endif
