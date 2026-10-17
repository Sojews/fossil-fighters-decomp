#ifndef GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H

#include "m/resource.h"
#include "game/dino_ui3_parts_fraction.h"
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_cleaning_menu_item.h"

class DinoEventLoop;

class DinoUiCleaningMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiCleaningMenu(DinoEventLoop *event);
    virtual ~DinoUiCleaningMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    BOOL IsDecidable();
    void ClearItems();
    void RefreshPage2();
    void Decide(int index);

    u32 mUnkB4;                 // 0xB4
    MMenuItem *mLinkFrom;       // 0xB8: linked to the first page item
    u32 mUnkBC;
    std::vector<void *> mItems; // 0xC0: this page's items
    u32 mUnkCC[2];              // 0xCC
    DinoUi3PartsFraction *mFraction; // 0xD4: "page n/m"
    DinoEventLoop *mEvent;      // 0xD8
    u32 mUnkDC[3];              // 0xDC
    std::vector<u32> mUnkE8;    // 0xE8
    MResource<void> mResource;  // 0xF4
    u32 mUnk104[3];             // 0x104
    int mPage;                  // 0x110
    int mLastPage;              // 0x114
};

class DinoUiCleaningBackButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiCleaningPageLButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningPageLButton();
    virtual void OnDecide();
};

class DinoUiCleaningPageRButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningPageRButton();
    virtual void OnDecide();
};

class DinoUiCleaningTest : public MProcess {
public:
    virtual ~DinoUiCleaningTest();
    virtual void Idle();

    void (DinoUiCleaningTest::*mState)(); // 0x74
    std::vector<void *> mUnk7C; // 0x7C
};

#endif
