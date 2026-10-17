#ifndef GAME_DINO_UI_MASK_EQUIP_MASK_BUTTON_H
#define GAME_DINO_UI_MASK_EQUIP_MASK_BUTTON_H

#include "m/menu_item.h"
#include "m/motion.h"

class DinoUiMaskEquipMaskButton : public MMenuItem {
public:
    virtual ~DinoUiMaskEquipMaskButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u32 mUnk88;
    MPoint mPos;        // 0x8C: the center
    u32 mUnk94;
    MMotion *mMotion;   // 0x98
};

#endif
