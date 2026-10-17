#ifndef GAME_DINO_UI_ISLAND_MAP_MENU_H
#define GAME_DINO_UI_ISLAND_MAP_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiIslandMapMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiIslandMapMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

#endif
