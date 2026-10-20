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

// What a tab of the order select shows; only the first byte is known: whether the tab is there.
struct DinoUiOrderSelectTab {
    u32 present : 8;
    u32 unk0_8 : 24;
    u8 unk04[0x14];
};

// The opponent the order is chosen against.
struct DinoUiOrderSelectInfo {
    std::string name;               // 0x00
    DinoCreatureState creatures[5]; // 0x0C: the opponent's team
    MFx32 unk84[5];                 // 0x84 (some 4-byte class: the game copies them one by one)
};

class MSmartOam;
class DinoUiRvInformation;
class DinoUiOrderSelectMenuItem;
class DinoUiPartsTeamTech;
class DinoUiPartsReviverName;
class DinoCommonWindowMessage;
class DinoHelpDummyItem;
class DinoUiOrderSelectMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiOrderSelectMenu(u32 team, const DinoUiOrderSelectInfo *opponent, u8 hasFourthTab, u8 unk1F7, u32 unk1F8,
                          u8 unk1FD);
    MSmartOam *GetOam() { return mOam; }
    virtual ~DinoUiOrderSelectMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void SetPage(u8 page);
    // Turns the information to the page; help comes back once it has.
    void SetInfoPage(u8 page);
    void ShowPage(u8 page);
    void PageLeft();
    void PageRight();
    void ClearItems();
    void CreateItems();
    void RefreshItems();
    // While an item is picked: the others and the back button can be chosen instead.
    void SelectItem(int index);
    void RefreshOKButton();
    BOOL IsTeamInvalid();
    void LinkItems();
    void FinishPage();
    void FinishTab();
    // The opponent's team's medals (on the right of the four tabs, or of three).
    void CreateOpponentItems();
    // The player's, the opponent's and the team's names.
    void CreateNameLabels(const wchar_t *playerName);
    // The decide and back buttons: cancels a swap, or (if the team can fight) saves it and leaves, else says why not.
    void Confirm();
    // The battle's rule text (empty for none).
    std::string GetUnk44Text();
    std::string GetUnk3CText();

    u8 unkB3[1];
    MSmartOam *mOam;            // 0xB4: the items' sprites
    DinoUiOrderSelectMenuItem *mBackButton; // 0xB8
    DinoUiOrderSelectMenuItem *mOKButton;   // 0xBC
    MMenuItem *mTabs[4];        // 0xC0
    MTextPicture *mUnkD0;       // 0xD0: the page's title
    MPictureFile *mPictures[4]; // 0xD4
    MMenuItem *mItems[5];       // 0xE4
    MMenuItem *mUnkF8[5];       // 0xF8: by tab
    u32 mTeamIndex;             // 0x10C
    DinoUiOrderSelectInfo mOpponent;    // 0x110
    u8 mHasFourthTab;           // 0x1A8
    u8 unk1A9[0x1AC - 0x1A9];
    MTaskSet *mCursorTasks;     // 0x1AC: the cursor's moves
    class DinoUiFieldMenuCursor *mCursor;   // 0x1B0
    DinoUiRvInformation *mInfo; // 0x1B4
    DinoUiPartsReviverName *mName;  // 0x1B8: the focused member's
    MTaskSet *mUnk1BC;          // 0x1BC
    DinoCommonWindowMessage *mMessage;  // 0x1C0: "swap with whom?"
    DinoTeam mTeam;             // 0x1C4
    u32 mFocusIndex;            // 0x1EC: the item focused after a tab change
    u32 mUnk1F0;                // 0x1F0
    u8 mPage;                   // 0x1F4
    u8 mFocusTabs;              // 0x1F5: focus the new page's tab when paging
    u8 mUnk1F6;                 // 0x1F6
    u8 mUnk1F7;                 // 0x1F7
    u32 mUnk1F8;                // 0x1F8
    u8 mItemsLinked;            // 0x1FC
    u8 mUnk1FD;                 // 0x1FD
    u8 unk1FE[2];
    MTaskSet *mUnk200;          // 0x200
    u8 mUnk204;                 // 0x204
    u8 mUnk205;                 // 0x205
    DinoUiPartsTeamTech *mTech; // 0x208: shown when the team has a technique
    MResource<DinoKasekiDefs> mKasekiDefs;      // 0x20C
    DinoHelpDummyItem *mHelp21C;    // 0x21C: help areas on the top screen
    DinoHelpDummyItem *mHelp220;    // 0x220
    u8 unk224[0x248 - 0x224];
    DinoHelpDummyItem *mHelps[7];   // 0x248
};

class DinoUiOrderSelect : public MProcess {
public:
    DinoUiOrderSelect(u32 team, const DinoUiOrderSelectInfo &info, u8 unk110, u8 unk111, u32 unk114, u8 unk112);
    virtual ~DinoUiOrderSelect();
    virtual void Idle();

    u32 mTeam;                  // 0x74
    DinoUiOrderSelectInfo mInfo;    // 0x78
    u8 mUnk110;                 // 0x110
    u8 mUnk111;                 // 0x111
    u8 mUnk112;                 // 0x112
    u32 mUnk114;                // 0x114
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
    DinoUiOrderSelectBackButton(DinoUiOrderSelectMenu *owner);
    virtual ~DinoUiOrderSelectBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrderSelectDecideButton : public DinoUiOrderSelectMenuItem {
public:
    DinoUiOrderSelectDecideButton(DinoUiOrderSelectMenu *owner);
    virtual ~DinoUiOrderSelectDecideButton();
    virtual void OnDecide();
};

#endif
