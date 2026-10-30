// MSmartOam and its blocks, transfers and animation.

#include "m/smart_oam.h"
#include "m/display.h"


void MSmartOamCell::SetChr(MChrBlock *chr) {
    u32 tile = chr->tile;
    if (colors256 == 1) {
        tile <<= 1;
    }
    this->tile = tile;
}

void MSmartOamCell::SetPalette(MPalUnit *pal) {
    palette = pal->index;
}

void MSmartOamUnit::SetMode(int mode) {
    u32 n = count;
    MSmartOamCell *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->gfxMode = mode;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetObjMode(int mode) {
    MSmartOamCell *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            entry->objMode = mode;
            if (mode != 3) {
                MPoint size = gObjSizes[0][entry->shape | (entry->size << 2)];
                entry->width = size.x;
                entry->height = size.y;
            } else {
                MPoint size = gObjSizes[0][entry->shape | (entry->size << 2)];
                int width = size.x * 2;
                int height = size.y * 2;
                entry->width = width;
                entry->height = height;
            }
            n--;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetAffineParam(int param) {
    u32 n = count;
    MSmartOamCell *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->affineParam = param;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetFlip(BOOL hflip, BOOL vflip) {
    MSmartOamCell *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->hflip = hflip;
            entry->vflip = vflip;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetPriority(int priority) {
    u32 n = count;
    MSmartOamCell *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->priority = priority;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetAnchor(int anchor) {
    MSmartOamCell *entry;
    u32 n;
    n = count;
    entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->links = (entry->links & 0x3FFFFFFF) | (anchor << 30);
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::Clear() {
    MSmartOamCell *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->Clear();
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::Hide() {
    MSmartOamCell *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->objMode = 2;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::Show() {
    MSmartOamCell *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->objMode = 0;
            entry++;
        } while (n != 0);
    }
}

MSmartOam::MSmartOam(int count, int oamCount, BOOL fromEnd) {
    mDisplay = gCurrentDisplay;
    mCount = count;
    mOamCount = oamCount != 0 ? oamCount : count;
    mUnk1C = 0;
    mOffset.x = 0;
    mOffset.y = 0;
    mHidden = FALSE;
    mUnk3C = 0;
    mUnk40 = 0;
    mClipWidth = 256;
    mClipHeight = 192;
    Init(fromEnd);
}

MSmartOam::MSmartOam(MDisplay *display, int count, int oamCount, BOOL fromEnd) {
    mDisplay = display;
    mCount = count;
    mOamCount = oamCount != 0 ? oamCount : count;
    mUnk1C = 0;
    mOffset.x = 0;
    mOffset.y = 0;
    mHidden = FALSE;
    mUnk3C = 0;
    mUnk40 = 0;
    mClipWidth = 256;
    mClipHeight = 192;
    Init(fromEnd);
}

void MSmartOam::Init(BOOL fromEnd) {
    mOamBlock = mDisplay->mObj->mOam->Alloc(mOamCount, fromEnd);
    mEntries = new MSmartOamCell[mCount + 1];
    mOrder = new int[mCount + 1];
    mOrder[0] = mCount;
    mEntries[mCount].links = 0;
    for (u32 i = 1; i < mCount + 1; i++) {
        mOrder[i] = i - 1;
        mEntries[i - 1].links = i;
    }
    MSmartOamCell *entry = mEntries;
    u32 n = mCount;
    if (n != 0) {
        do {
            n--;
            entry->Clear();
            entry++;
        } while (n != 0);
    }
    entry->Clear();
    mHeap = new MHeap((u32)mEntries, mCount * sizeof(MSmartOamCell), sizeof(MSmartOamCell));
}

MSmartOamCell::MSmartOamCell() {
}

MSmartOam::~MSmartOam() {
    delete mHeap;
    delete[] mOrder;
    delete[] mEntries;
    mDisplay->mObj->mOam->Free(mOamBlock);
}

void MSmartOam::SetUnk1C(int value) {
    mUnk1C = value;
}

MSmartOamUnit *MSmartOam::Alloc(int count, BOOL fromEnd) {
    MScopedLock lock(&mHeap->mMutex, TRUE);
    MSmartOamUnit *block =
        (MSmartOamUnit *)mHeap->Alloc(count * sizeof(MSmartOamCell), fromEnd, 4, 0, 0, 0);
    block->count = count;
    block->owner = this;
    MSmartOamCell *entry = block->GetEntries();
    while (count != 0) {
        count--;
        entry->block = block;
        entry++;
    }
    return block;
}

void MSmartOam::Free(MSmartOamUnit *block) {
    MScopedLock lock(&mHeap->mMutex, TRUE);
    block->Clear();
    mHeap->Free(block);
}

MTask *MSmartOam::CreateOffsetTransfer(MPoint offset) {
    return new MSmartOamOffsetTransfer(this, offset);
}

MTask *MSmartOam::CreateShowTransfer() {
    return new MSmartOamShowTransfer(this);
}

MTask *MSmartOam::CreateHideTransfer() {
    return new MSmartOamHideTransfer(this);
}

MTask *MSmartOam::CreateOffsetAnime(MPoint from, MPoint to, int duration, int ease) {
    return new MSmartOamOffsetAnime(this, from, to, duration, ease);
}

void MSmartOamOffsetAnime::Apply(int step) {
    MPointAnime::Apply(step);
    mOam->SetOffset(MPoint(mValue.x, mValue.y));
}

void MSmartOamHideTransfer::Transfer() {
    mOam->mHidden = TRUE;
}

void MSmartOamShowTransfer::Transfer() {
    mOam->mHidden = FALSE;
}

void MSmartOamOffsetTransfer::Transfer() {
    mOam->SetOffset(mOffset);
}

// Places the entries into the real OAM block every frame: sorted first if asked (by anchor, depth and/or link),
// offset, clipped to the screen; the block's remaining entries are hidden.
// NONMATCHING: register allocation (the game keeps the sort mask in r5 and the cell loop's counters in r9/lr).
#pragma section ITCM begin
#pragma thumb off
void MSmartOam::Execute() {
    MScopedLock lock(&mHeap->mMutex, TRUE);
    if (mHidden) {
        mOamBlock->Hide();
        return;
    }
    if (mUnk1C != 0) {
        int mask = 0;
        if (mUnk1C & 1) {
            mask |= 0xc0000000;
        }
        if (mUnk1C & 2) {
            mask |= 0x3fffc000;
        }
        if (mUnk1C & 4) {
            mask |= 0x3ff;
        }
        for (u32 i = 2; i <= mCount; i++) {
            u32 j = i;
            int index = mOrder[i];
            u32 key = mEntries[index].links & mask;
            while ((mEntries[mOrder[j - 1]].links & mask) > key) {
                mOrder[j] = mOrder[j - 1];
                j--;
            }
            mOrder[j] = index;
        }
    }
    int left = mUnk3C;
    int top = mUnk40;
    int right = mClipWidth;
    int bottom = mClipHeight;
    MOamCell *dst = mOamBlock->GetEntries();
    u32 i = 1;
    u32 used = 0;
    for (; i <= mCount && used < mOamCount; i++) {
        MSmartOamCell *cell = &mEntries[mOrder[i]];
        if (cell->objMode == 2) {
            continue;
        }
        MPoint pos = MPoint(cell->posX - cell->unk08, cell->posY - cell->unk0C) - mOffset;
        if (pos.x + cell->width < left || pos.x >= right || pos.y + cell->height < top || pos.y >= bottom) {
            continue;
        }
        dst->attr01 = cell->attr01;
        dst->attr2 = cell->attr2;
        dst->SetCoord(pos);
        dst++;
        used++;
    }
    for (; used < mOamCount; used++) {
        dst->objMode = 2;
        dst++;
    }
}
#pragma thumb reset
#pragma section ITCM end
