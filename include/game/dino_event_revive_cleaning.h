#ifndef GAME_DINO_EVENT_REVIVE_CLEANING_H
#define GAME_DINO_EVENT_REVIVE_CLEANING_H

#include "m/task.h"

// How the cleaning went.
struct DinoEventReviveResult {
    u32 unk00[4];
    s32 mOutcome;   // 0x10: 1 or 2 revives, 3 gives up, 4 retries
};

class DinoEventReviveCleaning : public MProcess {
public:
    DinoEventReviveCleaning(s32 rock, DinoEventReviveResult *result)
        : MProcess(NULL), mRock(rock), mResult(result), mStep(0) {}
    virtual ~DinoEventReviveCleaning();
    virtual void Idle();

    s32 mRock;                      // 0x74
    DinoEventReviveResult *mResult; // 0x78
    u32 mStep;                      // 0x7C
};

#endif
