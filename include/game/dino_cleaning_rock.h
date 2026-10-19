#ifndef GAME_DINO_CLEANING_ROCK_H
#define GAME_DINO_CLEANING_ROCK_H

#include "m/task.h"
#include "m/fx32.h"

class DinoCleaningImage;

class DinoCleaningRock : public MProcess {
public:
    DinoCleaningRock(u32 arg0, DinoCleaningImage *image, u32 arg2, fx32 scale);
    virtual ~DinoCleaningRock();
    virtual void Idle();

    DinoCleaningImage *GetImage();
    MFx32 GetRatio();
    BOOL IsCellClear(int row, int col);
    s32 GetCellLayer(int row, int col);
    u8 GetCell(int row, int col);
    // Works out each cell's hardness: its layer's, scaled.
    void RefreshHardness();
    // Hits the cell with the power, breaking through layers; returns what's left of the power.
    MFx32 Hit(int row, int col, MFx32 power);

    u32 mUnk74;                 // 0x74
    DinoCleaningImage *mImage;  // 0x78
    u32 mUnk7C;                 // 0x7C
    MFx32 mScale;               // 0x80
    MFx32 mHardness[36][28];    // 0x84
};

#endif
