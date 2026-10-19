// DinoFieldRaster.

#include "game/dino_field_raster.h"
#include "m/raster.h"
#include "m/fx32.h"

// How far the field's lines sway (set elsewhere).
extern "C" MFx32 data_020b5ac4;

// The sine of the phase's place in the period, doubled.
static inline MFx32 Wave(u32 phase, u32 period) {
    return MFx32::FromRaw(FX_SinIdx((u16)((phase << 16) / period))) * MFx32(2);
}

DinoFieldRaster::DinoFieldRaster(u32 arg) : MProcess(NULL) {
    mUnk78 = arg;
    mFrame = 0;
    mIO = new MRasterIO((u32)-1, (volatile void *)0x04000010, 4);
    mParallel.AddTail(mIO);
}

DinoFieldRaster::~DinoFieldRaster() {
}

// Sways each line by three waves.
// NONMATCHING: the temporaries' stack slots, and when the line pointer and the sway amount are loaded.
void DinoFieldRaster::Idle() {
    u32 t = ++mFrame / 2;
    u16 *line = (u16 *)mIO->mBack;
    for (u32 i = 0; i < M_RASTER_LINES; i++) {
        MFx32 sway = Wave(t + i, 50) + Wave(t * 2 + i, 100) + Wave(t * 3 + i, 200);
        line[0] = MFx32ToInt(MFx32::FromRaw(sway.v & 0xFFFFF000) * data_020b5ac4);
        line[1] = 0;
        line += 2;
    }
    mIO->Flip();
}
