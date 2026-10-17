#ifndef GAME_DINO_UI_RV_STATUS_MENU_ITEM_MENU_H
#define GAME_DINO_UI_RV_STATUS_MENU_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_rv_status_menu_item.h"

class DinoUiRvStatusMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiRvStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

class DinoUiRvStatusBackButton : public DinoUiRvStatusMenuItem {
public:
    virtual ~DinoUiRvStatusBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiRvStatusPageLButton : public DinoUiRvStatusMenuItem {
public:
    virtual ~DinoUiRvStatusPageLButton();
    virtual void OnDecide();
};

class DinoUiRvStatusPageRButton : public DinoUiRvStatusMenuItem {
public:
    virtual ~DinoUiRvStatusPageRButton();
    virtual void OnDecide();
};

#endif
