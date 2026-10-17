#ifndef GAME_DINO_UI_CLEANING_MENU_ITEM_H
#define GAME_DINO_UI_CLEANING_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiCleaningMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiCleaningMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

#endif
