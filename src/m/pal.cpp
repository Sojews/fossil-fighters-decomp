// Palette memory.

#include <algorithm>
#include "m/pal.h"
#include "m/looper.h"
#include "m/lcd.h"
#include "m/transfer.h"

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

// Blends two colors, weight 0-32 toward b.
static inline u16 MBlendColor(u16 a, u16 b, u32 w) {
    u16 blue = ((32 - w) * ((a & 0x7C00) >> 10) + w * ((b & 0x7C00) >> 10)) >> 5 << 10;
    u16 red = ((32 - w) * (a & 0x1F) + w * (b & 0x1F)) >> 5;
    u16 green = ((32 - w) * ((a & 0x3E0) >> 5) + w * ((b & 0x3E0) >> 5)) >> 5 << 5;
    return red | green | blue;
}

MPalAnime::MPalAnime(MPalBlock *block, u32 start, u32 count) {
    mBlock = block;
    mStart = start;
    mCount = std::min(count, block->size / block->unitSize - start);
    mColorCount = mCount * block->unitSize >> 1;
    mLoop = FALSE;
    mLoadStart = 0;
    mLoadCount = 0;
    mColors = new u16[mColorCount];
    mLoops = 1;
    mLoopsLeft = 1;
}

MPalAnime::~MPalAnime() {
    delete[] mColors;
    for (Frame *it = &*mFrames.begin(); it != &*mFrames.end(); it++) {
        if (it->owned) {
            delete[] (u16 *) it->data;
        }
    }
}

void MPalAnime::AddFrame(const u16 *data) {
    Frame frame;
    frame.data = data;
    frame.owned = FALSE;
    frame.color = 0;
    mFrames.push_back(frame);
    mSteps = (mFrames.size() - 1) * 32 + 1;
}

void MPalAnime::AddColor(u16 color) {
    Frame frame;
    frame.data = NULL;
    frame.owned = FALSE;
    frame.color = color;
    mFrames.push_back(frame);
    mSteps = (mFrames.size() - 1) * 32 + 1;
}

void MPalAnime::AddBlend(const u16 *data, u16 color, int weight) {
    if (weight == 0) {
        AddFrame(data);
        return;
    }
    if (weight == 32) {
        AddColor(color);
        return;
    }
    u16 *dst = new u16[mColorCount];
    Frame frame;
    frame.data = dst;
    frame.owned = TRUE;
    frame.color = 0;
    mFrames.push_back(frame);
    for (u32 n = mColorCount; n != 0; n--) {
        *dst++ = MBlendColor(*data++, color, weight);
    }
    mSteps = (mFrames.size() - 1) * 32 + 1;
}

void MPalAnime::SetLoop() {
    mLoop = TRUE;
    mLoops = -1;
    mLoopsLeft = -1;
    mSteps = mFrames.size() * 32;
    mFrames.push_back(mFrames.front());
    mFrames.back().owned = FALSE;
}

void MPalAnime::SetLoadRange(u32 start, u32 count) {
    mLoadStart = start;
    mLoadCount = count;
}

void MPalAnime::Apply(int step) {
    u32 w = step & 0x1F;
    u32 i = (u32) step >> 5;
    Frame *cur = &mFrames[i];
    const u16 *a = cur->data;
    Frame *next = &mFrames[(i + 1) % mFrames.size()];
    const u16 *b = next->data;
    u16 curColor = cur->color;
    u16 nextColor = next->color;
    u16 *dst = mColors;
    if (a != NULL && b != NULL) {
        for (u32 n = mColorCount; n != 0; n--) {
            *dst++ = MBlendColor(*a++, *b++, w);
        }
    } else if (a != NULL && b == NULL) {
        for (u32 n = mColorCount; n != 0; n--) {
            *dst++ = MBlendColor(*a++, nextColor, w);
        }
    } else if (a == NULL && b != NULL) {
        for (u32 n = mColorCount; n != 0; n--) {
            *dst++ = MBlendColor(curColor, *b++, w);
        }
    } else {
        u32 n = mColorCount;
        if (n != 0) {
            u16 color = MBlendColor(curColor, nextColor, w);
            do {
                *dst++ = color;
            } while (--n != 0);
        }
    }
    if (mLoadCount == 0) {
        mBlock->Load(mColors, mStart, mCount);
    } else {
        mBlock->LoadColors(mColors + mLoadStart, mLoadStart, mLoadCount);
    }
}

// NONMATCHING: register allocation only (the game keeps freeSrc in r7 and spills the bank; ours the reverse).
void MPalLoadTask::Transfer() {
    u8 freeSrc = mUnk20;
    int bank = (int)mExtPalette;
    u32 size = mSize;
    void *dest = mDest;
    const void *src = mSrc;
    if (bank == 0) {
        MCopy(src, dest, size);
    } else {
        MVramCnt cnt;
        cnt.raw = 0;
        cnt.mst = 0;
        cnt.offset = 0;
        cnt.enable = TRUE;
        MVramCnt old = gLcd->SetBankCnt(bank, cnt);
        MCopy(src, dest, size);
        MI_WaitDma(M_DMA_NO);
        gLcd->SetBankCnt(bank, old);
    }
    if (freeSrc) {
        delete[] (u8 *)src;
    }
}

MPalLoadTask::~MPalLoadTask() {
}
