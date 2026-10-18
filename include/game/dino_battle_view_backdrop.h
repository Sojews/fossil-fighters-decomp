#ifndef GAME_DINO_BATTLE_VIEW_BACKDROP_H
#define GAME_DINO_BATTLE_VIEW_BACKDROP_H

#include "m/raster.h"

// The battle's backdrop: a color per line, from one of the prepared tables or fading between two.
class DinoBattleViewBackdrop : public MRasterPal {
public:
    virtual ~DinoBattleViewBackdrop();
    virtual void Execute();
    virtual void OnVSync();

    u16 *mTables[5];                // 0x40
    u16 mFadeLines[M_RASTER_LINES]; // 0x54
    u32 mTable;                     // 0x1D4: shown when not fading
    u32 mFrom;                      // 0x1D8
    u32 mTo;                        // 0x1DC
    int mDuration;                  // 0x1E0
    int mFrame;                     // 0x1E4
    u8 mFading;                     // 0x1E8
};

#endif
