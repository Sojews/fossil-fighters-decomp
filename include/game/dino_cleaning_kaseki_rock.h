#ifndef GAME_DINO_CLEANING_KASEKI_ROCK_H
#define GAME_DINO_CLEANING_KASEKI_ROCK_H

#include "m/task.h"

class DinoCleaningKasekiRock : public MProcess {
public:
    virtual ~DinoCleaningKasekiRock();

    void SetUnk128(BOOL value);

    u8 unk74[0xB4];
    u8 mUnk128; // 0x128
};

#endif
