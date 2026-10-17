#ifndef GAME_DINO_UI_SHOP_BUY_MENU_H
#define GAME_DINO_UI_SHOP_BUY_MENU_H

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
