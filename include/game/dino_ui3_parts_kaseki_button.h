#ifndef GAME_DINO_UI3_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI3_PARTS_KASEKI_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"

class DinoUi3PartsKasekiButton : public MMenuItem {
public:
    virtual ~DinoUi3PartsKasekiButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    u8 unk88[0xC];
    MView *mView; // 0x94
};

#endif
