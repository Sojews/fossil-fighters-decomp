// DinoFieldRaster.

#include "game/dino_field_raster.h"
#include "m/raster.h"

DinoFieldRaster::DinoFieldRaster(u32 arg) : MProcess(NULL) {
    mUnk78 = arg;
    mFrame = 0;
    mIO = new MRasterIO((u32)-1, (volatile void *)0x04000010, 4);
    mParallel.AddTail(mIO);
}

DinoFieldRaster::~DinoFieldRaster() {
}
