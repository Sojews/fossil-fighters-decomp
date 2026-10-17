#ifndef GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H
#define GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCleaningToolMenu : public MMenu {
public:
    virtual ~DinoCleaningToolMenu();
    virtual void OnStart();
};

#endif
