#ifndef GAME_DINO_UI_SHOP_SELL_MENU_H
#define GAME_DINO_UI_SHOP_SELL_MENU_H

#include "m/resource.h"
#include "game/dino_ui3_parts_fraction.h"
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_shop_sell_menu_item.h"

class MSmartOam;
class DinoUi3PartsMoney;
class DinoUiShopSellSellButton;
class DinoUiShopSellPageLButton;
class DinoUiShopSellPageRButton;
class DinoUi3PartsCursor;
class DinoUi3PartsRockName;

class DinoUiShopSellMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiShopSellMenu();
    virtual ~DinoUiShopSellMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    BOOL Sell();
    void FinishSell();
    void LoadFossils();
    void ClearItems();
    u8 GetFlag(int index);
    void SetFlag(int index, u8 flag);
    u32 GetPrice(int index);
    void RefreshTotal();
    const DinoKasekiDef *GetKasekiDef(u32 id) {
        const DinoKasekiDefs *defs = mKasekiDefs.Get();
        return (const DinoKasekiDef *)((const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->tableOffset))[id]);
    }
    void RefreshPage2();

    MResource<DinoKasekiDefs> mKasekiDefs;  // 0xB4
    MSmartOam *mOam;            // 0xC4: the items' sprites
    MMenuItem *mBack;           // 0xC8
    DinoUiShopSellSellButton *mLinkFrom; // 0xCC: the sell button, linked to the first page item
    u32 mUnkD0;
    std::vector<void *> mItems; // 0xD4: this page's items
    DinoUiShopSellPageLButton *mPageL; // 0xE0
    DinoUiShopSellPageRButton *mPageR; // 0xE4
    DinoUi3PartsFraction *mFraction; // 0xE8: "page n/m"
    MTaskSet *mCursorTasks;     // 0xEC
    DinoUi3PartsCursor *mCursor; // 0xF0
    DinoUi3PartsRockName *mRockName; // 0xF4
    std::vector<u32> mFossils;  // 0xF8: the fossils for sale
    DinoUi3PartsMoney *mMoney;  // 0x104
    u8 mFlags[0x40];            // 0x108
    int mPage;                  // 0x148
    int mLastPage;              // 0x14C
    u8 mCount;                  // 0x150: how many are chosen
    u8 mSelected;               // 0x151: 0xFF for none
    BOOL mConfirmed;            // 0x154: the yes/no answer
    MMenuItem *mFocused;        // 0x158
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
