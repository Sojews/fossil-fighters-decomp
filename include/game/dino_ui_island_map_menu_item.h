#ifndef GAME_DINO_UI_ISLAND_MAP_MENU_ITEM_H
#define GAME_DINO_UI_ISLAND_MAP_MENU_ITEM_H

#include "m/motion.h"
#include "m/menu_item.h"

class DinoUiIslandMapMenu;

class MSmartOam;

class DinoUiIslandMapMenuItem : public MMenuItem {
public:
    DinoUiIslandMapMenuItem(DinoUiIslandMapMenu *owner, const std::string &path, MPoint pos, u32 spot, MSmartOam *oam);
    virtual ~DinoUiIslandMapMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();

    DinoUiIslandMapMenu *mOwner; // 0x88
    MMotion *mMotion;   // 0x8C
    u32 mSpot;          // 0x90
};

#endif
