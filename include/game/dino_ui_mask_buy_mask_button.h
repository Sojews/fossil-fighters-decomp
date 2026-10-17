#ifndef GAME_DINO_UI_MASK_BUY_MASK_BUTTON_H
#define GAME_DINO_UI_MASK_BUY_MASK_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoUiMaskBuyMaskButton : public MMenuItem {
public:
    virtual ~DinoUiMaskBuyMaskButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    u8 unk88[0xC];
    MView *mView; // 0x94
};

#endif
