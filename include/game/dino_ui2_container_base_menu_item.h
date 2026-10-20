#ifndef GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_H
#define GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUi2ContainerBaseMenu;

class DinoUi2ContainerBaseMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUi2ContainerBaseMenuItem(DinoUi2ContainerBaseMenu *owner, std::string motion, MPoint pos, u32 textId);
    virtual ~DinoUi2ContainerBaseMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUi2ContainerBaseMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUi2ContainerBaseBackButton : public DinoUi2ContainerBaseMenuItem {
public:
    DinoUi2ContainerBaseBackButton(DinoUi2ContainerBaseMenu *owner);
    virtual ~DinoUi2ContainerBaseBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUi2ContainerBasePageLButton : public DinoUi2ContainerBaseMenuItem {
public:
    DinoUi2ContainerBasePageLButton(DinoUi2ContainerBaseMenu *owner);
    virtual ~DinoUi2ContainerBasePageLButton();
    virtual void OnDecide();
};

class DinoUi2ContainerBasePageRButton : public DinoUi2ContainerBaseMenuItem {
public:
    DinoUi2ContainerBasePageRButton(DinoUi2ContainerBaseMenu *owner);
    virtual ~DinoUi2ContainerBasePageRButton();
    virtual void OnDecide();
};

class DinoUi2ContainerBaseDecideButton : public DinoUi2ContainerBaseMenuItem {
public:
    DinoUi2ContainerBaseDecideButton(DinoUi2ContainerBaseMenu *owner, std::string motion, u32 textId);
    virtual ~DinoUi2ContainerBaseDecideButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
