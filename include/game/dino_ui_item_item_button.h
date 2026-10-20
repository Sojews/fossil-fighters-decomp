#ifndef GAME_DINO_UI_ITEM_ITEM_BUTTON_H
#define GAME_DINO_UI_ITEM_ITEM_BUTTON_H

#include "m/view.h"
#include "m/motion.h"
#include "m/menu_item.h"

class DinoUiItemMenu;

class DinoUiItemItemButton : public MMenuItem {
public:
    DinoUiItemItemButton(DinoUiItemMenu *owner, MPoint pos);
    virtual ~DinoUiItemItemButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    DinoUiItemMenu *mOwner; // 0x88
    MPoint mPos;  // 0x8C: the center
    MMotion *mView; // 0x94
};

#endif
