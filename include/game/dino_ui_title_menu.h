#ifndef GAME_DINO_UI_TITLE_MENU_H
#define GAME_DINO_UI_TITLE_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiTitleMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoUiTitleMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

#endif
