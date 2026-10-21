#ifndef GAME_DINO_CLEANING_NEW_DUST_H
#define GAME_DINO_CLEANING_NEW_DUST_H

#include "m/task.h"
#include "m/fx32.h"

class DinoCleaningImageView;
class MMotion;
class MSmartOam;

// A bit of dust brushed off, flying from where it was in an arc and landing as dirt.
class DinoCleaningNewDust : public MProcess {
public:
    inline DinoCleaningNewDust(DinoCleaningImageView *view, MFx32Point from, MFx32Point to, MFx32 height,
                               MSmartOam *oam);
    virtual ~DinoCleaningNewDust();
    virtual void Idle();

    DinoCleaningImageView *mView;   // 0x74
    MFx32Point mFrom;               // 0x78
    MFx32Point mTo;                 // 0x80
    MFx32 mHeight;                  // 0x88
    MFx32 mFrame;                   // 0x8C
    MFx32 mDuration;                // 0x90
    MMotion *mMotion;               // 0x94
};

#endif
