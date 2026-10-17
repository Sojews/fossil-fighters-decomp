#ifndef GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H
#define GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCleaningStopConfirmMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoCleaningStopConfirmMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

#endif
