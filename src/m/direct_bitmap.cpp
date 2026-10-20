// MDirectBitmap.

#include "m/bitmap.h"
#include "game/dino_static_init.h"
#include "m/lcd.h"
#include "m/looper.h"
#include "m/transfer.h"

// The sizes: (128, 128), (256, 256), (512, 256), (512, 512), set by the file's static initialization.
extern "C" MPoint sDirectBitmapSizes[4];

u32 MDirectBitmap::GetVramSize(int size) {
    return sDirectBitmapSizes[size].x * 2 * sDirectBitmapSizes[size].y;
}

MDirectBitmap::MDirectBitmap(MDisplay *display, u32 screenBase, u32 base, int size, int bg)
    : MAffineScreen(display, screenBase, base, sDirectBitmapSizes[size].x * 2 * sDirectBitmapSizes[size].y, size,
                    bg) {
    mSize = size;
    mBitmapSize.x = sDirectBitmapSizes[size].x;
    mBitmapSize.y = sDirectBitmapSizes[size].y;
    mBmpCntBits.directColor = TRUE;
    mBmpCntBits.bitmap = TRUE;
}

// NONMATCHING: the bank compare's operands are swapped.
// Captures the screen into this bitmap, which must be 256x256 and in one of VRAM banks A-D.
void MDirectBitmap::Capture(int srcA) {
    u32 bank;

    if (mSize != 1) {
        OS_Terminate();
    }
    for (bank = 1; bank <= 4; bank++) {
        u32 base = mBase;
        if (base == gLcd->GetBankAddress(bank)) {
            goto found;
        }
    }
    OS_Terminate();
found:
    gLcd->StartCapture(0, srcA, 0, 3, 0, bank, 0, 16, 0);
}

// The file's static initialization (see dino_static_init.h): element 0 through the pool, the others directly.
extern "C" u32 data_020c254c[];
static struct DirectBitmapStaticInit {
    DirectBitmapStaticInit() {
        DINO_POOL_WORD(data_020c254c, 0x58) = 128;
        DINO_POOL_WORD(data_020c254c, 0x5c) = 128;
        volatile MPoint *p = &sDirectBitmapSizes[1];
        p->x = 256;
        p->y = 256;
        p = &sDirectBitmapSizes[2];
        p->x = 512;
        p->y = 256;
        p = &sDirectBitmapSizes[3];
        p->x = 512;
        p->y = 512;
    }
} sDirectBitmapStaticInit;
