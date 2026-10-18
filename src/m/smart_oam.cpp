// MSmartOam and its blocks, transfers and animation. MSmartOam::Execute (in ITCM) isn't decompiled yet.

#include "m/smart_oam.h"
#include "m/display.h"


void MSmartOamEntry::SetChr(MChrBlock *chr) {
    u32 tile = chr->tile;
    if (colors256 == 1) {
        tile <<= 1;
    }
    this->tile = tile;
}

void MSmartOamEntry::SetPalette(MPalUnit *pal) {
    palette = pal->index;
}

void MSmartOamUnit::SetMode(int mode) {
    u32 n = count;
    MSmartOamEntry *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->gfxMode = mode;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetObjMode(int mode) {
    MSmartOamEntry *entry = GetEntries();
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
    MSmartOamEntry *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->affineParam = param;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetFlip(BOOL hflip, BOOL vflip) {
    MSmartOamEntry *entry = GetEntries();
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
    MSmartOamEntry *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->priority = priority;
            entry++;
        } while (n != 0);
    }
}

void MSmartOamUnit::SetAnchor(int anchor) {
    MSmartOamEntry *entry;
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
    MSmartOamEntry *entry = GetEntries();
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
    MSmartOamEntry *entry = GetEntries();
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
    MSmartOamEntry *entry = GetEntries();
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
    mEntries = new MSmartOamEntry[mCount + 1];
    mOrder = new int[mCount + 1];
    mOrder[0] = mCount;
    mEntries[mCount].links = 0;
    for (u32 i = 1; i < mCount + 1; i++) {
        mOrder[i] = i - 1;
        mEntries[i - 1].links = i;
    }
    MSmartOamEntry *entry = mEntries;
    u32 n = mCount;
    if (n != 0) {
        do {
            n--;
            entry->Clear();
            entry++;
        } while (n != 0);
    }
    entry->Clear();
    mHeap = new MHeap((u32)mEntries, mCount * sizeof(MSmartOamEntry), sizeof(MSmartOamEntry));
}

MSmartOamEntry::MSmartOamEntry() {
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
        (MSmartOamUnit *)mHeap->Alloc(count * sizeof(MSmartOamEntry), fromEnd, 4, 0, 0, 0);
    block->count = count;
    block->owner = this;
    MSmartOamEntry *entry = block->GetEntries();
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
