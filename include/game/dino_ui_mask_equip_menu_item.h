#ifndef GAME_DINO_UI_MASK_EQUIP_MENU_ITEM_H
#define GAME_DINO_UI_MASK_EQUIP_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiMaskEquipMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMaskEquipMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

#endif
