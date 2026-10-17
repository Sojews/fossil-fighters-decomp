#ifndef GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H
#define GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCommonWindowMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoCommonWindowMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void OnIdle();
};

class DinoCommonWindowMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoCommonWindowMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

#endif
