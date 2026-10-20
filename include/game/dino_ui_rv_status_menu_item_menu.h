#ifndef GAME_DINO_UI_RV_STATUS_MENU_ITEM_MENU_H
#define GAME_DINO_UI_RV_STATUS_MENU_ITEM_MENU_H

#include "game/dino_ui_field_menu_cursor.h"
#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_rv_status_menu_item.h"

class MSmartOam;
struct DinoTeam;
class DinoUiRvInformation;
class DinoUiFieldMenuFraction4;

class DinoUiRvStatusMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    // The saved team's creatures, starting at *page.
    DinoUiRvStatusMenu(int *page);
    // All the creatures with any part, in the sort's order (see DinoUiMedalMenu::Sort), starting at *page.
    DinoUiRvStatusMenu(int *page, u8 sort);
    // The team's creatures, starting at slot page.
    DinoUiRvStatusMenu(int page, const DinoTeam *team);
    virtual ~DinoUiRvStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    void PageRight();
    void PageLeft();
    void ShowPage();
    void SelectTab(u8 tab);
    // Turns the information to the page; help comes back once it has.
    void SetInfoPage(u8 page);
    void RefreshTab();

    MSmartOam *mOam;    // 0xB4: the items' sprites
    DinoUiRvStatusMenuItem *mBack;  // 0xB8
    DinoUiRvStatusMenuItem *mPageL; // 0xBC: (only with more than one page)
    DinoUiRvStatusMenuItem *mPageR; // 0xC0
    MMenuItem *mTabItems[4]; // 0xC4
    MParallelTaskSet *mCursorTasks; // 0xD4
    DinoUiFieldMenuCursor *mCursor; // 0xD8
    DinoUiRvInformation *mInfo;         // 0xDC
    DinoUiFieldMenuFraction4 *mFraction;    // 0xE0
    MParallelTaskSet *mUnkE4;   // 0xE4
    std::vector<u32> mCreatures;    // 0xE8: a page each
    MTaskSet *mUnkF4;   // 0xF4
    int *mResult;       // 0xF8: gets the page left on
    u32 mUnkFC;
    int mPage; // 0x100
    int mLastPage; // 0x104
    u8 mTab;            // 0x108
    u8 mUnk109;
    u8 mUnk10A;
    MResource<DinoKasekiDefs> mKasekiDefs; // 0x10C
    DinoHelpDummyItem *mHelp11C;    // 0x11C
    DinoHelpDummyItem *mHelp120;    // 0x120
    u8 unk124[0x148 - 0x124];
};

class DinoUiRvStatusBackButton : public DinoUiRvStatusMenuItem {
public:
    DinoUiRvStatusBackButton(DinoUiRvStatusMenu *owner);
    virtual ~DinoUiRvStatusBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiRvStatusPageLButton : public DinoUiRvStatusMenuItem {
public:
    DinoUiRvStatusPageLButton(DinoUiRvStatusMenu *owner);
    virtual ~DinoUiRvStatusPageLButton();
    virtual void OnDecide();
};

class DinoUiRvStatusPageRButton : public DinoUiRvStatusMenuItem {
public:
    DinoUiRvStatusPageRButton(DinoUiRvStatusMenu *owner);
    virtual ~DinoUiRvStatusPageRButton();
    virtual void OnDecide();
};

#endif
