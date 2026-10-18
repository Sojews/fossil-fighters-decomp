#ifndef GAME_DINO_UI_ORDER_SELECT_H
#define GAME_DINO_UI_ORDER_SELECT_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include <string>

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiOrderSelectMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiOrderSelectMenu();
    virtual void OnIdle();

    void SetPage(u8 page);
    void PageLeft();
    void PageRight();

    u8 unkB3[0xC0 - 0xB3];
    MMenuItem *mTabs[4];        // 0xC0
    u8 unkD0[0x1A8 - 0xD0];
    u8 mHasFourthTab;           // 0x1A8
    u8 unk1A9[0x1F4 - 0x1A9];
    u8 mPage;                   // 0x1F4
    u8 mFocusTabs;              // 0x1F5: focus the new page's tab when paging
    u8 mUnk1F6;                 // 0x1F6
};

class DinoUiOrderSelect : public MProcess {
public:
    virtual ~DinoUiOrderSelect();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    std::string mUnk78; // 0x78
};

class DinoUiOrderSelectMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiOrderSelectMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    void *mOwner;       // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiOrderSelectBackButton : public DinoUiOrderSelectMenuItem {
public:
    virtual ~DinoUiOrderSelectBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrderSelectDecideButton : public DinoUiOrderSelectMenuItem {
public:
    virtual ~DinoUiOrderSelectDecideButton();
    virtual void OnDecide();
};

#endif
