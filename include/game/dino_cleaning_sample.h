#ifndef GAME_DINO_CLEANING_SAMPLE_H
#define GAME_DINO_CLEANING_SAMPLE_H

#include "m/task.h"

class DinoCleaningSample : public MProcess {
public:
    DinoCleaningSample();
    virtual ~DinoCleaningSample();
    virtual void Idle();

    void StateMenu();
    void StateCleaning();

    void (DinoCleaningSample::*mState)(); // 0x74
    u32 mUnk7C;             // 0x7C
    u32 mUnk80;             // 0x80
    u32 mUnk84;             // 0x84
};

#endif
