#ifndef GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "m/blend.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUi2ContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUi2ContainerBaseMenu();
    virtual BOOL TryDecide() = 0;   // the decide button was pressed; FALSE if nothing happens

    u32 mUnkB4[16];
    int mPage; // 0xF4
    int mLastPage; // 0xF8
    u8 mUnkFC;                      // 0xFC
    u32 mUnk100[3];                 // 0x100
    std::vector<void *> mItems;     // 0x10C: this page's items
    u32 mUnk118[3];                 // 0x118
    MMenuItem *mLinkFrom;           // 0x124: linked to the first page item
    u32 mUnk128[3];
    std::vector<unsigned short> mUnk134; // 0x134: (the element type is a guess, distinct from other overlays')
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void ClearItems();
    void RefreshPage2();
};

class DinoUi2CannonShootMenu : public DinoUi2ContainerBaseMenu {
public:
    virtual ~DinoUi2CannonShootMenu();
};

#endif
