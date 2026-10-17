#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiSaveOptionMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiSaveOptionMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
};

#endif
