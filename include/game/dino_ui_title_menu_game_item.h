#ifndef GAME_DINO_UI_TITLE_MENU_GAME_ITEM_H
#define GAME_DINO_UI_TITLE_MENU_GAME_ITEM_H

#include "m/menu_item.h"

// The whole screen: touching anywhere (or Start) starts the game.
class DinoUiTitleMenuGameItem : public MMenuItem {
public:
    DinoUiTitleMenuGameItem() { SetKeys(8); }
    virtual ~DinoUiTitleMenuGameItem();
    virtual MRect GetFrame();
};

#endif
