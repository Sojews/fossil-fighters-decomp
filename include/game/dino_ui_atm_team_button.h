#ifndef GAME_DINO_UI_ATM_TEAM_BUTTON_H
#define GAME_DINO_UI_ATM_TEAM_BUTTON_H

#include "m/menu_item.h"

class DinoUiATMTeamButton : public MMenuItem {
public:
    virtual ~DinoUiATMTeamButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
