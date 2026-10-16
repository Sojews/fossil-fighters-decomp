#ifndef GAME_DINO_MAP_BACK_BUTTON_H
#define GAME_DINO_MAP_BACK_BUTTON_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoMapBackButton : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoMapBackButton();
};

#endif
