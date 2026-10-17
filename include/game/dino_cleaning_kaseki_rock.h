#ifndef GAME_DINO_CLEANING_KASEKI_ROCK_H
#define GAME_DINO_CLEANING_KASEKI_ROCK_H

#include "m/task.h"
#include "m/fx32.h"
#include "m/view.h"

class DinoCleaningRock;
class DinoCleaningImageView;

// The fossil rock being cleaned: an outer rock layer over the fossil.
class DinoCleaningKasekiRock : public MProcess {
public:
    virtual ~DinoCleaningKasekiRock();
    virtual void Idle();

    DinoCleaningRock *GetRock8C();
    DinoCleaningRock *GetRock90();
    u8 GetUnk4B8();
    BOOL IsCellClear(int row, int col);
    s32 GetCellLayer(int row, int col);
    DinoCleaningRock *GetTopRock(int row, int col);
    MFx32 GetRatio();
    MFx32 GetRatio(int index);
    BOOL IsViewUnkD9E0Zero();
    void Finish();
    void CountCells();

    void (DinoCleaningKasekiRock::*mState)();   // 0x74
    u32 unk7C;
    MView *mUnk80;                              // 0x80
    DinoCleaningImageView *mView;               // 0x84
    MTask *mUnk88;                              // 0x88
    DinoCleaningRock *mRock8C;                  // 0x8C
    DinoCleaningRock *mRock90;                  // 0x90
    MTask *mUnk94;                              // 0x94
    u32 mUnk98;                                 // 0x98
    u32 mUnk9C;                                 // 0x9C
    u32 mUnkA0[(0x4AC - 0xA0) / 4];             // 0xA0
    fx32 mUnk4AC;                               // 0x4AC
    fx32 mUnk4B0;                               // 0x4B0
    u32 unk4B4;
    u8 mUnk4B8;                                 // 0x4B8
};

#endif
