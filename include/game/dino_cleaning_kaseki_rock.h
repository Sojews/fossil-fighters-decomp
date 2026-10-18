#ifndef GAME_DINO_CLEANING_KASEKI_ROCK_H
#define GAME_DINO_CLEANING_KASEKI_ROCK_H

#include "m/task.h"
#include "m/fx32.h"
#include "m/view.h"

class DinoCleaningRock;
class DinoCleaningImageView;
class MSmartOam;

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
    void StartXray();
    void CountCells();
    void SetOffset(MPoint offset);
    void SetOffsetAll(MPoint offset);
    void StartDrop();
    void StateDrop();
    void State021601e0();

    void (DinoCleaningKasekiRock::*mState)();   // 0x74
    u32 unk7C;
    MView *mUnk80;                              // 0x80
    DinoCleaningImageView *mView;               // 0x84
    MView *mUnk88;                              // 0x88
    DinoCleaningRock *mRock8C;                  // 0x8C
    DinoCleaningRock *mRock90;                  // 0x90
    MTask *mUnk94;                              // 0x94
    u32 mUnk98;                                 // 0x98
    u32 mUnk9C;                                 // 0x9C
    u32 mUnkA0[(0x4A4 - 0xA0) / 4];             // 0xA0
    MSmartOam *mOam;                            // 0x4A4
    u32 unk4A8;
    MFx32 mDropY;                               // 0x4AC: the view's height while it drops in
    MFx32 mDropSpeed;                           // 0x4B0
    u32 mUnk4B4;                                // 0x4B4
    u8 mUnk4B8;                                 // 0x4B8
};

#endif
