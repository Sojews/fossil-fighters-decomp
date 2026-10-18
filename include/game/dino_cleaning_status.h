#ifndef GAME_DINO_CLEANING_STATUS_H
#define GAME_DINO_CLEANING_STATUS_H

#include "m/task.h"

// The element of an 8-byte-element vector the cleaning files share (the real type is unknown).
struct DinoCleaningStatusEntry {
    u32 unk0;
    u32 unk4;
};

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
    void StatePause();

    void (DinoCleaningStatus::*mState)(); // 0x74
    u32 unk7C[2];
    DinoCleaningTimer *mTimer;            // 0x84
    DinoCleaningScore *mScore;            // 0x88
};

#endif
