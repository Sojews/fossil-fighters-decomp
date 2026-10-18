#ifndef GAME_DINO_UI_ORDER_SELECT_H
#define GAME_DINO_UI_ORDER_SELECT_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/picture.h"
#include "m/resource.h"
#include <string>

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

// (The data's layout isn't known yet.)
struct DinoUiOrderSelectData;

class DinoUiOrderSelectMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiOrderSelectMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void SetPage(u8 page);
    void PageLeft();
    void PageRight();
    void ClearItems();
    void CreateItems();
    void RefreshItems();
    void RefreshOKButton();
    void FinishPage();
    void FinishTab();

    u8 unkB3[0xC0 - 0xB3];
    MMenuItem *mTabs[4];        // 0xC0
    u8 unkD0[0xD4 - 0xD0];
    MPictureFile *mPictures[4]; // 0xD4
    MMenuItem *mItems[5];       // 0xE4
    u8 unkF8[0x110 - 0xF8];
    std::string mUnk110;        // 0x110
    u8 unk11C[0x1A8 - 0x11C];
    u8 mHasFourthTab;           // 0x1A8
    u8 unk1A9[0x1EC - 0x1A9];
    u32 mFocusIndex;            // 0x1EC: the item focused after a tab change
    u32 mUnk1F0;                // 0x1F0
    u8 mPage;                   // 0x1F4
    u8 mFocusTabs;              // 0x1F5: focus the new page's tab when paging
    u8 mUnk1F6;                 // 0x1F6
    u8 unk1F7[0x20C - 0x1F7];
    MResource<DinoUiOrderSelectData> mUnk20C;   // 0x20C
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
