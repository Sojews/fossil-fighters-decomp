#ifndef GAME_DINO_UI_ITEM_ITEM_BUTTON_MENU_H
#define GAME_DINO_UI_ITEM_ITEM_BUTTON_MENU_H

#include "game/dino_team.h"
#include "game/dino_ui_field_menu_fraction.h"
#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_item_menu_item.h"

struct DinoTeam;
class MSmartOam;
class DinoUiItemMenuItem;

class DinoUiItemMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiItemMenu(int *choice, DinoTeam *team, DinoTeam *team2);
    virtual ~DinoUiItemMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    void PageRight();
    void PageLeft();
    void OnOKDecided();
    void SelectTab(int tab);
    void RefreshPage();
    void RefreshOKButton();

    MSmartOam *GetOam() { return mOam; }

    MSmartOam *mOam;            // 0xB4: the items' sprites
    MMenuItem *mUnkB8;          // 0xB8: focused after a page change
    DinoUiItemMenuItem *mOKButton;  // 0xBC
    u32 mUnkC0[7];
    MMenuItem *mTabItems[3]; // 0xDC
    u32 mUnkE8;
    int *mResult;               // 0xEC
    DinoTeam *mOldTeam;         // 0xF0: the team when the menu opened
    DinoTeam *mTeam;            // 0xF4
    u32 mUnkF8[2];
    DinoUiFieldMenuFraction *mFraction; // 0x100: "page n/m"
    u32 mUnk104[6];
    std::vector<void *> mUnk11C; // 0x11C
    int mPage;                  // 0x128
    int mLastPage;              // 0x12C
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
