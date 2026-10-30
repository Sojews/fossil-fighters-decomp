// Address-range allocator.

#include "m/heap.h"

MHeap::MHeap(u32 base, u32 size, u32 blockSize) : MMemoryBlock(base, size) {
    mUsed = NULL;
    mFallback = NULL;
    mBlockSize = blockSize;
    OS_InitMutex(&mMutex);
    mUnk34 = FALSE;
    mFree = NewBlock();
    mFree->next = NULL;
    mFree->addr = mBase;
    mFree->size = mSize + 1;    // one past the end, so freed blocks always have a free block after them
}

MHeap::~MHeap() {
    MMutexLock lock(&mMutex);
    MHeapBlock *block;
    MHeapBlock *next;

    for (block = mFallback; block != NULL; block = next) {
        delete[] (u8 *)block->addr;
        next = block->next;
        delete block;
    }
    for (block = mFree; block != NULL; block = next) {
        next = block->next;
        delete block;
    }
    for (block = mUsed; block != NULL; block = next) {
        next = block->next;
        delete block;
    }
}

void MHeap::SetUnk34(u8 value) {
    mUnk34 = value;
}

BOOL MHeap::IsEmpty() {
    MMutexLock lock(&mMutex);
    return mUsed == NULL;
}

MHeapBlock *MHeap::NewBlock() {
    return (MHeapBlock *)operator new[](mBlockSize);
}

static inline u32 MAlignUp(u32 v, u32 a) { return (v + a - 1) & ~(a - 1); }
static inline u32 MAlignDown(u32 v, u32 a) { return ~(a - 1) & v; }

MHeapBlock *MHeap::Alloc(u32 size, BOOL fromEnd, u32 align, u32 boundary, u32 boundaryCount, u32 limit) {
    MScopedLock lock(&mMutex, TRUE);
    MHeapBlock *best = NULL;
    u32 bestAddr = 0;
    MHeapBlock **link = &mFree;
    MHeapBlock **bestLink = NULL;
    MHeapBlock *block;
    MHeapBlock *n;

    for (block = mFree; block != NULL; block = block->next) {
        u32 blockSize = block->size;
        u32 blockAddr = block->addr;
        u32 addr;

        if (limit != 0) {
            u32 end = mBase + limit;
            if (blockAddr + blockSize > end) {
                blockSize = end - blockAddr;
                if ((int)blockSize <= 0) {
                    break;
                }
            }
        }
        if (!fromEnd) {
            addr = MAlignUp(blockAddr, align);
            if (boundary != 0
                && MAlignDown(addr + size - 1, boundary) - MAlignDown(addr, boundary) >= boundaryCount * boundary) {
                addr = MAlignUp(blockAddr, boundary);
            }
        } else {
            addr = MAlignDown(blockAddr + blockSize - size, align);
            if (boundary != 0) {
                u32 start = MAlignDown(addr, boundary);
                if (MAlignDown(addr + size - 1, boundary) - start >= boundaryCount * boundary) {
                    addr = start;
                }
            }
        }
        if (blockAddr <= addr && blockAddr + blockSize >= addr + size) {
            best = block;
            bestAddr = addr;
            bestLink = link;
            if (!fromEnd) {
                break;
            }
        }
        link = &block->next;
    }
    if (best == NULL) {
        n = NewBlock();
        n->next = mFallback;
        mFallback = n;
        n->size = size;
        n->addr = (u32) new u8[size];
        return n;
    }
    if (best->size == size && bestAddr == best->addr) {
        *bestLink = best->next;
        best->next = mUsed;
        mUsed = best;
        return best;
    }
    if (bestAddr == best->addr) {
        n = NewBlock();
        n->next = mUsed;
        mUsed = n;
        n->addr = best->addr;
        n->size = size;
        best->addr += size;
        best->size -= size;
        return n;
    }
    u32 freeEnd = best->addr + best->size;
    u32 allocEnd = bestAddr + size;
    if (freeEnd > allocEnd) {
        MHeapBlock *tail = NewBlock();
        tail->next = best->next;
        best->next = tail;
        tail->addr = allocEnd;
        tail->size = freeEnd - allocEnd;
        n = NewBlock();
        n->next = mUsed;
        mUsed = n;
        n->addr = bestAddr;
        n->size = size;
        best->size = bestAddr - best->addr;
        return n;
    }
    if (freeEnd == allocEnd) {
        n = NewBlock();
        n->next = mUsed;
        mUsed = n;
        n->addr = bestAddr;
        n->size = size;
        best->size = bestAddr - best->addr;
        return n;
    }
    return NULL;
}

void MHeap::Free(u32 addr, BOOL mustBeUsed) {
    MMutexLock lock(&mMutex);
    MHeapBlock **link;
    MHeapBlock *block;
    MHeapBlock *prev;
    MHeapBlock *cur;

    link = &mFallback;
    for (block = mFallback; block != NULL; block = block->next) {
        if (block->addr == addr) {
            *link = block->next;
            delete[] (u8 *)block->addr;
            delete block;
            return;
        }
        link = &block->next;
    }
    link = &mUsed;
    for (block = mUsed; block != NULL; block = block->next) {
        if (block->addr == addr) {
            *link = block->next;
            break;
        }
        link = &block->next;
    }
    if (mustBeUsed && block == NULL) {
        return;
    }
    prev = (MHeapBlock *)&mFree;
    for (cur = mFree; cur != NULL; cur = cur->next) {
        if (cur->addr > block->addr) {
            BOOL mergePrev = cur != mFree && prev->addr + prev->size == block->addr;
            BOOL mergeNext = block->addr + block->size == cur->addr;

            if (mergePrev && mergeNext) {
                prev->size += cur->size + block->size;
                prev->next = cur->next;
                delete block;
                delete cur;
            } else if (mergePrev && !mergeNext) {
                prev->size += block->size;
                delete block;
            } else if (!mergePrev && mergeNext) {
                cur->addr = block->addr;
                cur->size += block->size;
                delete block;
            } else {
                block->next = cur;
                prev->next = block;
            }
            return;
        }
        prev = cur;
    }
}

u32 MHeap::AllocAddr(u32 size, BOOL fromEnd, u32 align, u32 boundary, u32 boundaryCount, u32 limit) {
    return Alloc(size, fromEnd, align, boundary, boundaryCount, limit)->addr;
}

void MHeap::Free(MHeapBlock *block) {
    Free(block->addr, FALSE);
}

MHeapIterator MHeap::GetUsedBlocks() {
    return MHeapIterator(mUsed);
}

BOOL MHeap::IsFallback(MHeapBlock *block) {
    if (mBase <= block->addr && block->addr < mBase + mSize) {
        return FALSE;
    }
    return TRUE;
}

void MHeap::Walk() {
    MMutexLock lock(&mMutex);
    MHeapBlock *block;

    for (block = mFree; block != NULL; block = block->next) {
    }
    for (block = mUsed; block != NULL; block = block->next) {
    }
}
