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
    virtual ~DinoUiShopBuyMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();

    MArchivePtr mArchive;       // 0xB4
    MArchiveFile mFile;         // 0xB8
    u32 mUnkC4[4];              // 0xC4
    std::vector<void *> mUnkD4; // 0xD4
    u32 mUnkE0[2];              // 0xE0
    std::vector<u32> mUnkE8;    // 0xE8
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
