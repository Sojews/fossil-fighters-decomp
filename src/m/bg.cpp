// MBg.

#include "m/bg.h"
#include "m/text.h"
#include "m/bitmap.h"
#include "m/ext_affine.h"
#include "m/chr.h"
#include "m/pal.h"
#include "m/3d.h"

static const u32 sChrBoundaryCounts[] = {2, 4};
static const u32 sChrAligns[] = {0x20, 0x40};
// BG modes 0-5.
static const MBg::Mode sModes[] = {
    {{0, 4}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 3}, {3, 1}, {0, 0}, {0, 0}},
    {{0, 2}, {2, 2}, {0, 0}, {0, 0}},
    {{0, 3}, {0, 0}, {3, 1}, {0, 0}},
    {{0, 2}, {2, 1}, {3, 1}, {0, 0}},
    {{0, 2}, {0, 0}, {2, 2}, {0, 0}},
};

MBg::MBg(MDisplay *display, u32 vramBase, u32 vramSize, u32 paletteBase, ExtPalettes ext, u32 unk44, u32 unk48) {
    mDisplay = display;
    mUnk44 = unk44;
    mUnk48 = unk48;
    mBg0Reserved = FALSE;
    mMode = &sModes[0];
    mDisplay->SetBgMode(0, NULL, NULL, 0);
    mVram = new MHeap(vramBase, vramSize, 12);
    mPal = new MPal(paletteBase, NULL);
    for (u32 i = 0; i < 4; i++) {
        mScreens[i] = NULL;
    }
    for (u32 i = 0; i < 4; i++) {
        mChrs[i] = NULL;
    }
    u32 n;
    for (n = 0; n < 4 && ext.entries[n].palette != NULL; n++) {
        mExtPals[n] = new MPal(ext.entries[n].base, ext.entries[n].palette);
    }
    mExtPalCount = n;
    for (; n < 4; n++) {
        mExtPals[n] = NULL;
    }
}

MBg::~MBg() {
    if (!mVram->IsEmpty()) {
        mVram->Walk();
        OS_Terminate();
    }
    for (u32 i = 0; i < 4; i++) {
        delete mExtPals[i];
    }
    delete mVram;
    delete mPal;
}

void MBg::SetMode(int mode) {
    u32 i;
    const Mode *newMode = &sModes[mode];
    if (newMode == mMode) {
        return;
    }
    int count = 0;
    MScreen *screens[4];
    int from[4];
    int to[4];
    for (i = 0; i < 4; i++) {
        screens[i] = NULL;
    }
    u32 n = 0;
    if (mBg0Reserved) {
        screens[0] = mScreens[0];
        n = 1;
    }
    {
        i = n;
        if (i < mMode->text.count) {
            do {
                u32 bg = mMode->text.start + i;
                if (mScreens[bg] != NULL) {
                    if (n >= newMode->text.count) {
                        OS_Terminate();
                    }
                    u32 newBg = newMode->text.start + n;
                    screens[newBg] = mScreens[bg];
                    if (bg != newBg) {
                        from[count] = bg;
                        to[count] = newBg;
                        count++;
                    }
                    n++;
                }
                i++;
            } while (i < mMode->text.count);
        }
    }
    n = 0;
    {
        i = 0;
        if (i < mMode->affine.count) {
            do {
                u32 bg = mMode->affine.start + i;
                if (mScreens[bg] != NULL) {
                    if (n >= newMode->affine.count) {
                        OS_Terminate();
                    }
                    u32 newBg = newMode->affine.start + n;
                    screens[newBg] = mScreens[bg];
                    if (bg != newBg) {
                        from[count] = bg;
                        to[count] = newBg;
                        count++;
                    }
                    n++;
                }
                i++;
            } while (i < mMode->affine.count);
        }
    }
    n = 0;
    {
        i = 0;
        if (i < mMode->ext.count) {
            do {
                u32 bg = mMode->ext.start + i;
                if (mScreens[bg] != NULL) {
                    if (n >= newMode->ext.count) {
                        OS_Terminate();
                    }
                    u32 newBg = newMode->ext.start + n;
                    screens[newBg] = mScreens[bg];
                    if (bg != newBg) {
                        from[count] = bg;
                        to[count] = newBg;
                        count++;
                    }
                    n++;
                }
                i++;
            } while (i < mMode->ext.count);
        }
    }
    n = 0;
    {
        i = 0;
        if (i < mMode->unk18.count) {
            do {
                u32 bg = mMode->unk18.start + i;
                if (mScreens[bg] != NULL) {
                    if (n >= newMode->unk18.count) {
                        OS_Terminate();
                    }
                    u32 newBg = newMode->unk18.start + n;
                    screens[newBg] = mScreens[bg];
                    if (bg != newBg) {
                        from[count] = bg;
                        to[count] = newBg;
                        count++;
                    }
                    n++;
                }
                i++;
            } while (i < mMode->unk18.count);
        }
    }
    for (i = 0; i < 4; i++) {
        mScreens[i] = screens[i];
        if (mScreens[i] != NULL) {
            mScreens[i]->SetBg(i);
        }
    }
    mMode = newMode;
    mDisplay->SetBgMode(mode, from, to, count);
}

void MBg::Update() {
    MScopedLock lock(&mVram->mMutex, TRUE);
    for (u32 i = 0; i < 4; i++) {
        if (mScreens[i] != NULL) {
            mScreens[i]->Update();
        }
    }
}

void MBg::SetBg0Reserved(BOOL reserved) {
    mBg0Reserved = reserved;
}

MText *MBg::CreateText(int size, BOOL fromEnd) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 bg;
    for (bg = mBg0Reserved; bg < mMode->text.count; bg++) {
        if (mScreens[bg] == NULL) {
            break;
        }
    }
    if (bg >= mMode->text.count) {
        OS_Terminate();
    }
    u32 addr = mVram->AllocAddr(MText::GetVramSize(size), fromEnd, 0x800, 0, 0, 0x10000);
    u32 screenBase = (addr - mVram->mBase) >> 11;
    MText *text = new MText(mDisplay, screenBase, addr, size, bg);
    mScreens[bg] = text;
    if (mExtPalCount != 0) {
        u32 slot;
        MPal *pal;
        if (mExtPalCount == 2) {
            if (bg <= 2) {
                slot = TRUE;
            }
            pal = mExtPals[bg & 1];
        } else {
            slot = FALSE;
            pal = mExtPals[bg];
        }
        text->SetUnk30((u32)pal, slot);
    }
    return text;
}

void MBg::DeleteText(MText *text) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 bg;
    for (bg = 0; bg < mMode->text.count; bg++) {
        if (mScreens[bg] == (MScreen *)text) {
            break;
        }
    }
    text->Hide();
    mVram->Free(text->mBase, FALSE);
    mScreens[bg] = NULL;
    delete text;
}

MDirectBitmap *MBg::CreateDirectBitmap(int size, BOOL fromEnd) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < mMode->ext.count; i++) {
        if (mScreens[mMode->ext.start + i] == NULL) {
            break;
        }
    }
    if (i >= mMode->ext.count) {
        OS_Terminate();
    }
    u32 bg = i + mMode->ext.start;
    u32 addr = mVram->AllocAddr(MDirectBitmap::GetVramSize(size), fromEnd, 0x4000, 0, 0, 0x80000);
    u32 screenBase = (addr - mVram->mBase) >> 14;
    MDirectBitmap *bitmap = new MDirectBitmap(mDisplay, screenBase, addr, size, bg);
    mScreens[bg] = bitmap;
    return bitmap;
}

void MBg::DeleteDirectBitmap(MDirectBitmap *bitmap) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < mMode->ext.count; i++) {
        if (mScreens[mMode->ext.start + i] == (MScreen *)bitmap) {
            break;
        }
    }
    bitmap->Hide();
    mVram->Free(bitmap->mBase, FALSE);
    mScreens[i + mMode->ext.start] = NULL;
    delete bitmap;
}

MIndexBitmap *MBg::CreateIndexBitmap(int size, BOOL fromEnd) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < mMode->ext.count; i++) {
        if (mScreens[mMode->ext.start + i] == NULL) {
            break;
        }
    }
    // (The game checks against minus the count, so this never fails.)
    if (i > -mMode->ext.count) {
        OS_Terminate();
    }
    u32 bg = i + mMode->ext.start;
    u32 addr = mVram->AllocAddr(MIndexBitmap::GetVramSize(size), fromEnd, 0x4000, 0, 0, 0x80000);
    u32 screenBase = (addr - mVram->mBase) >> 14;
    MIndexBitmap *bitmap = new MIndexBitmap(mDisplay, screenBase, addr, size, bg);
    mScreens[bg] = bitmap;
    return bitmap;
}

void MBg::DeleteIndexBitmap(MIndexBitmap *bitmap) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < mMode->ext.count; i++) {
        if (mScreens[mMode->ext.start + i] == (MScreen *)bitmap) {
            break;
        }
    }
    bitmap->Hide();
    mVram->Free(bitmap->mBase, FALSE);
    mScreens[i + mMode->ext.start] = NULL;
    delete bitmap;
}

MExtAffine *MBg::CreateExtAffine(int size, BOOL fromEnd) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < mMode->ext.count; i++) {
        if (mScreens[mMode->ext.start + i] == NULL) {
            break;
        }
    }
    if (i >= mMode->ext.count) {
        OS_Terminate();
    }
    u32 bg = i + mMode->ext.start;
    u32 addr = mVram->AllocAddr(MExtAffine::GetVramSize(size), fromEnd, 0x800, 0, 0, 0x10000);
    u32 screenBase = (addr - mVram->mBase) >> 11;
    MExtAffine *screen = new MExtAffine(mDisplay, screenBase, addr, size, bg);
    mScreens[bg] = screen;
    screen->SetPalette(mExtPals[bg % mExtPalCount]);
    return screen;
}

void MBg::DeleteExtAffine(MExtAffine *screen) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < mMode->ext.count; i++) {
        if (mScreens[mMode->ext.start + i] == (MScreen *)screen) {
            break;
        }
    }
    screen->Hide();
    mVram->Free(screen->mBase, FALSE);
    mScreens[i + mMode->ext.start] = NULL;
    delete screen;
}

MChr *MBg::CreateChr(int mode, u32 units, BOOL fromEnd) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < 4; i++) {
        if (mChrs[i] == NULL) {
            break;
        }
    }
    u32 addr = mVram->AllocAddr(MChr::GetVramSize(mode, units), fromEnd, sChrAligns[mode], 0x4000,
                                sChrBoundaryCounts[mode], 0x40000);
    u32 charOffset;
    u32 base = mVram->mBase;
    u32 charBase = (addr - base) >> 14;
    charOffset = charBase << 14;
    mChrs[i] = new MChr(mDisplay, charBase, base + charOffset, addr, mode, units, 1);
    return mChrs[i];
}

void MBg::DeleteChr(MChr *chr) {
    MScopedLock lock(&mVram->mMutex, TRUE);
    u32 i;
    for (i = 0; i < 4; i++) {
        if (mChrs[i] == chr) {
            break;
        }
    }
    mVram->Free(chr->mBase, FALSE);
    mChrs[i] = NULL;
    delete chr;
}

M3d *MBg::Create3d() {
    mDisplay->SetBg0Is3D(TRUE);
    M3d *screen = new M3d(mDisplay, mUnk44, mUnk48);
    {
        MMutexLock lock(&mVram->mMutex);
        mScreens[0] = screen;
    }
    return screen;
}

void MBg::Delete3d(M3d *screen) {
    {
        MMutexLock lock(&mVram->mMutex);
        mScreens[0] = NULL;
    }
    delete screen;
    mDisplay->SetBg0Is3D(FALSE);
}
