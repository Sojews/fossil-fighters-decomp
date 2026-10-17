#ifndef GAME_DINO_UI_ITEM_ITEM_BUTTON_MENU_H
#define GAME_DINO_UI_ITEM_ITEM_BUTTON_MENU_H

#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_item_menu_item.h"

class DinoUiItemMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiItemMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    void PageRight();
    void PageLeft();
    void OnOKDecided();
    void SelectTab(int tab);

    u32 mUnkB4[10];
    MMenuItem *mTabItems[3]; // 0xDC
    u32 mUnkE8[13];
    std::vector<void *> mUnk11C; // 0x11C
};

class DinoUiItemBackButton : public DinoUiItemMenuItem {
public:
    virtual ~DinoUiItemBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiItemOKButton : public DinoUiItemMenuItem {
public:
    virtual ~DinoUiItemOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiItemPageLButton : public DinoUiItemMenuItem {
public:
    virtual ~DinoUiItemPageLButton();
    virtual void OnDecide();
};

class DinoUiItemPageRButton : public DinoUiItemMenuItem {
public:
    virtual ~DinoUiItemPageRButton();
    virtual void OnDecide();
};

#endif
