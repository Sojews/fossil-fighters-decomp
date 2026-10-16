// MOam, MAfn and MObj.

#include "m/obj.h"
#include "m/display.h"
#include "m/transfer.h"

void MOamEntry::SetPalette(MPalBlock *pal) {
    palette = pal->index;
}

void MOamBlock::SetMode(int mode) {
    u32 n = count;
    MOamEntry *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->gfxMode = mode;
            entry++;
        } while (n != 0);
    }
}

void MOamBlock::SetPriority(int priority) {
    u32 n = count;
    MOamEntry *entry = GetEntries();
    if (n != 0) {
        do {
            n--;
            entry->priority = priority;
            entry++;
        } while (n != 0);
    }
}

// NONMATCHING: the count and entry pointer get different registers.
void MOamBlock::Clear() {
    u32 n = count;
    MOamEntry *entry = GetEntries();
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

void MOamBlock::Hide() {
    MOamEntry *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->objMode = 2;
            entry++;
        } while (n != 0);
    }
}

void MOamBlock::Show() {
    MOamEntry *entry = GetEntries();
    u32 n = count;
    if (n != 0) {
        do {
            n--;
            entry->objMode = 0;
            entry++;
        } while (n != 0);
    }
}

MOam::MOam(u32 base, u32 size) : MHeap(base, size, sizeof(MOamBlock)) {
    MOamEntry *entry = (MOamEntry *)base;
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

MOamBlock *MOam::Alloc(u32 count, BOOL fromEnd) {
    MOamBlock *block = (MOamBlock *)MHeap::Alloc(count * sizeof(MOamEntry), fromEnd, 4, 0, 0, 0);
    block->count = count;
    return block;
}

void MOam::Free(MOamBlock *block) {
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
