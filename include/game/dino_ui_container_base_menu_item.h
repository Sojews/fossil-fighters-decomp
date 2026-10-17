#ifndef GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_H
#define GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiContainerBaseMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiContainerBaseMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiContainerBaseBackButton : public DinoUiContainerBaseMenuItem {
public:
    virtual ~DinoUiContainerBaseBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerBasePageLButton : public DinoUiContainerBaseMenuItem {
public:
    virtual ~DinoUiContainerBasePageLButton();
    virtual void OnDecide();
};

class DinoUiContainerBasePageRButton : public DinoUiContainerBaseMenuItem {
public:
    virtual ~DinoUiContainerBasePageRButton();
    virtual void OnDecide();
};

class DinoUiContainerBaseDecideButton : public DinoUiContainerBaseMenuItem {
public:
    virtual ~DinoUiContainerBaseDecideButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
