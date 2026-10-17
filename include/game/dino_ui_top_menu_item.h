#ifndef GAME_DINO_UI_TOP_MENU_ITEM_H
#define GAME_DINO_UI_TOP_MENU_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

// A top menu button.
class DinoUiTopMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiTopMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoUiTopMenuBackButton : public DinoUiTopMenuItem {
public:
    virtual ~DinoUiTopMenuBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiTopMenuSingleButton : public DinoUiTopMenuItem {
public:
    virtual ~DinoUiTopMenuSingleButton();
    virtual void OnDecide();
};

class DinoUiTopMenuMultiButton : public DinoUiTopMenuItem {
public:
    virtual ~DinoUiTopMenuMultiButton();
    virtual void OnDecide();
};

class DinoUiTopMenuMultiDarkButton : public DinoUiTopMenuItem {
public:
    virtual ~DinoUiTopMenuMultiDarkButton();
    virtual void OnDecide();
};

#endif
