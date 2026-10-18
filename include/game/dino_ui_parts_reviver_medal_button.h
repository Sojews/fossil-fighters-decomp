#ifndef GAME_DINO_UI_PARTS_REVIVER_MEDAL_BUTTON_H
#define GAME_DINO_UI_PARTS_REVIVER_MEDAL_BUTTON_H

#include "m/motion.h"
#include "m/menu_item.h"

// A revived creature's medal.
class DinoUiPartsReviverMedalButton : public MMenuItem {
public:
    DinoUiPartsReviverMedalButton(MSmartOam *oam, MPoint pos, u32 creature, u8 lit, u8 byKind);
    virtual ~DinoUiPartsReviverMedalButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    void PlayNormal();
    void PlayFocused();
    void PlayDecided();
    // Flies the medal to `to` and lands it there.
    MTask *CreateFlyInTask(MPoint to);

    MMotion *mMotion;   // 0x88
    u8 mKind;           // 0x8C: the medal's first anime is 1 + mKind
    u8 mEmpty;          // 0x8D: no creature: hidden and silent
    MRect mFrame;       // 0x90
};

#endif
