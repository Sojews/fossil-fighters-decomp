#ifndef GAME_DINO_UI_ATM_MENU_H
#define GAME_DINO_UI_ATM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiATMMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiATMMenu();
    void OnBackDecided();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

#endif
