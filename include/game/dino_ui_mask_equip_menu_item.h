#ifndef GAME_DINO_UI_MASK_EQUIP_MENU_ITEM_H
#define GAME_DINO_UI_MASK_EQUIP_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiMaskEquipMenu;

class DinoUiMaskEquipMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiMaskEquipMenuItem(DinoUiMaskEquipMenu *owner, const std::string &path, MPoint pos, u32 textId);
    virtual ~DinoUiMaskEquipMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiMaskEquipMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

#endif
