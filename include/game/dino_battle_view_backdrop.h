#ifndef GAME_DINO_BATTLE_VIEW_BACKDROP_H
#define GAME_DINO_BATTLE_VIEW_BACKDROP_H

#include "m/raster.h"
#include "game/dino_resource.h"

// A backdrop table's color (four bytes each) at the given offset into the battle adjust defs.
static inline const u8 *DinoBackdropColor(const DinoBattleAdjustDefs *defs, u32 offset, u32 t) {
    return (const u8 *) defs + offset + t * 4;
}

// The battle's backdrop: a color per line, from one of the prepared tables or fading between two. (Its type info
// names MRasterIO as the base: it isn't an MRasterPal, though it keeps the display the same way.)
class DinoBattleViewBackdrop : public MRasterIO {
public:
    // Fills each table with its backdrop's colors, from the top color down to the bottom one.
    DinoBattleViewBackdrop() : MRasterIO(-1, gCurrentDisplay->GetBackdropReg(), 2) {
        mUnk3C = (u32) gCurrentDisplay;
        mTable = 0;
        mFrame = 0;
        mFading = FALSE;
        // Declared up front in the game's spill-slot order.
        int db, dg, dr;
        u32 b, g, r;
        u16 *dst;
        int t;
        u16 *line;
        int ab, ag, ar;
        for (t = 0; t < 5; t++) {
            const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
            const u8 *top = DinoBackdropColor(defs, defs->backdropTop, t);
            const DinoBattleAdjustDefs *defs2 = gDinoResource->GetBattleAdjustDefs();
            const u8 *bottom = DinoBackdropColor(defs2, defs2->backdropBottom, t);
            r = top[0];
            g = top[1];
            b = top[2];
            dr = bottom[0] - r;
            dg = bottom[1] - g;
            db = bottom[2] - b;
            mTables[t] = new u16[M_RASTER_LINES];
            line = mTables[t];
            ab = ar = ag = 0;
            for (int y = 0; y < M_RASTER_LINES; y++) {
                dst = line;
                line++;
                *dst = (((b + ab / 191) >> 3) << 10) | ((((g + ag / 191) >> 3) << 5) | (0x8000 | ((r + ar / 191) >> 3)));
                ar += dr;
                ag += dg;
                ab += db;
            }
        }
    }
    virtual ~DinoBattleViewBackdrop();
    virtual void Execute();
    virtual void OnVSync();

    u32 mUnk3C;                     // 0x3C: the display
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
