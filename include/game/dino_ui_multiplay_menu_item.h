#ifndef GAME_DINO_UI_MULTIPLAY_MENU_ITEM_H
#define GAME_DINO_UI_MULTIPLAY_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiMultiplayMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMultiplayMenuItem();
};

#endif
