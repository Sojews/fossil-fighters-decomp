#ifndef GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "game/dino_ui3_parts_fraction.h"
#include "m/archive.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiContainerBaseMenu();
    virtual BOOL TryDecide() = 0;   // the decide button was pressed; FALSE if nothing happens
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void ClearItems();
    void RefreshPage2();

    DinoUi3PartsFraction *mFraction; // 0xB4: "page n/m"
    u32 mUnkB8[16];
    int mPage;                  // 0xF8
    int mLastPage;              // 0xFC
    u8 mUnk100; // 0x100
    u8 unk101[3];
    u32 mUnk104[1];
    MArchivePtr mArchive;       // 0x108
    MArchiveFile mFile;         // 0x10C
    u32 mUnk118[2];             // 0x118
    std::vector<void *> mItems; // 0x120: this page's items
    u32 mUnk12C[3];             // 0x12C
    MMenuItem *mLinkFrom;       // 0x138: linked to the first page item
    u32 mUnk13C[3];
    std::vector<u32> mUnk148;   // 0x148
};

#endif
