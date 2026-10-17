#ifndef GAME_DINO_UI_ISLAND_MAP_MENU_ITEM_H
#define GAME_DINO_UI_ISLAND_MAP_MENU_ITEM_H

#include "m/menu_item.h"

class DinoUiIslandMapMenuItem : public MMenuItem {
public:
    virtual ~DinoUiIslandMapMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
};

#endif
