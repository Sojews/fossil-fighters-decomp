#ifndef GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_MENU_H
#define GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_status_menu_item.h"

class DinoUiStatusMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnOKDecided();
};

class DinoUiStatusBackButton : public DinoUiStatusMenuItem {
public:
    virtual ~DinoUiStatusBackButton();
    virtual void OnDecide();
};

class DinoUiStatusOKButton : public DinoUiStatusMenuItem {
public:
    virtual ~DinoUiStatusOKButton();
    virtual void OnDecide();
};

#endif
