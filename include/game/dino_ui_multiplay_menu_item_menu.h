#ifndef GAME_DINO_UI_MULTIPLAY_MENU_ITEM_MENU_H
#define GAME_DINO_UI_MULTIPLAY_MENU_ITEM_MENU_H

#include "m/motion.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "game/dino_ui_multiplay_menu_item.h"

class DinoUiMultiplayMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiMultiplayMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    void Decide(int choice);
};

class DinoUiMultiplayWirelessButton : public DinoUiMultiplayMenuItem {
public:
    virtual ~DinoUiMultiplayWirelessButton();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoUiMultiplayMenu;

class DinoUiMultiplayMenuBackButton : public DinoUiMultiplayMenuItem {
public:
    virtual ~DinoUiMultiplayMenuBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMultiplayOrganizationButton : public DinoUiMultiplayMenuItem2 {
public:
    virtual ~DinoUiMultiplayOrganizationButton();
    virtual void OnDecide();
};

#endif
