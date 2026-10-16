#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_ITEM_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiSaveOptionMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiSaveOptionMenuItem();
};

#endif
