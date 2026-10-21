#ifndef GAME_DINO_CLEANING_SAMPLE_CHIPS_TYPE_ITEM_H
#define GAME_DINO_CLEANING_SAMPLE_CHIPS_TYPE_ITEM_H

#include "m/view.h"
#include "m/menu_item.h"

class MIndicator;

class MSmartOam;
class MFont;
class MMenu;

class DinoCleaningSampleChipsTypeItem : public MMenuItem {
public:
    DinoCleaningSampleChipsTypeItem(MMenu *menu, MSmartOam *oam, MFont *font, u32 *type, MPoint pos);
    virtual ~DinoCleaningSampleChipsTypeItem();
    virtual void Idle();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u32 *mType;             // 0x88: the chosen type
    MIndicator *mView;      // 0x8C
    MMenuItem *mKeyItemA; // 0x90
    MMenuItem *mKeyItemB; // 0x94
};

#endif
