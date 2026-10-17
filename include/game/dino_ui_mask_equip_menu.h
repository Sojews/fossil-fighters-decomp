#ifndef GAME_DINO_UI_MASK_EQUIP_MENU_H
#define GAME_DINO_UI_MASK_EQUIP_MENU_H

#include "game/dino_ui3_parts_fraction.h"
#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_mask_equip_menu_item.h"

class DinoUiMaskEquipMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiMaskEquipMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void Decide(int index);
    void Equip();
    void ClearItems();
    void RefreshPage2();

    u32 mUnkB4;
    MMenuItem *mLinkFrom;   // 0xB8: linked to the first page item
    u32 mUnkBC;
    std::vector<void *> mItems; // 0xC0: this page's items
    u32 mUnkCC[6];          // 0xCC
    DinoUi3PartsFraction *mFraction; // 0xE4: "page n/m"
    int mChoice;            // 0xE8
    u32 mUnkEC;
    std::vector<void *> mUnkF0; // 0xF0
    int mPage;              // 0xFC
    int mLastPage;          // 0x100
    u8 mUnk104;             // 0x104
};

class DinoUiMaskEquipBackButton : public DinoUiMaskEquipMenuItem {
public:
    virtual ~DinoUiMaskEquipBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMaskEquipPageLButton : public DinoUiMaskEquipMenuItem {
public:
    virtual ~DinoUiMaskEquipPageLButton();
    virtual void OnDecide();
};

class DinoUiMaskEquipPageRButton : public DinoUiMaskEquipMenuItem {
public:
    virtual ~DinoUiMaskEquipPageRButton();
    virtual void OnDecide();
};

#endif
