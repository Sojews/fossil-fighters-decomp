#ifndef GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_H
#define GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiContainerBaseMenu;

class DinoUiContainerBaseMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiContainerBaseMenuItem(DinoUiContainerBaseMenu *owner, std::string motion, MPoint pos, u32 textId);
    virtual ~DinoUiContainerBaseMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiContainerBaseMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiContainerBaseBackButton : public DinoUiContainerBaseMenuItem {
public:
    DinoUiContainerBaseBackButton(DinoUiContainerBaseMenu *owner, u32 textId);
    virtual ~DinoUiContainerBaseBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerBasePageLButton : public DinoUiContainerBaseMenuItem {
public:
    DinoUiContainerBasePageLButton(DinoUiContainerBaseMenu *owner);
    virtual ~DinoUiContainerBasePageLButton();
    virtual void OnDecide();
};

class DinoUiContainerBasePageRButton : public DinoUiContainerBaseMenuItem {
public:
    DinoUiContainerBasePageRButton(DinoUiContainerBaseMenu *owner);
    virtual ~DinoUiContainerBasePageRButton();
    virtual void OnDecide();
};

class DinoUiContainerBaseDecideButton : public DinoUiContainerBaseMenuItem {
public:
    DinoUiContainerBaseDecideButton(DinoUiContainerBaseMenu *owner, std::string motion, u32 textId);
    virtual ~DinoUiContainerBaseDecideButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
