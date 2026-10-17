#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_save_option_menu_item.h"

class DinoUiSaveOptionMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiSaveOptionMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    void RemoveItem();
};

class DinoUiSaveOptionSaveButton : public DinoUiSaveOptionMenuItem {
public:
    virtual ~DinoUiSaveOptionSaveButton();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoUiSaveOptionOptionButton : public DinoUiSaveOptionMenuItem {
public:
    virtual ~DinoUiSaveOptionOptionButton();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoUiSaveOptionBackButton : public DinoUiSaveOptionMenuItem {
public:
    virtual ~DinoUiSaveOptionBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
