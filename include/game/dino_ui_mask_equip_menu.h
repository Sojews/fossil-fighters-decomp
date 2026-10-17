#ifndef GAME_DINO_UI_MASK_EQUIP_MENU_H
#define GAME_DINO_UI_MASK_EQUIP_MENU_H

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

    u32 mUnkB4[3];          // 0xB4
    std::vector<void *> mUnkC0; // 0xC0
    u32 mUnkCC[9];          // 0xCC
    std::vector<void *> mUnkF0; // 0xF0
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
