#ifndef GAME_DINO_UI_SHOP_BUY_MENU_H
#define GAME_DINO_UI_SHOP_BUY_MENU_H

#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_shop_buy_menu_item.h"

class MSmartOam;
class DinoUi3PartsMoney;
class DinoUiShopBuyBuyButton;
class DinoUi3PartsCursor;
class DinoUi3PartsRockName;

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
    void FinishBuy();
    void LoadFossils();
    const DinoKasekiDef *GetKasekiDef(u32 id) {
        const DinoKasekiDefs *defs = (const DinoKasekiDefs *)mFile.GetData();
        return (const DinoKasekiDef *)((const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->tableOffset))[id]);
    }
    void ClearItems();
    u8 GetFlag(int index);
    void SetFlag(int index, u8 flag);
    u32 GetPrice(int index);
    void RefreshPage2();
    void RefreshPage3();

    MArchivePtr mArchive;       // 0xB4
    MArchiveFile mFile;         // 0xB8
    MSmartOam *GetOam() { return mOam; }

    MSmartOam *mOam;            // 0xC4: the items' sprites
    MMenuItem *mUnkC8;          // 0xC8: the back button
    DinoUiShopBuyBuyButton *mLinkFrom; // 0xCC: the buy button, linked to the first page item
    u32 mUnkD0;                 // 0xD0
    std::vector<void *> mItems; // 0xD4: this page's items
    u32 mUnkE0[2];              // 0xE0
    std::vector<u32> mUnkE8;    // 0xE8
    MTaskSet *mCursorTasks;     // 0xF4
    DinoUi3PartsCursor *mCursor; // 0xF8
    DinoUi3PartsRockName *mRockName; // 0xFC
    DinoUi3PartsMoney *mMoney;  // 0x100
    u8 mFlags[8];               // 0x104: the page's chosen items
    int mPage; // 0x10C
    int mLastPage; // 0x110
    u32 mFunds;                 // 0x114
    u32 mTotal;                 // 0x118: the chosen items' price
    u8 mCount;                  // 0x11C: how many are chosen
    u8 mSelected;               // 0x11D: 0xFF for none
    BOOL mConfirmed;            // 0x120: the yes/no answer
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
