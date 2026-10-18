#ifndef GAME_DINO_UI_CONTAINER_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CONTAINER_MENU_ITEM_MENU_H

#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_team.h"
#include "game/dino_ui_field_menu_fraction.h"
#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_container_menu_item.h"

class DinoUiPartsRockName;

class DinoUiContainerMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiContainerMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void OnOKDecided();
    void SelectTab(int tab);
    void Decide();
    void ClearItems();
    void RefreshPage2();

    u32 mUnkB4[3];              // 0xB4
    MMenuItem *mTabItems[3];    // 0xC0
    u32 mUnkCC;                 // 0xCC
    std::vector<void *> mItems; // 0xD0: this page's items
    u32 mUnkDC[2];              // 0xDC
    DinoUiFieldMenuFraction *mFraction; // 0xE4: "page n/m"
    int *mResult;               // 0xE8
    DinoTeam *mOldTeam;         // 0xEC: the team when the menu opened
    DinoTeam *mTeam;            // 0xF0
    MSerialTaskSet *mCursorTasks;   // 0xF4
    DinoUiFieldMenuCursor *mCursor; // 0xF8
    DinoUiPartsRockName *mPanel;    // 0xFC: shows the focused container
    std::vector<s32> mUnk100;   // 0x100
    MArchivePtr mArchive;       // 0x10C
    MArchiveFile mFile;         // 0x110
    u32 mUnk11C[2];             // 0x11C
    int mPage;                  // 0x124
    int mLastPage;              // 0x128
};

class DinoUiContainerBackButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerOKButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerPageLButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerPageLButton();
    virtual void OnDecide();
};

class DinoUiContainerPageRButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerPageRButton();
    virtual void OnDecide();
};

#endif
