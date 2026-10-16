#ifndef GAME_DINO_CLEANING_TOOL_MENU_ITEM_H
#define GAME_DINO_CLEANING_TOOL_MENU_ITEM_H

#include "game/help.h"
#include "m/menu_item.h"

class DinoCleaningToolMenuItem : public DinoHelpItem, public MMenuItem {
public:
    virtual ~DinoCleaningToolMenuItem();
};

#endif
