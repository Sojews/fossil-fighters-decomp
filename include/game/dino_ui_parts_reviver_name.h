#ifndef GAME_DINO_UI_PARTS_REVIVER_NAME_H
#define GAME_DINO_UI_PARTS_REVIVER_NAME_H

#include "m/indicator.h"
#include "m/motion.h"
#include "m/smart_oam.h"
#include "m/task.h"
#include "game/help.h"

// A revived creature's name: a plate that grows a letter at a time, then the name on it.
class DinoUiPartsReviverName : public MProcess, public DinoHelpHook {
public:
    DinoUiPartsReviverName();
    virtual ~DinoUiPartsReviverName();
    virtual void Unk0C();
    virtual void Unk08();
    virtual void Idle();

    MTask *CreateFlyInTask(MPoint to);
    void SetCreature(u32 id);
    void SetPosition(MPoint pos);
    void Hide();

    MFont *mFont;       // 0x7C
    MSmartOam *mOam;    // 0x80
    MIndicator *mText;  // 0x84: shown once every letter is out
    MMotion *mMotion;   // 0x88: the plate, one more letter per anime
    u8 mLength;         // 0x8C: in letters (8 pixels)
    u8 mLeft;           // 0x8D: letters still to show
    u8 mMotionHidden;   // 0x8E: hidden for the help
    u8 mTextHidden;     // 0x8F: hidden for the help
    u8 mTextWidth;      // 0x90
};

#endif
