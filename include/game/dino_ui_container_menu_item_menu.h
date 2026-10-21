#ifndef GAME_DINO_UI_CONTAINER_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CONTAINER_MENU_ITEM_MENU_H

#include "m/resource.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_team.h"
#include "game/dino_ui_field_menu_fraction.h"
#include <string>
#include <vector>
#include "game/dino_ui_fossil_entry.h"
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_container_menu_item.h"

struct DinoTeam;
class MSmartOam;
class MTextPicture;
class DinoUiPartsRockName;

class DinoUiContainerMenuItem;

class DinoUiContainerMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiContainerMenu(int *choice, DinoTeam *team, DinoTeam *team2);
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
    void Decide(int index);
    void ClearItems();
    void RefreshEntries();
    void RefreshPage2();
    void RefreshOKButton();
    // Throws the chosen entry away, if the player said yes.
    void FinishDiscard();

    MSmartOam *mOam;            // 0xB4: the items' sprites
    DinoUiContainerMenuItem *mBack;     // 0xB8
    DinoUiContainerMenuItem *mOKButton; // 0xBC
    MMenuItem *mTabItems[3];    // 0xC0
    MTextPicture *mTitle;       // 0xCC
    std::vector<void *> mItems; // 0xD0: this page's items
    DinoUiContainerMenuItem *mPageL;    // 0xDC
    DinoUiContainerMenuItem *mPageR;    // 0xE0
    DinoUiFieldMenuFraction *mFraction; // 0xE4: "page n/m"
    int *mResult;               // 0xE8
    DinoTeam *mOldTeam;         // 0xEC: the team when the menu opened
    DinoTeam *mTeam;            // 0xF0
    MParallelTaskSet *mCursorTasks; // 0xF4
    DinoUiFieldMenuCursor *mCursor; // 0xF8
    DinoUiPartsRockName *mPanel;    // 0xFC: shows the focused container
    std::vector<DinoUiFossilEntry> mUnk100;   // 0x100: the containers' rocks
    MResource<DinoKasekiDefs> mKasekiDefs;  // 0x10C
    u32 mUnk11C;                // 0x11C: the chosen entry
    u32 mUnk120;                // 0x120: the yes/no window's answer
    int mPage;                  // 0x124
    int mLastPage;              // 0x128
    u8 unk12C[0x130 - 0x12C];
};

class DinoUiContainerBackButton : public DinoUiContainerMenuItem {
public:
    DinoUiContainerBackButton(DinoUiContainerMenu *owner);
    virtual ~DinoUiContainerBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerOKButton : public DinoUiContainerMenuItem {
public:
    DinoUiContainerOKButton(DinoUiContainerMenu *owner);
    virtual ~DinoUiContainerOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerPageLButton : public DinoUiContainerMenuItem {
public:
    DinoUiContainerPageLButton(DinoUiContainerMenu *owner);
    virtual ~DinoUiContainerPageLButton();
    virtual void OnDecide();
};

class DinoUiContainerPageRButton : public DinoUiContainerMenuItem {
public:
    DinoUiContainerPageRButton(DinoUiContainerMenu *owner);
    virtual ~DinoUiContainerPageRButton();
    virtual void OnDecide();
};

#endif
