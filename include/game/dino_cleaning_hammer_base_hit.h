#ifndef GAME_DINO_CLEANING_HAMMER_BASE_HIT_H
#define GAME_DINO_CLEANING_HAMMER_BASE_HIT_H

#include "m/fx32.h"
#include "m/seed_generator.h"
#include "m/task.h"

class DinoCleaningKasekiRock;

class DinoCleaningHammerBaseHit : public MProcess {
public:
    DinoCleaningHammerBaseHit(DinoCleaningKasekiRock *rock, MRandom *random, u8 big);
    virtual ~DinoCleaningHammerBaseHit();
    virtual void Idle();

    void StateStart();
    void State0215d0c4();

    void (DinoCleaningHammerBaseHit::*mState)(); // 0x74
    DinoCleaningKasekiRock *mRock;  // 0x7C
    MRandom *mUnk80;                // 0x80
    u8 mBig;                        // 0x84: the strong hit
    MFx32 mRadius;                  // 0x88
};

#endif
