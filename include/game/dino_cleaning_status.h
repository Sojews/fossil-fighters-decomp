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

class MSmartOam;

class DinoCleaning;
class MTextPicture;

class DinoCleaningStatus : public MProcess {
public:
    DinoCleaningStatus(DinoCleaning *cleaning, u32 frames, u32 unk, u8 flag);
    virtual ~DinoCleaningStatus();
    virtual void Idle();

    DinoCleaningTimer *GetTimer();
    DinoCleaningScore *GetScore();
    void Start();
    void Stop();
    void StateIdle();
    void StatePause();
    // Blinks while paused.
    void StatePaused();

    void (DinoCleaningStatus::*mState)(); // 0x74
    MTextPicture *mFrame;                 // 0x7C
    MTextPicture *mTimerFrame;            // 0x80
    DinoCleaningTimer *mTimer;            // 0x84
    DinoCleaningScore *mScore;            // 0x88
    MSmartOam *mOam;                      // 0x8C
};

#endif
