#ifndef GAME_DINO_UI_SHOP_SELL_MENU_H
#define GAME_DINO_UI_SHOP_SELL_MENU_H

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
