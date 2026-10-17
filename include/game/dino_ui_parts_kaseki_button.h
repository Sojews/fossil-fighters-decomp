#ifndef GAME_DINO_UI_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI_PARTS_KASEKI_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoUiPartsKasekiButton : public MMenuItem {
public:
    virtual ~DinoUiPartsKasekiButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    MPoint mPos;  // 0x88: the center
    u32 mUnk90;
    MView *mView; // 0x94
};

#endif
