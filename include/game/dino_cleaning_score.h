#ifndef GAME_DINO_CLEANING_SCORE_H
#define GAME_DINO_CLEANING_SCORE_H

#include "m/task.h"

class DinoCleaningScore : public MProcess {
public:
    virtual ~DinoCleaningScore();
    virtual void Idle();

    u8 GetUnk90();
    u8 GetUnk91();

    u8 unk74[0x1C];
    u8 mUnk90;      // 0x90
    u8 mUnk91;      // 0x91
};

#endif
