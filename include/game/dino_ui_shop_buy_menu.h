#ifndef GAME_DINO_UI_SHOP_BUY_MENU_H
#define GAME_DINO_UI_SHOP_BUY_MENU_H

#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_shop_buy_menu_item.h"

class DinoUiShopBuyMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiShopBuyMenu();
    virtual ~DinoUiShopBuyMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    BOOL Buy();
    void ClearItems();
    void RefreshPage2();
    void RefreshPage3();

    MArchivePtr mArchive;       // 0xB4
    MArchiveFile mFile;         // 0xB8
    u32 mUnkC4[2];              // 0xC4
    MMenuItem *mLinkFrom;       // 0xCC: linked to the first page item
    u32 mUnkD0;                 // 0xD0
    std::vector<void *> mItems; // 0xD4: this page's items
    u32 mUnkE0[2];              // 0xE0
    std::vector<u32> mUnkE8;    // 0xE8
    u8 unkF4[24];
    int mPage; // 0x10C
    int mLastPage; // 0x110
    u8 unk114[8];
    u8 mUnk11C; // 0x11C
    u32 mUnk120;    // 0x120
};

class DinoUiShopBuyBackButton : public DinoUiShopBuyMenuItem {
public:
    virtual ~DinoUiShopBuyBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiShopBuyPageLButton : public DinoUiShopBuyMenuItem {
public:
    virtual ~DinoUiShopBuyPageLButton();
    virtual void OnDecide();
};

class DinoUiShopBuyPageRButton : public DinoUiShopBuyMenuItem {
public:
    virtual ~DinoUiShopBuyPageRButton();
    virtual void OnDecide();
};

class DinoUiShopBuyBuyButton : public DinoUiShopBuyMenuItem {
public:
    virtual ~DinoUiShopBuyBuyButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
