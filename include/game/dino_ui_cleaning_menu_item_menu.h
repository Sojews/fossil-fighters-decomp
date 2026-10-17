#ifndef GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_cleaning_menu_item.h"

class DinoUiCleaningMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiCleaningMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

class DinoUiCleaningBackButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiCleaningPageLButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningPageLButton();
    virtual void OnDecide();
};

class DinoUiCleaningPageRButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningPageRButton();
    virtual void OnDecide();
};

#endif
