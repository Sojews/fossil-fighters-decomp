#ifndef GAME_DINO_UI_PARTS_REVIVER_MEDAL_BUTTON_H
#define GAME_DINO_UI_PARTS_REVIVER_MEDAL_BUTTON_H

#include "m/motion.h"
#include "m/menu_item.h"

class DinoUiPartsReviverMedalButton : public MMenuItem {
public:
    virtual ~DinoUiPartsReviverMedalButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    MMotion *mMotion;   // 0x88
    u8 mKind;           // 0x8C: the medal's first anime is 1 + mKind
    MRect mFrame;       // 0x90
};

#endif
