#ifndef GAME_DINO_UI_PARTS_REVIVER_NAME_H
#define GAME_DINO_UI_PARTS_REVIVER_NAME_H

#include "m/motion.h"
#include "m/task.h"
#include "game/help.h"

class DinoUiPartsReviverName : public MProcess, public DinoHelpHook {
public:
    virtual ~DinoUiPartsReviverName();
    virtual void Unk0C();
    virtual void Unk08();
    virtual void Idle();

    u32 mUnk7C[2];      // 0x7C
    MView *mDone;       // 0x84: shown once every letter is out
    MMotion *mMotion;   // 0x88: the name, one more letter per anime
    u8 mLength;         // 0x8C
    u8 mLeft;           // 0x8D: letters still to show
};

#endif
