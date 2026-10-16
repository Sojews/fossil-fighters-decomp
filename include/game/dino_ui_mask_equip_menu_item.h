#ifndef GAME_DINO_UI_MASK_EQUIP_MENU_ITEM_H
#define GAME_DINO_UI_MASK_EQUIP_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiMaskEquipMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMaskEquipMenuItem();
};

#endif
