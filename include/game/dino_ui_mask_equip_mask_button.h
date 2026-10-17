#ifndef GAME_DINO_UI_MASK_EQUIP_MASK_BUTTON_H
#define GAME_DINO_UI_MASK_EQUIP_MASK_BUTTON_H

#include "m/menu_item.h"

class DinoUiMaskEquipMaskButton : public MMenuItem {
public:
    virtual ~DinoUiMaskEquipMaskButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
