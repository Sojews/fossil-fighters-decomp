#ifndef GAME_DINO_UI_MASK_BUY_MENU_H
#define GAME_DINO_UI_MASK_BUY_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_mask_buy_menu_item.h"

class DinoUiMaskBuyMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiMaskBuyMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    BOOL Buy();

    u32 mUnkB4[4];          // 0xB4
    std::vector<void *> mUnkC4; // 0xC4
    u32 mUnkD0[8];          // 0xD0
    std::vector<void *> mUnkF0; // 0xF0
    u8 unkFC[0x10];
    u8 mUnk10C; // 0x10C
};

class DinoUiMaskBuyBackButton : public DinoUiMaskBuyMenuItem {
public:
    virtual ~DinoUiMaskBuyBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMaskBuyPageLButton : public DinoUiMaskBuyMenuItem {
public:
    virtual ~DinoUiMaskBuyPageLButton();
    virtual void OnDecide();
};

class DinoUiMaskBuyPageRButton : public DinoUiMaskBuyMenuItem {
public:
    virtual ~DinoUiMaskBuyPageRButton();
    virtual void OnDecide();
};

class DinoUiMaskBuyBuyButton : public DinoUiMaskBuyMenuItem {
public:
    virtual ~DinoUiMaskBuyBuyButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
