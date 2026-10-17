#ifndef GAME_DINO_CLEANING_BLOWER_H
#define GAME_DINO_CLEANING_BLOWER_H

#include "m/task.h"

class DinoCleaning;
class DinoBlower;

class DinoCleaningBlower : public MProcess {
public:
    DinoCleaningBlower(DinoCleaning *cleaning);
    virtual ~DinoCleaningBlower();
    virtual void Idle();

    DinoCleaning *mCleaning;  // 0x74
    DinoBlower *mBlower;            // 0x78
    u8 mPressed[25];                // 0x7C: whether L or R was pressed, over the last 25 frames
    u8 mIndex;                      // 0x95
};

#endif
