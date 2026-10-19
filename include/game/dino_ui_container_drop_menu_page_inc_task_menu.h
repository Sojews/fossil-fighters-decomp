#ifndef GAME_DINO_UI_CONTAINER_DROP_MENU_PAGE_INC_TASK_MENU_H
#define GAME_DINO_UI_CONTAINER_DROP_MENU_PAGE_INC_TASK_MENU_H

#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_container_drop_menu_item.h"

class MSmartOam;
class DinoUiContainerDropMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    virtual ~DinoUiContainerDropMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void ShowPage();

    MSmartOam *mOam;            // 0xB4: the items' sprites
    u32 mUnkB8[3];              // 0xB8
    std::vector<void *> mUnkC4; // 0xC4
    u32 mUnkD0[5];              // 0xD0
    std::vector<std::string> mUnkE4; // 0xE4
    u32 mUnkF0[3];              // 0xF0
    std::vector<u32> mUnkFC;    // 0xFC
    MArchivePtr mArchive;       // 0x108
    MArchiveFile mFile;         // 0x10C
    int mPage; // 0x118
    int mLastPage; // 0x11C
    u32 mUnk120;                // 0x120
    u8 mStarted;                // 0x124
    int mUnk128;                // 0x128: 1 closes the menu
};

class DinoUiContainerDropBackButton : public DinoUiContainerDropMenuItem {
public:
    virtual ~DinoUiContainerDropBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerDropPageLButton : public DinoUiContainerDropMenuItem {
public:
    virtual ~DinoUiContainerDropPageLButton();
    virtual void OnDecide();
};

class DinoUiContainerDropPageRButton : public DinoUiContainerDropMenuItem {
public:
    virtual ~DinoUiContainerDropPageRButton();
    virtual void OnDecide();
};

#endif
