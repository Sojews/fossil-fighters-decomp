#ifndef GAME_DINO_CLEANING_KASEKI_ROCK_H
#define GAME_DINO_CLEANING_KASEKI_ROCK_H

#include "m/task.h"
#include "m/fx32.h"
#include "m/seed_generator.h"
#include "m/view.h"

class DinoCleaningRock;
class DinoCleaningImageView;
class MSmartOam;

// The fossil rock being cleaned: an outer rock layer over the fossil.
class DinoCleaningKasekiRockXrayBlendManager;

class DinoCleaning;

class DinoCleaningKasekiRock : public MProcess {
public:
    DinoCleaningKasekiRock(DinoCleaning *cleaning, u32 a, u32 b, u32 c, u32 d, u8 e, u32 f, u32 g, u32 h, u8 i, BOOL j,
                           u32 k, u32 l);
    virtual ~DinoCleaningKasekiRock();
    virtual void Idle();

    DinoCleaningRock *GetRock8C();
    DinoCleaningRock *GetRock90();
    u8 GetUnk4B8();
    BOOL IsCellClear(int row, int col);
    s32 GetCellLayer(int row, int col);
    DinoCleaningRock *GetTopRock(int row, int col);
    MFx32 GetDirtRatio();
    MFx32 GetRatio();
    MFx32 GetRatio(int index);
    BOOL IsViewUnkD9E0Zero();
    void Finish();
    void StartXray();
    void StopXray();
    void CountCells();
    void SetOffset(MPoint offset);
    void SetOffsetAll(MPoint offset);
    void Hit(int row, int col, MFx32 power, MFx32 scale, MFx32 decay);
    void StartDrop();
    void StateDrop();
    void StateShake();

    void (DinoCleaningKasekiRock::*mState)();   // 0x74
    u32 unk7C;
    MView *mUnk80;                              // 0x80
    DinoCleaningImageView *mView;               // 0x84
    MView *mUnk88;                              // 0x88
    DinoCleaningRock *mRock8C;                  // 0x8C
    DinoCleaningRock *mRock90;                  // 0x90
    DinoCleaningKasekiRockXrayBlendManager *mXray;  // 0x94
    u32 mUnk98;                                 // 0x98
    u32 mUnk9C;                                 // 0x9C
    u32 mUnkA0[4];                              // 0xA0: cells per layer
    u8 unkB0[0x4A0 - 0xB0];
    u32 mUnk4A0;                                // 0x4A0: 1 when both images show the rock's layers
    MSmartOam *mOam;                            // 0x4A4
    MRandom *mRandom;                           // 0x4A8
    MFx32 mDropY;                               // 0x4AC: the view's height while it drops in
    MFx32 mDropSpeed;                           // 0x4B0
    u32 mUnk4B4;                                // 0x4B4
    u8 mUnk4B8;                                 // 0x4B8
};

#endif
