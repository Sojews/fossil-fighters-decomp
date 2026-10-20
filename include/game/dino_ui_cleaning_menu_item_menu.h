#ifndef GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H

#include "m/resource.h"
#include "game/dino_ui3_parts_fraction.h"
#include <vector>
#include "game/dino_ui_fossil_entry.h"
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_cleaning_menu_item.h"

class MSmartOam;
class MTextPicture;
class DinoEventLoop;

class DinoUi3PartsCursor;
class DinoUi3PartsRockName;

class DinoUiCleaningMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
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
    void FinishDecide();

    MSmartOam *mOam;            // 0xB4: the items' sprites
    MMenuItem *mLinkFrom;       // 0xB8: linked to the first page item
    MTextPicture *mBackground;  // 0xBC
    std::vector<void *> mItems; // 0xC0: this page's items
    MMenuItem *mPageL;          // 0xCC
    MMenuItem *mPageR;          // 0xD0
    DinoUi3PartsFraction *mFraction; // 0xD4: "page n/m"
    s32 *mResult;               // 0xD8: the rock chosen, or -1
    MTaskSet *mCursorTasks;     // 0xDC
    DinoUi3PartsCursor *mCursor; // 0xE0
    DinoUi3PartsRockName *mRockName; // 0xE4
    std::vector<DinoUiFossilEntry> mUnkE8;    // 0xE8: the rocks
    MResource<DinoKasekiDefs> mKasekiDefs; // 0xF4
    s32 mChoice;                // 0x104: the rock decided on
    u32 mAnswer;                // 0x108: the confirmation's answer
    u32 mUnk10C;                // 0x10C
    int mPage;                  // 0x110
    int mLastPage;              // 0x114
};

class DinoUiCleaningBackButton : public DinoUiCleaningMenuItem {
public:
    DinoUiCleaningBackButton(DinoUiCleaningMenu *owner);
    virtual ~DinoUiCleaningBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiCleaningPageLButton : public DinoUiCleaningMenuItem {
public:
    DinoUiCleaningPageLButton(DinoUiCleaningMenu *owner);
    virtual ~DinoUiCleaningPageLButton();
    virtual void OnDecide();
};

class DinoUiCleaningPageRButton : public DinoUiCleaningMenuItem {
public:
    DinoUiCleaningPageRButton(DinoUiCleaningMenu *owner);
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
