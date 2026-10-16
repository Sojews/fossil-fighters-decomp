#ifndef GAME_DINO_HELP_SAMPLE_MENU_ITEM_H
#define GAME_DINO_HELP_SAMPLE_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoHelpSampleMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoHelpSampleMenuItem();
};

#endif
