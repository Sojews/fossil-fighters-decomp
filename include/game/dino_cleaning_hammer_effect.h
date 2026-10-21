#ifndef GAME_DINO_CLEANING_HAMMER_EFFECT_H
#define GAME_DINO_CLEANING_HAMMER_EFFECT_H

#include "m/task.h"
#include "m/fx32.h"

class MRasterWindow;
class DinoCleaningKasekiRock;

class DinoCleaningHammerEffect : public MProcess {
public:
    DinoCleaningHammerEffect(DinoCleaningKasekiRock *rock, MPoint center, u8 big);
    virtual ~DinoCleaningHammerEffect();
    virtual void Idle();

    void StateStart();
    void State0215caf8();
    void StateEnd();
    void StateClear();

    void (DinoCleaningHammerEffect::*mState)(); // 0x74
    DinoCleaningKasekiRock *mRock;  // 0x7C
    MPoint mCenter;             // 0x80
    u8 mBig;                    // 0x88: the strong hit's ring
    int mFrame;                 // 0x8C
    int mFrames;                // 0x90
    MRasterWindow *mWindow;     // 0x94
    u32 unk98;
    MFx32 mSpeed;               // 0x9C: radius per frame
};

#endif
