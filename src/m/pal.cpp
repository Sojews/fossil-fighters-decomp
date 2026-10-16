// Palette memory.

#include <algorithm>
#include "m/pal.h"
#include "m/looper.h"

extern "C" {
extern u32 data_0208c314[2];    // bytes per palette: normal, extended
}

// Palettes start..start+count of this block. Color 0 of the whole palette memory (the backdrop) is left alone.
MTask *MPalBlock::CreateLoadTask(const void *src, u32 start, u32 count) {
    u32 size = std::min(this->size - start * unitSize, count * unitSize);
    u32 dest = addr + start * unitSize;

    if (dest == pal->mBase) {
        src = (const u8 *)src + 2;
        dest += 2;
        size -= 2;
    }
    return pal->CreateLoadTask(src, (void *)dest, size, 0);
}

void MPalBlock::Load(const void *src, u32 start, u32 count) {
    gLooper->AddTransferTask(CreateLoadTask(src, start, count));
}

// Colors start..start+count of this block.
MTask *MPalBlock::CreateColorLoadTask(const void *src, u32 start, u32 count) {
    u32 size = std::min(this->size - start * 2, count * 2);
    u32 dest = addr + start * 2;

    if (dest == pal->mBase) {
        src = (const u8 *)src + 2;
        dest += 2;
        size -= 2;
    }
    return pal->CreateLoadTask(src, (void *)dest, size, 0);
}

void MPalBlock::LoadColors(const void *src, u32 start, u32 count) {
    gLooper->AddTransferTask(CreateColorLoadTask(src, start, count));
}

MPal::MPal(u32 base, void *extPalette)
    : MHeap(base, data_0208c314[extPalette != NULL ? 1 : 0] << 4, sizeof(MPalBlock)) {
    mUnk38 = 0;
    mExtPalette = extPalette;
    mExtended = extPalette != NULL ? 1 : 0;
    mUnitSize = data_0208c314[mExtended];
}

MPal::~MPal() {
    if (!IsEmpty()) {
        Walk();
        OS_Terminate();
    }
}

MPalBlock *MPal::Alloc(u32 count, const void *data, BOOL fromEnd) {
    MScopedLock lock(&mMutex, TRUE);
    MPalBlock *block;

    if (data != NULL) {
        MHeapIterator it = GetUsedBlocks();
        while ((block = (MPalBlock *)it.block) != NULL) {
            if (block->data == data) {
                block->refCount++;
                return block;
            }
            it.block = block->next;
        }
    }
    block = (MPalBlock *)MHeap::Alloc(count * mUnitSize, fromEnd, mUnitSize, 0, 0, 0);
    block->pal = this;
    block->index = (block->addr - mBase) / mUnitSize;
    block->data = data;
    block->refCount = 1;
    block->unitSize = mUnitSize;
    if (data != NULL) {
        block->Load(data, 0, -1);
    }
    return block;
}

void MPal::Free(MPalBlock *block) {
    MScopedLock lock(&mMutex, TRUE);

    if (--block->refCount != 0) {
        return;
    }
    MHeap::Free(block);
}

MTask *MPal::CreateLoadTask(const void *src, void *dest, u32 size, u8 unk) {
    return new MPalLoadTask(mExtPalette, src, dest, size, unk);
}
