#ifndef GAME_DINO_CLEANING_XRAY_ITEM_H
#define GAME_DINO_CLEANING_XRAY_ITEM_H

#include "game/help.h"
#include "m/menu_item.h"

class DinoCleaningXrayItem : public DinoHelpItem, public MMenuItem {
public:
    virtual ~DinoCleaningXrayItem();
};

#endif
