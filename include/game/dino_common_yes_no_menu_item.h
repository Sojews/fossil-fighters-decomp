#ifndef GAME_DINO_COMMON_YES_NO_MENU_ITEM_H
#define GAME_DINO_COMMON_YES_NO_MENU_ITEM_H

#include "game/help.h"
#include "m/menu_item.h"

class DinoCommonYesNoMenuItem : public DinoHelpItem, public MMenuItem {
public:
    virtual ~DinoCommonYesNoMenuItem();
};

#endif
