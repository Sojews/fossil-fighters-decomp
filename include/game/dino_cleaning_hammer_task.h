#ifndef GAME_DINO_CLEANING_HAMMER_TASK_H
#define GAME_DINO_CLEANING_HAMMER_TASK_H

#include "m/fx32.h"
#include "m/task.h"

class DinoCleaningKasekiRock;
class DinoCleaningRock;
class MRandom;

class DinoCleaningHammerTask : public MProcess {
public:
    virtual ~DinoCleaningHammerTask();
    virtual void Idle();

    void StateStart();
    void State0215ccfc();

    void (DinoCleaningHammerTask::*mState)(); // 0x74
    DinoCleaningKasekiRock *mKasekiRock;    // 0x7C
    DinoCleaningRock *mRock;                // 0x80: the hit layer
    MRandom *mRandom;                       // 0x84
    int mRow;                               // 0x88: the hit cell
    int mCol;                               // 0x8C
    // Copies of the rock's cells, [row][col], and how the hit has reached them.
    u32 mLayers[0x24][0x1C];                // 0x90
    int mCells[0x24][0x1C];                 // 0x1050
    int mHit[0x24][0x1C];                   // 0x2010: 0 nothing to break, 1 not reached yet, 2 the hit
    int mPrevHit[0x24][0x1C];               // 0x2FD0: mHit as the frame started
    u8 mBig;                                // 0x3F90: the strong hit
    MFx32 mRadius;                          // 0x3F94
    MFx32 mRadius2;                         // 0x3F98
    MFx32 mDecay;                           // 0x3F9C
};

#endif
