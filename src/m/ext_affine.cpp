// MExtAffine and its transfers. Not decompiled yet: the tile map copy with flips (func_0204c5f0).

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

// Copies a w x h block of tile entries from (x, y) of the source to (dx, dy) of the map, with the flips.
extern "C" void func_0204c5f0(int blocks, const u16 *src, int srcWidth, int srcHeight, int x, int y, int w, int h,
                              u32 base, int size, int dx, int dy, u16 attr, u16 flip);

// The copy wraps round the map's right edge: it's done in two parts (in the other order when flipped).
void MExtAffineTransfer::Transfer() {
    int blocks = MExtAffine::GetDimensions(mDstSize).x >> 3;
    mDst.x &= blocks - 1;
    mDst.y &= blocks - 1;
    const u16 *src;
    int srcWidth;
    int srcHeight;
    int x;
    int y;
    u32 base;
    int size;
    int dy;
    u16 attr;
    int h;
    int w;
    dy = mDst.y;
    u16 flip = mFlip;
    attr = mAttr;
    size = mDstSize;
    base = mDstBase;
    int dx = mDst.x;
    h = mSrcRect.GetHeight();
    w = mSrcRect.GetWidth();
    y = mSrcRect.top;
    x = mSrcRect.left;
    srcHeight = mSrcSize.y;
    srcWidth = mSrcSize.x;
    src = mSrc;
    if (dx + w < blocks) {
        func_0204c5f0(blocks, src, srcWidth, srcHeight, x, y, w, h, base, size, dx, dy, attr, flip);
    } else if (!(flip & 0x400)) {
        int first = blocks - dx;
        func_0204c5f0(blocks, src, srcWidth, srcHeight, x, y, first, h, base, size, dx, dy, attr, flip);
        func_0204c5f0(blocks, src, srcWidth, srcHeight, x + blocks - dx, y, w - first, h, base, size, 0, dy, attr,
                      flip);
    } else {
        int first = blocks - dx;
        func_0204c5f0(blocks, src, srcWidth, srcHeight, x + w - first, y, first, h, base, size, dx, dy, attr, flip);
        func_0204c5f0(blocks, src, srcWidth, srcHeight, x, y, w - first, h, base, size, 0, dy, attr, flip);
    }
}

MExtAffineFill::~MExtAffineFill() {
}

// Fills a w x h block of entries from p, in a map that's blocks entries wide.
#define MEXT_AFFINE_FILL_BLOCK(start, w, h) \
    { \
        int i; \
        u16 *p = start; \
        for (int j = h; j != 0; j--, p += blocks - (w)) { \
            for (i = w; i != 0; i--) { \
                *p++ = entry; \
            } \
        } \
    }

// The fill wraps round the map's edges.
void MExtAffineFill::Transfer() {
    int dx = mRect.left;
    int dy = mRect.top;
    int w = mRect.GetWidth();
    int h = mRect.GetHeight();
    int blocks = MExtAffine::GetDimensions(mDstSize).x >> 3;
    dx &= blocks - 1;
    dy &= blocks - 1;
    u16 entry = mEntry;
    u16 *base;
    int size;
    size = mDstSize;
    base = (u16 *)mDstBase;
    if (dx + w < blocks) {
        if (dy + h < blocks) {
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, dx, dy), w, h);
        } else {
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, dx, dy), w, blocks - dy);
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, dx, 0), w, h - (blocks - dy));
        }
    } else {
        if (dy + h < blocks) {
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, dx, dy), blocks - dx, h);
        } else {
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, dx, dy), blocks - dx, blocks - dy);
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, dx, 0), blocks - dx, h - (blocks - dy));
        }
        if (dy + h < blocks) {
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, 0, dy), w - (blocks - dx), h);
        } else {
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, 0, dy), w - (blocks - dx), blocks - dy);
            MEXT_AFFINE_FILL_BLOCK(MExtAffine::GetEntry(base, size, 0, 0), w - (blocks - dx), h - (blocks - dy));
        }
    }
}
