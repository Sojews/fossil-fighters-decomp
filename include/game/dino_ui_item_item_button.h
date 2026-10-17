#ifndef GAME_DINO_UI_ITEM_ITEM_BUTTON_H
#define GAME_DINO_UI_ITEM_ITEM_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoUiItemItemButton : public MMenuItem {
public:
    virtual ~DinoUiItemItemButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    u8 unk88[0xC];
    MView *mView; // 0x94
};

#endif
