#ifndef GAME_DINO_UI_CONTAINER_DROP_MENU_PAGE_INC_TASK_MENU_H
#define GAME_DINO_UI_CONTAINER_DROP_MENU_PAGE_INC_TASK_MENU_H

#include "m/resource.h"
#include "game/dino_resource.h"
#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_container_drop_menu_item.h"

class MSmartOam;
#include "game/dino_ui3_parts_kaseki_button.h"

class DinoUi3PartsFraction;
class DinoUi3PartsCursor;
class DinoUi3PartsRockName;
struct DinoBackupEntry;
class MView;
class MTextPicture;

class DinoUiContainerDropMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiContainerDropMenu(u32 arg0, u32 arg1, const std::vector<std::string> &names);
    virtual ~DinoUiContainerDropMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void ShowPage();
    // Unlinks and deletes the page's items (leaving the list as it is).
    void DeleteItems();
    // Makes the page's kaseki buttons and links them.
    void CreateItems();

    MSmartOam *mOam;            // 0xB4: the items' sprites
    MMenuItem *mUnkB8;          // 0xB8: below the grid
    MTextPicture *mTitle;       // 0xBC
    MView *mUnkC0;              // 0xC0: shown on the last page
    std::vector<MMenuItem *> mItems;    // 0xC4: the page's kaseki buttons
    DinoUiContainerDropMenuItem *mPageL;    // 0xD0
    DinoUiContainerDropMenuItem *mPageR;    // 0xD4
    DinoUi3PartsFraction *mFraction;    // 0xD8: "page n/m"
    u32 mUnkDC;                 // 0xDC
    const DinoBackupEntry *mUnkE0;  // 0xE0: the last page's entry
    std::vector<std::string> mUnkE4; // 0xE4
    MParallelTaskSet *mCursorTasks; // 0xF0
    DinoUi3PartsCursor *mCursor;    // 0xF4
    DinoUi3PartsRockName *mRockName;    // 0xF8
    std::vector<DinoUiKasekiIcon> mIcons;   // 0xFC: the listed kaseki
    MResource<DinoKasekiDefs> mKasekiDefs;  // 0x108
    int mPage; // 0x118
    int mLastPage; // 0x11C
    int mUnk120;                // 0x120
    u8 mStarted;                // 0x124
    int mUnk128;                // 0x128: 1 closes the menu
    DinoHelpDummyItem *mHelp12C;    // 0x12C
    DinoHelpDummyItem *mHelp130;    // 0x130
};

class DinoUiContainerDropBackButton : public DinoUiContainerDropMenuItem {
public:
    DinoUiContainerDropBackButton(DinoUiContainerDropMenu *owner);
    virtual ~DinoUiContainerDropBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerDropPageLButton : public DinoUiContainerDropMenuItem {
public:
    DinoUiContainerDropPageLButton(DinoUiContainerDropMenu *owner);
    virtual ~DinoUiContainerDropPageLButton();
    virtual void OnDecide();
};

class DinoUiContainerDropPageRButton : public DinoUiContainerDropMenuItem {
public:
    DinoUiContainerDropPageRButton(DinoUiContainerDropMenu *owner);
    virtual ~DinoUiContainerDropPageRButton();
    virtual void OnDecide();
};

#endif
