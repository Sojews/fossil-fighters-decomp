// MChr, MOam, MAfn and MObj.

#include <algorithm>
#include "m/obj.h"
#include "m/display.h"
#include "m/looper.h"
#include "m/transfer.h"

static const u32 sChrShifts[2] = {5, 6};
static const u32 sChrUnitSizes[2] = {32, 64};

// (The game's pool puts a word and three type infos, 0x28 bytes, ahead of the size tables; __sinit reaches the first
// table from the pool's start.)
static u32 sObjPoolHead[10];

MPoint gObjTileSizes[4][4] = {
    {MPoint(1, 1), MPoint(2, 1), MPoint(1, 2), MPoint()},
    {MPoint(2, 2), MPoint(4, 1), MPoint(1, 4), MPoint()},
    {MPoint(4, 4), MPoint(4, 2), MPoint(2, 4), MPoint()},
    {MPoint(8, 8), MPoint(8, 4), MPoint(4, 8), MPoint()},
};

MPoint gObjSizes[4][4] = {
    {MPoint(8, 8), MPoint(16, 8), MPoint(8, 16), MPoint(0, 0)},
    {MPoint(16, 16), MPoint(32, 8), MPoint(8, 32), MPoint(0, 0)},
    {MPoint(32, 32), MPoint(32, 16), MPoint(16, 32), MPoint(0, 0)},
    {MPoint(64, 64), MPoint(64, 32), MPoint(32, 64), MPoint(0, 0)},
};

MTask *MChrBlock::CreateLoadTask(const void *src, u32 start, u32 count, int palette) {
    count = std::min(unitSize == 32 ? size >> 5 : size >> 6, count);
    u32 bytes = count * unitSize;
    if (unitSize == 32) {
        return new MSimpleTransfer(src, (void *)(addr + start * unitSize), bytes, FALSE);
    }
    return new MChr256Transfer(src, (void *)(addr + start * unitSize), bytes, palette);
}

void MChrBlock::Load(const void *src, u32 start, u32 count, int palette) {
    gLooper->AddTransferTask(CreateLoadTask(src, start, count, palette));
}

// Built without common-subexpression elimination (the 16-color path reloads the unit size).
#pragma opt_common_subs off
void MChrBlock::Load(MArchiveFile *file, u32 offset, u32 start, u32 unused, u32 count, int palette) {
    u32 bytes = std::min(size - start * unitSize, count * unitSize);
    offset *= unitSize;
    if (unitSize == 32) {
        file->archive->Read(file->index, offset, (void *)(addr + start * unitSize), bytes, NULL);
    } else {
        file->archive->Read(file->index, offset, (void *)(addr + start * unitSize), bytes,
                            new MChr256Filter(palette));
    }
}
#pragma opt_common_subs reset

MTask *MChrBlock::CreateFillTask(int color, u32 start, u32 count) {
    u32 bytes = std::min(size - start * unitSize, count * unitSize);
    if (unitSize == 32) {
        return new MSimpleFill(color * 0x11111111, (void *)(addr + start * unitSize), bytes);
    }
    return new MSimpleFill(color * 0x01010101, (void *)(addr + start * unitSize), bytes);
}

void MChrBlock::Fill(int color, u32 start, u32 count) {
    gLooper->AddTransferTask(CreateFillTask(color, start, count));
}

u32 MChr::GetVramSize(int mode, u32 units) {
    return sChrUnitSizes[mode] * units;
}

MChr::MChr(MDisplay *display, int charBase, u32 unk44, u32 base, int mode, u32 units, int boundary)
    : MHeap(base, sChrUnitSizes[mode] * units, sizeof(MChrBlock)) {
    mDisplay = display;
    mMode = mode;
    mCharBase = charBase;
    mUnk44 = unk44;
    mBoundary = boundary;
}

MChrBlock *MChr::Alloc(u32 count, BOOL fromEnd) {
    return Alloc(mMode, count, fromEnd);
}

MChrBlock *MChr::Alloc(int mode, u32 count, BOOL fromEnd) {
    MScopedLock lock(&mMutex, TRUE);
    u32 unit = sChrUnitSizes[mode];
    MChrBlock *block = (MChrBlock *)MHeap::Alloc(count * unit, fromEnd, mBoundary * unit, 0, 0, 0);
    block->unitSize = unit;
    block->key = NULL;
    block->refCount = 1;
    if (!IsFallback(block)) {
        block->index = (int)(block->addr - mUnk44) >> sChrShifts[mode];
        block->tile = block->index / mBoundary;
    } else {
        block->index = (int)(mBase - mUnk44) >> sChrShifts[mode];
        block->tile = block->index / mBoundary;
    }
    return block;
}

MChrBlock *MChr::AllocShared(const void *data, u32 count, int palette, BOOL fromEnd) {
    return AllocShared(mMode, data, count, palette, fromEnd);
}

// Returns the block already holding data, or allocates one and loads data into it.
MChrBlock *MChr::AllocShared(int mode, const void *data, u32 count, int palette, BOOL fromEnd) {
    MScopedLock lock(&mMutex, TRUE);
    for (MHeapIterator it = GetUsedBlocks(); it.block != NULL; it.block = it.block->next) {
        MChrBlock *block = (MChrBlock *)it.block;
        if (block->key == data) {
            block->refCount++;
            return block;
        }
    }
    MChrBlock *block = Alloc(mode, count, fromEnd);
    block->key = data;
    block->Load(data, 0, 0x7FFF, palette);
    return block;
}

void MChr::FreeShared(MChrBlock *block) {
    MScopedLock lock(&mMutex, TRUE);
    if (--block->refCount != 0) {
        return;
    }
    MHeap::Free(block);
}

void MChr256Filter::Filter(const void *src, void *dest, u32 size) {
    MAddPaletteOffset((const u16 *)src, (u16 *)dest, size, mPalette);
}

void MOamCell::SetPalette(MPalUnit *pal) {
    palette = pal->index;
}

void MOamUnit::SetMode(int mode) {
    u32 n = count;
    MOamCell *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->gfxMode = mode;
            entry++;
        } while (n != 0);
    }
}

void MOamUnit::SetPriority(int priority) {
    u32 n = count;
    MOamCell *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->priority = priority;
            entry++;
        } while (n != 0);
    }
}

void MOamUnit::Clear() {
    MOamCell *entry;
    u32 n;
    n = count;
    entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->attr01 = 0;
            entry->attr2 = 0;
            entry->objMode = 2;
            entry++;
        } while (n != 0);
    }
}

void MOamUnit::Hide() {
    MOamCell *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->objMode = 2;
            entry++;
        } while (n != 0);
    }
}

void MOamUnit::Show() {
    MOamCell *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->objMode = 0;
            entry++;
        } while (n != 0);
    }
}

MOam::MOam(u32 base, u32 size) : MHeap(base, size, sizeof(MOamUnit)) {
    MOamCell *entry = (MOamCell *)base;
    for (u32 i = 0; i < 128; i++) {
        entry->Clear();
        entry++;
    }
}

MOam::~MOam() {
    if (!IsEmpty()) {
        Walk();
        OS_Terminate();
    }
}

MOamUnit *MOam::Alloc(u32 count, BOOL fromEnd) {
    MOamUnit *block = (MOamUnit *)MHeap::Alloc(count * sizeof(MOamCell), fromEnd, 4, 0, 0, 0);
    block->count = count;
    return block;
}

void MOam::Free(MOamUnit *block) {
    block->Clear();
    MHeap::Free(block);
}

MAfn::MAfn(u32 base, u32 size) : MHeap(base, size, sizeof(MAfnBlock)) {
}

MAfn::~MAfn() {
    if (!IsEmpty()) {
        Walk();
        OS_Terminate();
    }
}

MAfnBlock *MAfn::Alloc(u32 count, BOOL fromEnd) {
    MAfnBlock *block = (MAfnBlock *)MHeap::Alloc(count * 32, fromEnd, 4, 0, 0, 0);
    block->index = (block->addr - mBase) >> 5;
    return block;
}

void MAfn::Free(MAfnBlock *block) {
    MHeap::Free(block);
}

MObj::MObj(MDisplay *display, u32 oam, u32 chrBase, u32 chrSize, u32 palBase, u32 extPalBase, void *extPalette) {
    mDisplay = display;
    mOamAddr = oam;
    mShadow = new u8[0x400];

    u32 blocks = 1;
    u32 units = chrSize >> 5;
    if (units > 1024) {
        blocks = units >> 10;
    }
    int boundary;
    switch (blocks) {
    case 1:
    default:
        boundary = 0;
        break;
    case 2:
        boundary = 1;
        break;
    case 4:
        boundary = 2;
        break;
    case 8:
        boundary = 3;
        break;
    }
    mDisplay->SetObjTileBoundary(boundary);
    mChr = new MChr(mDisplay, 0, chrBase, chrBase, 0, units, blocks);
    mPal = new MPal(palBase, NULL);
    mOam = new MOam((u32)mShadow, 0x400);
    mAfn = new MAfn((u32)mShadow, 0x400);
    if (extPalBase != 0) {
        mExtPal = new MPal(extPalBase, extPalette);
    } else {
        mExtPal = NULL;
    }
}

MObj::~MObj() {
    delete mExtPal;
    delete mPal;
    delete mChr;
    delete mOam;
    delete mAfn;
    delete[] mShadow;
}

void MObj::Update() {
    MCopy(mShadow, (void *)mOamAddr, 0x400);
}

void MObj::Show() {
    mDisplay->ShowPlanes(0x10);
}

void MObj::Hide() {
    mDisplay->HidePlanes(0x10);
}
