#ifndef GAME_DINO_UI_MASK_BUY_MASK_BUTTON_H
#define GAME_DINO_UI_MASK_BUY_MASK_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoUiMaskBuyMaskButton : public MMenuItem {
public:
    virtual ~DinoUiMaskBuyMaskButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    MPoint mPos;  // 0x88: the center
    MMotion *mMotion; // 0x90
    MView *mView; // 0x94: the mask's details
    MView *mMark; // 0x98: shown while the mask is chosen
};

#endif
