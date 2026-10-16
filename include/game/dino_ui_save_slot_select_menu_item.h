#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_ITEM_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiSaveSlotSelectMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiSaveSlotSelectMenuItem();
};

#endif
