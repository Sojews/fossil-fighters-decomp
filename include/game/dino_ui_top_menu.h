#ifndef GAME_DINO_UI_TOP_MENU_H
#define GAME_DINO_UI_TOP_MENU_H

#include "m/menu.h"
#include "game/help.h"

// The title screen's top menu.
class DinoUiTopMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiTopMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    void OnBackDecided();
    void OnSingleDecided();
    void OnMultiDecided();
};

#endif
