// MDirectBitmap.
// NONMATCHING: the static initializer (the game pools the size table after the type infos, at +0x58).

#include "m/bitmap.h"
#include "m/lcd.h"
#include "m/looper.h"
#include "m/transfer.h"

static MPoint sDirectBitmapSizes[4] = {
    MPoint(128, 128),
    MPoint(256, 256),
    MPoint(512, 256),
    MPoint(512, 512),
};

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
