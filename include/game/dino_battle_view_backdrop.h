#ifndef GAME_DINO_BATTLE_VIEW_BACKDROP_H
#define GAME_DINO_BATTLE_VIEW_BACKDROP_H

#include "m/raster.h"
#include "game/dino_resource.h"

// The battle's backdrop: a color per line, from one of the prepared tables or fading between two.
class DinoBattleViewBackdrop : public MRasterPal {
public:
    // Fills each table with its backdrop's colors, from the top color down to the bottom one.
    DinoBattleViewBackdrop() : MRasterPal(-1) {
        mTable = 0;
        mFrame = 0;
        mFading = FALSE;
        for (int t = 0; t < 5; t++) {
            const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
            const u8 *top = (const u8 *) defs + defs->backdropTop + t * 4;
            const DinoBattleAdjustDefs *defs2 = gDinoResource->GetBattleAdjustDefs();
            int r = top[0];
            int g = top[1];
            int b = top[2];
            const u8 *bottom = (const u8 *) defs2 + defs2->backdropBottom + t * 4;
            int dr = bottom[0] - r;
            int dg = bottom[1] - g;
            int db = bottom[2] - b;
            u16 *line = mTables[t] = new u16[M_RASTER_LINES];
            int ar = 0;
            int ag = 0;
            int ab = 0;
            for (int y = 0; y < M_RASTER_LINES; y++) {
                *line++ = 0x8000 | (((b + ab / 191) >> 3) << 10) | (((g + ag / 191) >> 3) << 5) | ((r + ar / 191) >> 3);
                ar += dr;
                ag += dg;
                ab += db;
            }
        }
    }
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
