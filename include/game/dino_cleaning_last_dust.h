#ifndef GAME_DINO_CLEANING_LAST_DUST_H
#define GAME_DINO_CLEANING_LAST_DUST_H

#include "m/task.h"
#include "m/fx32.h"

class MMotion;
class MSmartOam;
class DinoCleaningImageView;

// The last bit of dust, flying from where it was to its place.
class DinoCleaningLastDust : public MProcess {
public:
    inline DinoCleaningLastDust(DinoCleaningImageView *view, MFx32Point from, MFx32Point to, MSmartOam *oam);
    virtual ~DinoCleaningLastDust();
    virtual void Idle();

    u32 unk74;
    MFx32Point mFrom;       // 0x78
    MFx32Point mTo;         // 0x80
    MFx32 mFrame;           // 0x88
    MFx32 mDuration;        // 0x8C
    MMotion *mMotion;       // 0x90
};

#endif
