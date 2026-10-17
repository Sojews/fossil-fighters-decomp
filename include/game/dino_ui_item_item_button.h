#ifndef GAME_DINO_UI_ITEM_ITEM_BUTTON_H
#define GAME_DINO_UI_ITEM_ITEM_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoUiItemItemButton : public MMenuItem {
public:
    virtual ~DinoUiItemItemButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    u32 mUnk88;
    MPoint mPos;  // 0x8C: the center
    MView *mView; // 0x94
};

#endif
