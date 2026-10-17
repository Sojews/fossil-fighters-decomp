#ifndef GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_H
#define GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUi2ContainerBaseMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUi2ContainerBaseMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUi2ContainerBaseBackButton : public DinoUi2ContainerBaseMenuItem {
public:
    virtual ~DinoUi2ContainerBaseBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUi2ContainerBasePageLButton : public DinoUi2ContainerBaseMenuItem {
public:
    virtual ~DinoUi2ContainerBasePageLButton();
    virtual void OnDecide();
};

class DinoUi2ContainerBasePageRButton : public DinoUi2ContainerBaseMenuItem {
public:
    virtual ~DinoUi2ContainerBasePageRButton();
    virtual void OnDecide();
};

class DinoUi2ContainerBaseDecideButton : public DinoUi2ContainerBaseMenuItem {
public:
    virtual ~DinoUi2ContainerBaseDecideButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
