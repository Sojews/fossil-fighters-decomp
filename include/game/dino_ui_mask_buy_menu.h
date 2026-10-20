#ifndef GAME_DINO_UI_MASK_BUY_MENU_H
#define GAME_DINO_UI_MASK_BUY_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_mask_buy_menu_item.h"

class MSmartOam;
class DinoUi3PartsMoney;
class DinoUiMaskBuyBuyButton;
class MTextPicture;
class DinoUiMaskBuyMenuItem;
class DinoUi3PartsCursor;
class DinoUi3PartsMaskName;

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
    void FinishBuy();
    void LoadMasks();
    void RefreshPage1();
    void RefreshPage2();
    void RefreshPage3();

    MSmartOam *GetOam() { return mOam; }

    MSmartOam *mOam;        // 0xB4: the items' sprites
    DinoUiMaskBuyMenuItem *mBack;   // 0xB8
    DinoUiMaskBuyBuyButton *mBuy; // 0xBC
    MTextPicture *mBackground;  // 0xC0
    std::vector<void *> mUnkC4; // 0xC4: the page's mask buttons
    DinoUiMaskBuyMenuItem *mPageL;  // 0xD0
    DinoUiMaskBuyMenuItem *mPageR;  // 0xD4
    MTaskSet *mCursorTasks; // 0xD8
    DinoUi3PartsCursor *mCursor; // 0xDC
    DinoUi3PartsMaskName *mMaskName; // 0xE0
    DinoUi3PartsMoney *mMoney;  // 0xE4
    u8 mFlags[8];           // 0xE8: the page's chosen masks
    std::vector<u32> mMasks;    // 0xF0: the masks for sale
    int mPage; // 0xFC
    int mLastPage; // 0x100
    u32 mFunds;             // 0x104
    u32 mTotal;             // 0x108: the chosen masks' price
    u8 mCount;              // 0x10C: how many are chosen
    u8 mSelected;           // 0x10D: 0xFF for none
    BOOL mConfirmed;        // 0x110: the yes/no answer
};

class DinoUiMaskBuyBackButton : public DinoUiMaskBuyMenuItem {
public:
    DinoUiMaskBuyBackButton(DinoUiMaskBuyMenu *owner);
    virtual ~DinoUiMaskBuyBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMaskBuyPageLButton : public DinoUiMaskBuyMenuItem {
public:
    DinoUiMaskBuyPageLButton(DinoUiMaskBuyMenu *owner);
    virtual ~DinoUiMaskBuyPageLButton();
    virtual void OnDecide();
};

class DinoUiMaskBuyPageRButton : public DinoUiMaskBuyMenuItem {
public:
    DinoUiMaskBuyPageRButton(DinoUiMaskBuyMenu *owner);
    virtual ~DinoUiMaskBuyPageRButton();
    virtual void OnDecide();
};

class DinoUiMaskBuyBuyButton : public DinoUiMaskBuyMenuItem {
public:
    DinoUiMaskBuyBuyButton(DinoUiMaskBuyMenu *owner);
    virtual ~DinoUiMaskBuyBuyButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
