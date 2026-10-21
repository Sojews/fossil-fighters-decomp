#ifndef GAME_DINO_UI_ATM_TEAM_BUTTON_H
#define GAME_DINO_UI_ATM_TEAM_BUTTON_H

#include "m/motion.h"
#include "m/menu_item.h"

class DinoUiATMMenu;

class DinoUiATMTeamButton : public MMenuItem {
public:
    DinoUiATMTeamButton(DinoUiATMMenu *owner, MPoint pos);
    virtual ~DinoUiATMTeamButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    MMotion *mMotion;   // 0x88
    DinoUiATMMenu *mOwner;  // 0x8C
    MPoint mPos;        // 0x90
};

#endif
