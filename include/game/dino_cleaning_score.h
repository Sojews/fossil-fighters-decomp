#ifndef GAME_DINO_CLEANING_SCORE_H
#define GAME_DINO_CLEANING_SCORE_H

#include "m/task.h"
#include "m/fx32.h"

class DinoCleaningScore : public MProcess {
public:
    void Show();
    void SetUnk9CC();
    virtual ~DinoCleaningScore();
    virtual void Idle();

    u8 GetUnk90();
    u8 GetUnk91();
    MFx32 GetUnk80();
    MFx32 GetUnk84();

    u8 unk74[0x80 - 0x74];
    MFx32 mUnk80;   // 0x80
    MFx32 mUnk84;   // 0x84
    u8 unk88[0x90 - 0x88];
    u8 mUnk90;      // 0x90
    u8 mUnk91;      // 0x91
    u8 unk92[0x93A];
    u8 mUnk9CC;     // 0x9CC
};

#endif
