#ifndef GAME_DINO_UI_RV_STATUS_MENU_ITEM_MENU_H
#define GAME_DINO_UI_RV_STATUS_MENU_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_rv_status_menu_item.h"

class DinoUiRvStatusMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiRvStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    void PageRight();
    void PageLeft();
    void ShowPage();
    void SelectTab(u8 tab);

    u32 mUnkB4[4];
    MMenuItem *mTabItems[4]; // 0xC4
    u32 mUnkD4[5];
    std::vector<void *> mUnkE8; // 0xE8
    u32 mUnkF4[3];
    int mPage; // 0x100
    int mLastPage; // 0x104
    u32 mUnk108[1];
    MArchivePtr mArchive;       // 0x10C
    MArchiveFile mFile;         // 0x110
};

class DinoUiRvStatusBackButton : public DinoUiRvStatusMenuItem {
public:
    virtual ~DinoUiRvStatusBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiRvStatusPageLButton : public DinoUiRvStatusMenuItem {
public:
    virtual ~DinoUiRvStatusPageLButton();
    virtual void OnDecide();
};

class DinoUiRvStatusPageRButton : public DinoUiRvStatusMenuItem {
public:
    virtual ~DinoUiRvStatusPageRButton();
    virtual void OnDecide();
};

#endif
