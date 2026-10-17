#ifndef GAME_DINO_UI_ATM_BACK_BUTTON_H
#define GAME_DINO_UI_ATM_BACK_BUTTON_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiATMBackButton : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiATMBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

#endif
