#ifndef GAME_DINO_UI_MASK_BUY_MENU_H
#define GAME_DINO_UI_MASK_BUY_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_mask_buy_menu_item.h"

class DinoUi3PartsMoney;

class DinoUiMaskBuyMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiMaskBuyMenu();
    virtual ~DinoUiMaskBuyMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    u8 GetFlag(int index);
    void SetFlag(int index, u8 flag);
    u32 GetPrice(int index);
    BOOL Buy();
    void RefreshPage1();
    void RefreshPage2();
    void RefreshPage3();

    u32 mUnkB4[4];          // 0xB4
    std::vector<void *> mUnkC4; // 0xC4
    u32 mUnkD0[5];          // 0xD0
    DinoUi3PartsMoney *mMoney;  // 0xE4
    u8 mFlags[8];           // 0xE8: the page's chosen masks
    std::vector<u32> mMasks;    // 0xF0: the masks for sale
    int mPage; // 0xFC
    int mLastPage; // 0x100
    u32 mFunds;             // 0x104
    u32 mTotal;             // 0x108: the chosen masks' price
    u8 mCount;              // 0x10C: how many are chosen
    u32 mUnk110;    // 0x110
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
