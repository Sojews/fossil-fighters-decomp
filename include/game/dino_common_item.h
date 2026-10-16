#ifndef GAME_DINO_COMMON_ITEM_H
#define GAME_DINO_COMMON_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoCommonItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoCommonItem();
};

#endif
