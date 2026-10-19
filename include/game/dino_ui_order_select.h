#ifndef GAME_DINO_UI_ORDER_SELECT_H
#define GAME_DINO_UI_ORDER_SELECT_H

#include "game/dino_resource.h"
#include "game/dino_backup.h"
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

class MSmartOam;
class DinoUiRvInformation;
class DinoUiOrderSelectMenuItem;
class DinoUiPartsTeamTech;
class DinoUiOrderSelectMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    virtual ~DinoUiOrderSelectMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void SetPage(u8 page);
    // Turns the information to the page; help comes back once it has.
    void SetInfoPage(u8 page);
    void PageLeft();
    void PageRight();
    void ClearItems();
    void CreateItems();
    void RefreshItems();
    void RefreshOKButton();
    BOOL IsTeamInvalid();
    void LinkItems();
    void FinishPage();
    void FinishTab();

    u8 unkB3[1];
    MSmartOam *mOam;            // 0xB4: the items' sprites
    DinoUiOrderSelectMenuItem *mBackButton; // 0xB8
    DinoUiOrderSelectMenuItem *mOKButton;   // 0xBC
    MMenuItem *mTabs[4];        // 0xC0
    u8 unkD0[0xD4 - 0xD0];
    MPictureFile *mPictures[4]; // 0xD4
    MMenuItem *mItems[5];       // 0xE4
    MMenuItem *mUnkF8[5];       // 0xF8: by tab
    u8 unk10C[0x110 - 0x10C];
    std::string mUnk110;        // 0x110
    // (What each tab shows; only the first byte is known: whether the tab is there.)
    struct Tab {
        u32 present : 8;
        u32 unk0_8 : 24;
        u8 unk04[0x14];
    } mTabData[5];              // 0x11C
    u8 unk194[0x1A8 - 0x194];
    u8 mHasFourthTab;           // 0x1A8
    u8 unk1A9[0x1B4 - 0x1A9];
    DinoUiRvInformation *mInfo; // 0x1B4
    u32 unk1B8;
    MTaskSet *mUnk1BC;          // 0x1BC
    u8 unk1C0[0x1C4 - 0x1C0];
    DinoTeam mTeam;             // 0x1C4
    u32 mFocusIndex;            // 0x1EC: the item focused after a tab change
    u32 mUnk1F0;                // 0x1F0
    u8 mPage;                   // 0x1F4
    u8 mFocusTabs;              // 0x1F5: focus the new page's tab when paging
    u8 mUnk1F6;                 // 0x1F6
    u8 unk1F7[0x1FC - 0x1F7];
    u8 mItemsLinked;            // 0x1FC
    u8 unk1FD[0x208 - 0x1FD];
    DinoUiPartsTeamTech *mTech; // 0x208: shown when the team has a technique
    MResource<DinoUiOrderSelectData> mUnk20C;   // 0x20C
};

class DinoUiOrderSelect : public MProcess {
public:
    virtual ~DinoUiOrderSelect();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    std::string mUnk78; // 0x78
};

class DinoUiOrderSelectMenu;

class DinoUiOrderSelectMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiOrderSelectMenuItem(DinoUiOrderSelectMenu *owner, const std::string &path, MPoint pos, u32 textId);
    virtual ~DinoUiOrderSelectMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiOrderSelectMenu *mOwner;  // 0x94
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
