// Address-range allocator.

#include "m/heap.h"

MHeap::MHeap(u32 base, u32 size, u32 unit) : MMemoryBlock(base, size) {
    mUsed = NULL;
    mFallback = NULL;
    mUnit = unit;
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
    return (MHeapBlock *)operator new[](mUnit);
}

MHeapBlock *MHeap::Alloc(u32 size, BOOL fromEnd, u32 align, u32 boundary, u32 boundaryCount, u32 limit) {
    MScopedLock lock(&mMutex, TRUE);
    u32 bestAddr = 0;
    MHeapBlock **bestLink = NULL;
    MHeapBlock *best = NULL;
    MHeapBlock **link = &mFree;
    MHeapBlock *block;
    MHeapBlock *n;

    for (block = mFree; block != NULL; block = block->next) {
        u32 blockAddr = block->addr;
        u32 blockSize = block->size;
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
            addr = (blockAddr + align - 1) & ~(align - 1);
            if (boundary != 0
                && ((addr + size - 1) & ~(boundary - 1)) - (addr & ~(boundary - 1)) >= boundary * boundaryCount) {
                addr = (blockAddr + boundary - 1) & ~(boundary - 1);
            }
        } else {
            addr = (blockAddr + blockSize - size) & ~(align - 1);
            if (boundary != 0) {
                u32 start = addr & ~(boundary - 1);
                if (((addr + size - 1) & ~(boundary - 1)) - start >= boundary * boundaryCount) {
                    addr = start;
                }
            }
        }
        if (addr >= blockAddr && blockAddr + blockSize >= addr + size) {
            bestAddr = addr;
            best = block;
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

BOOL MHeap::Contains(MHeapBlock *block) {
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
