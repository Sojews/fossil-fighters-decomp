#ifndef GAME_DINO_CLEANING_HAMMER_EFFECT_H
#define GAME_DINO_CLEANING_HAMMER_EFFECT_H

#include "m/task.h"

class MRasterWindow;

class DinoCleaningHammerEffect : public MProcess {
public:
    virtual ~DinoCleaningHammerEffect();
    virtual void Idle();

    void StateEnd();

    void (DinoCleaningHammerEffect::*mState)(); // 0x74
    u8 unk7C[0x94 - 0x7C];
    MRasterWindow *mWindow;     // 0x94
};

#endif
