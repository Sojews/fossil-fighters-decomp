// MChr16Fill and MChr256Transfer.

#include "m/canvas.h"
#include "m/chr.h"
#include "m/transfer.h"

void MChr256Transfer::Transfer() {
    if (mPalette != 0) {
        MAddPaletteOffset((const u16 *)mSrc, (u16 *)mDest, mSize, mPalette);
    } else {
        MCopy(mSrc, mDest, mSize);
    }
}

MChr16Fill::~MChr16Fill() {
}

// Fills the rect (clipped to the tiles) with the color, a row of 4-bit pixels at a time.
// NONMATCHING: register allocation (the game spills tilesX, tx and w and keeps rightMask and first in registers).
void MChr16Fill::Transfer() {
    if (mPattern == (u32) -1 || mSize.x <= 0 || mSize.y <= 0) {
        return;
    }
    u32 fill = 0;
    for (u32 i = 0, shift = 0; i < 8; i++, shift += 4) {
        fill |= mPattern << shift;
    }
    if (mOrigin.y < 0) {
        mSize.y += mOrigin.y;
        mOrigin.y = 0;
    }
    int over = mOrigin.y + mSize.y - mTiles.y * 8;
    if (over > 0) {
        mSize.y -= over;
    }
    int rows = mSize.y;
    int tilesX = mTiles.x;
    int x = mOrigin.x;
    int y = mOrigin.y;
    int tx = x >> 3;
    u32 *p = (u32 *) (mBase + (tx + (y >> 3) * tilesX) * 32 + (y & 7) * 4);
    int left = x & 7;
    u32 leftMask = ~((1 << (left * 4)) - 1);
    int w = mSize.x;
    int end = left + w;
    int right = end & 7;
    u32 rightMask = (1 << (right * 4)) - 1;
    if (right == 0) {
        rightMask = -1;
    }
    int cols = (end + 7) >> 3;
    int nextRow = -cols * 8 + 1;
    int nextTileRow = nextRow + (tilesX - 1) * 8;
    u32 first;
    if (x < 0) {
        first = -tx;
    } else {
        first = 0;
    }
    u32 last;
    int overX = x + w - tilesX * 8;
    if (overX > 0) {
        last = cols - ((overX + 7) >> 3);
    } else {
        last = cols;
    }
    if (rows != 0) {
        u32 mask = leftMask & rightMask;
        do {
            for (u32 c = 0; c < (u32) cols; c++) {
                if (c >= first && c < last) {
                    if (c == 0 && c == cols - 1) {
                        *p = (*p & ~mask) | (fill & mask);
                    } else if (c == 0) {
                        *p = (*p & ~leftMask) | (leftMask & fill);
                    } else if (c == cols - 1) {
                        *p = (*p & ~rightMask) | (fill & rightMask);
                    } else {
                        *p = fill;
                    }
                }
                p += 8;
            }
            if ((((u32) p - mBase) & 0x1F) != 0x1C) {
                p += nextRow;
            } else {
                p += nextTileRow;
            }
        } while (--rows);
    }
}
