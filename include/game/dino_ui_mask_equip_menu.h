#ifndef GAME_DINO_UI_MASK_EQUIP_MENU_H
#define GAME_DINO_UI_MASK_EQUIP_MENU_H

#include "game/dino_ui3_parts_fraction.h"
#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_mask_equip_menu_item.h"

class MSmartOam;
class DinoUi3PartsCursor;
class MIndicator;
class MTextPicture;

class DinoUiMaskEquipMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiMaskEquipMenu();
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
    // Puts the mask on if the player said yes.
    void FinishEquip();
    void ClearItems();
    void RefreshPage2();

    MSmartOam *mOam;        // 0xB4: the items' sprites
    MMenuItem *mLinkFrom;   // 0xB8: linked to the first page item
    MTextPicture *mBackground;  // 0xBC
    std::vector<void *> mItems; // 0xC0: this page's items
    MMenuItem *mPageL;      // 0xCC
    MMenuItem *mPageR;      // 0xD0
    u32 mUnkD4;
    MTaskSet *mCursorTasks; // 0xD8
    DinoUi3PartsCursor *mCursor; // 0xDC
    MIndicator *mName;      // 0xE0: the focused mask's name
    DinoUi3PartsFraction *mFraction; // 0xE4: "page n/m"
    int mChoice;            // 0xE8
    u32 mAnswer;            // 0xEC: the yes/no window's
    std::vector<void *> mUnkF0; // 0xF0: the masks (ids)
    int mPage;              // 0xFC
    int mLastPage;          // 0x100
    u8 mUnk104;             // 0x104
};

class DinoUiMaskEquipBackButton : public DinoUiMaskEquipMenuItem {
public:
    DinoUiMaskEquipBackButton(DinoUiMaskEquipMenu *owner);
    virtual ~DinoUiMaskEquipBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMaskEquipPageLButton : public DinoUiMaskEquipMenuItem {
public:
    DinoUiMaskEquipPageLButton(DinoUiMaskEquipMenu *owner);
    virtual ~DinoUiMaskEquipPageLButton();
    virtual void OnDecide();
};

class DinoUiMaskEquipPageRButton : public DinoUiMaskEquipMenuItem {
public:
    DinoUiMaskEquipPageRButton(DinoUiMaskEquipMenu *owner);
    virtual ~DinoUiMaskEquipPageRButton();
    virtual void OnDecide();
};

#endif
