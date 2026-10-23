// MExtAffine and its transfers.

#include "m/ext_affine.h"
#include "game/dino_static_init.h"

// The screen sizes: (128, 128), (256, 256), (512, 512), (1024, 1024), set by the file's static initialization.
extern "C" MPoint sSizes[4];

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

// Copies h rows of w tile entries from (x, y) of the source into the map from (dx, dy), adding attr and
// flipping with flip (the rows go bottom up when vertically flipped, each row right to left when horizontally).
static inline void MExtAffineCopyTiles(const u16 *src, int srcWidth, int x, int y, int w, int h, u32 base, int size,
                                       int blocks, int dx, int dy, u16 attr, u16 flip) {
    const u16 *s;
    if (!(flip & 0x800)) {
        if (!(flip & 0x400)) {
            u16 *d = MExtAffine::GetEntry((u16 *) base, size, dx, dy);
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d = (*s++ + attr) ^ flip;
                    d++;
                }
                rows--;
                d += blocks - w;
                s += srcWidth - w;
            }
        } else {
            u16 *d = MExtAffine::GetEntry((u16 *) base, size, dx + w - 1, dy);
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d-- = (*s++ + attr) ^ flip;
                }
                rows--;
                d += blocks + w;
                s += srcWidth - w;
            }
        }
    } else {
        if (!(flip & 0x400)) {
            u16 *d = MExtAffine::GetEntry((u16 *) base, size, dx, dy + h - 1);
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d++ = (*s++ + attr) ^ flip;
                }
                rows--;
                d += -(blocks + w);
                s += srcWidth - w;
            }
        } else {
            u16 *d = MExtAffine::GetEntry((u16 *) base, size, dx + w - 1, dy + h - 1);
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d-- = (*s++ + attr) ^ flip;
                }
                rows--;
                d += -(blocks - w);
                s += srcWidth - w;
            }
        }
    }
}

// Copies a w x h block of tile entries from (x, y) of the source to (dx, dy) of the map, with the flips; a copy
// crossing the map's bottom edge goes on at its top (the source's last rows first when flipped).
extern "C" void func_0204c5f0(int blocks, const u16 *src, int srcWidth, int srcHeight, int x, int y, int w, int h,
                              u32 base, int size, int dx, int dy, u16 attr, u16 flip) {
    if (dy + h < blocks) {
        MExtAffineCopyTiles(src, srcWidth, x, y, w, h, base, size, blocks, dx, dy, attr, flip);
    } else if (!(flip & 0x800)) {
        MExtAffineCopyTiles(src, srcWidth, x, y, w, blocks - dy, base, size, blocks, dx, dy, attr, flip);
        MExtAffineCopyTiles(src, srcWidth, x, y + blocks - dy, w, h - (blocks - dy), base, size, blocks, dx, 0, attr,
                            flip);
    } else {
        MExtAffineCopyTiles(src, srcWidth, x, y + h - (blocks - dy), w, blocks - dy, base, size, blocks, dx, dy, attr,
                            flip);
        MExtAffineCopyTiles(src, srcWidth, x, y, w, h - (blocks - dy), base, size, blocks, dx, 0, attr, flip);
    }
}

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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_020c0838[];
extern "C" u32 data_020c0874[];
extern "C" u32 data_020c087c[];
extern "C" u32 data_020c0884[];
static struct ExtAffineStaticInit {
    ExtAffineStaticInit() {
        DINO_POOL_WORD(data_020c0838, 0x34) = 128;
        DINO_POOL_WORD(data_020c0838, 0x38) = 128;
        volatile MPoint *p = (volatile MPoint *) data_020c0874;
        p->x = 256;
        p->y = 256;
        p = (volatile MPoint *) data_020c087c;
        p->x = 512;
        p->y = 512;
        p = (volatile MPoint *) data_020c0884;
        p->x = 1024;
        p->y = 1024;
    }
} sExtAffineStaticInit;
