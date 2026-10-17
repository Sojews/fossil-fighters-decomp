// MExtAffine and its transfers. MExtAffineTransfer::Transfer and MExtAffineFill::Transfer (tile map copies with
// flips) aren't decompiled yet.

#include "m/ext_affine.h"

static MPoint sSizes[4] = {
    MPoint(128, 128),
    MPoint(256, 256),
    MPoint(512, 512),
    MPoint(1024, 1024),
};

u32 MExtAffine::GetVramSize(int size) {
    return sSizes[size].x * sSizes[size].y / 64 * 2;
}

u16 *MExtAffine::GetEntry(u16 *base, int size, int x, int y) {
    return base + (sSizes[size].x >> 3) * y + x;
}

MPoint MExtAffine::GetDimensions(int size) {
    return sSizes[size];
}

MExtAffine::MExtAffine(MDisplay *display, u32 screenBase, u32 base, int size, int bg)
    : MAffineScreen(display, screenBase, base, GetVramSize(size), size, bg) {
    mSize = size;
    mPal = NULL;
    mBmpCntBits.bitmap = FALSE;
}

int MExtAffine::GetSize() {
    return mSize;
}

void MExtAffine::SetPalette(MPal *pal) {
    mPal = pal;
}

MPal *MExtAffine::GetPalette() {
    return mPal;
}

// NONMATCHING: the stack temporaries are laid out in a different order.
void MExtAffine::Transfer(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip,
                          bool vflip) {
    gLooper->AddTransferTask(new MExtAffineTransfer(this, src, srcSize, dst, tile, palette, hflip, vflip));
}

void MExtAffine::Fill(MRect rect, int tile, int palette, bool hflip, bool vflip) {
    gLooper->AddTransferTask(new MExtAffineFill(this, rect, tile, palette, hflip, vflip));
}

MExtAffineTransfer::~MExtAffineTransfer() {
}

MExtAffineFill::~MExtAffineFill() {
}
