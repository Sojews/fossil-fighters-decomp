#ifndef GAME_DINO_UI_SHOP_SELL_MENU_H
#define GAME_DINO_UI_SHOP_SELL_MENU_H

#include "game/dino_ui3_parts_fraction.h"
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_shop_sell_menu_item.h"

class DinoUiShopSellMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiShopSellMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    BOOL Sell();
    void ClearItems();
    void RefreshPage2();

    MArchivePtr mArchive;       // 0xB4
    MArchiveFile mFile;         // 0xB8
    u32 mUnkC4[2];              // 0xC4
    MMenuItem *mLinkFrom;       // 0xCC: linked to the first page item
    u32 mUnkD0;
    std::vector<void *> mItems; // 0xD4: this page's items
    u32 mUnkE0[2];              // 0xE0
    DinoUi3PartsFraction *mFraction; // 0xE8: "page n/m"
    u32 mUnkEC;
    MView *mCursor;             // 0xF0
    u32 mUnkF4;
    std::vector<u32> mUnkF8;    // 0xF8
    u32 mUnk104[17];            // 0x104
    int mPage;                  // 0x148
    int mLastPage;              // 0x14C
    u8 mUnk150;                 // 0x150
    u32 mUnk154;
    MMenuItem *mUnk158;         // 0x158
};

class DinoUiShopSellBackButton : public DinoUiShopSellMenuItem {
public:
    virtual ~DinoUiShopSellBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiShopSellPageLButton : public DinoUiShopSellMenuItem {
public:
    virtual ~DinoUiShopSellPageLButton();
    virtual void OnDecide();
};

class DinoUiShopSellPageRButton : public DinoUiShopSellMenuItem {
public:
    virtual ~DinoUiShopSellPageRButton();
    virtual void OnDecide();
};

class DinoUiShopSellSellButton : public DinoUiShopSellMenuItem {
public:
    virtual ~DinoUiShopSellSellButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
