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

class DinoUi3PartsCursor;
class DinoUi3PartsRockName;

class DinoUiCleaningMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiCleaningMenu(s32 *result);
    virtual ~DinoUiCleaningMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    BOOL IsDecidable(u32 index);
    const DinoKasekiDef *GetKasekiDef(u32 id) {
        const DinoKasekiDefs *defs = mKasekiDefs.Get();
        return (const DinoKasekiDef *)((const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->tableOffset))[id]);
    }
    void ClearItems();
    void RefreshPage2();
    void Decide(int index);

    u32 mUnkB4;                 // 0xB4
    MMenuItem *mLinkFrom;       // 0xB8: linked to the first page item
    u32 mUnkBC;
    std::vector<void *> mItems; // 0xC0: this page's items
    u32 mUnkCC[2];              // 0xCC
    DinoUi3PartsFraction *mFraction; // 0xD4: "page n/m"
    s32 *mResult;               // 0xD8: the rock chosen, or -1
    MTaskSet *mCursorTasks;     // 0xDC
    DinoUi3PartsCursor *mCursor; // 0xE0
    DinoUi3PartsRockName *mRockName; // 0xE4
    std::vector<u32> mUnkE8;    // 0xE8
    MResource<DinoKasekiDefs> mKasekiDefs; // 0xF4
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
    DinoUiCleaningTest();
    virtual ~DinoUiCleaningTest();
    virtual void Idle();
    void Setup();
    void Leave();
    void Finish();

    void (DinoUiCleaningTest::*mState)(); // 0x74
    std::vector<void *> mUnk7C; // 0x7C
};

#endif
