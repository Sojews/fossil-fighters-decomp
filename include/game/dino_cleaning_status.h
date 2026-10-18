#ifndef GAME_DINO_CLEANING_STATUS_H
#define GAME_DINO_CLEANING_STATUS_H

#include "m/task.h"

class DinoCleaningTimer;
class DinoCleaningScore;

class DinoCleaningStatus : public MProcess {
public:
    virtual ~DinoCleaningStatus();
    virtual void Idle();

    DinoCleaningTimer *GetTimer();
    DinoCleaningScore *GetScore();
    void Start();
    void Stop();

    void (DinoCleaningStatus::*mState)(); // 0x74
    u32 unk7C[2];
    DinoCleaningTimer *mTimer;            // 0x84
    DinoCleaningScore *mScore;            // 0x88
};

#endif
