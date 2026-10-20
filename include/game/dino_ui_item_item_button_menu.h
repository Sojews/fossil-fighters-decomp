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

class MIndicator;

class DinoUiFieldMenuCursor;
class MConsole;
class MTextPicture;

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
    DinoUiItemMenuItem *mPageL; // 0xC0: (only with more than one page)
    DinoUiItemMenuItem *mPageR; // 0xC4
    MMenuItem *mItems[5];       // 0xC8: the page's item buttons
    MMenuItem *mTabItems[3]; // 0xDC
    MTextPicture *mBackground;  // 0xE8
    int *mResult;               // 0xEC
    DinoTeam *mOldTeam;         // 0xF0: the team when the menu opened
    DinoTeam *mTeam;            // 0xF4
    MTaskSet *mCursorTasks;     // 0xF8
    DinoUiFieldMenuCursor *mCursor; // 0xFC
    DinoUiFieldMenuFraction *mFraction; // 0x100: "page n/m"
    MIndicator *mLabels[5];     // 0x104: the page's item names
    MConsole *mConsole;         // 0x118: the focused item's description
    std::vector<void *> mKeyItems; // 0x11C: the items the player has (ids)
    int mPage;                  // 0x128
    int mLastPage;              // 0x12C
};

class DinoUiItemBackButton : public DinoUiItemMenuItem {
public:
    DinoUiItemBackButton(DinoUiItemMenu *owner);
    virtual ~DinoUiItemBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiItemOKButton : public DinoUiItemMenuItem {
public:
    DinoUiItemOKButton(DinoUiItemMenu *owner);
    virtual ~DinoUiItemOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiItemPageLButton : public DinoUiItemMenuItem {
public:
    DinoUiItemPageLButton(DinoUiItemMenu *owner);
    virtual ~DinoUiItemPageLButton();
    virtual void OnDecide();
};

class DinoUiItemPageRButton : public DinoUiItemMenuItem {
public:
    DinoUiItemPageRButton(DinoUiItemMenu *owner);
    virtual ~DinoUiItemPageRButton();
    virtual void OnDecide();
};

#endif
