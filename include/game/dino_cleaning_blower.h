#ifndef GAME_DINO_CLEANING_BLOWER_H
#define GAME_DINO_CLEANING_BLOWER_H

#include "m/task.h"

class DinoCleaningKasekiRock;
class DinoBlower;

class DinoCleaningBlower : public MProcess {
public:
    DinoCleaningBlower(DinoCleaningKasekiRock *rock);
    virtual ~DinoCleaningBlower();
    virtual void Idle();

    DinoCleaningKasekiRock *mRock;  // 0x74
    DinoBlower *mBlower;            // 0x78
    u8 mPressed[25];                // 0x7C: whether L or R was pressed, over the last 25 frames
    u8 mIndex;                      // 0x95
};

#endif
