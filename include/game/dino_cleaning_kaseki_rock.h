#ifndef GAME_DINO_CLEANING_KASEKI_ROCK_H
#define GAME_DINO_CLEANING_KASEKI_ROCK_H

#include "m/task.h"

class MInput;
class DinoCommonLive;

class DinoCleaningKasekiRock : public MProcess {
public:
    virtual ~DinoCleaningKasekiRock();

    void SetUnk128(BOOL value);
    MInput *GetUnkDC();
    u32 GetUnkE0();
    u32 GetUnkD4();
    u32 GetUnkD8();
    DinoCommonLive *GetUnkE4();

    u8 unk74[0x60];
    u32 mUnkD4;     // 0xD4
    u32 mUnkD8;     // 0xD8
    MInput *mUnkDC; // 0xDC
    u32 mUnkE0;     // 0xE0
    DinoCommonLive *mUnkE4; // 0xE4
    u8 unkE8[0x40];
    u8 mUnk128; // 0x128
};

#endif
