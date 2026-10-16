// A first-fit allocator for an address range that isn't directly accessible memory, such as VRAM or palette
// RAM. It hands out blocks (address and size), and falls back to main memory when the range is full.
#ifndef M_HEAP_H
#define M_HEAP_H

#include "m/task.h"

// A range of addresses.
class MMemoryBlock {
public:
    MMemoryBlock(u32 base, u32 size) {
        mBase = base;
        mSize = size;
    }

    u32 mBase;
    u32 mSize;
};

struct MHeapBlock {
    MHeapBlock *next;
    u32 addr;
    u32 size;
};

// Locks a mutex for the rest of the scope.
class MMutexLock {
public:
    MMutexLock(OSMutex *mutex) {
        mMutex = mutex;
        OS_LockMutex(mMutex);
    }
    ~MMutexLock() { OS_UnlockMutex(mMutex); }

    OSMutex *mMutex;
    u32 mUnk4;
};

// Walks a list of blocks.
struct MHeapIterator {
    MHeapIterator(MHeapBlock *b) : block(b) {}

    MHeapBlock *block;
};

class MHeap : public MMemoryBlock {
public:
    MHeap(u32 base, u32 size, u32 blockSize);
    virtual ~MHeap();

    void SetUnk34(u8 value);
    BOOL IsEmpty();
    MHeapBlock *NewBlock();
    MHeapBlock *Alloc(u32 size, BOOL fromEnd, u32 align, u32 boundary, u32 boundaryCount, u32 limit);
    void Free(u32 addr, BOOL mustBeUsed);
    u32 AllocAddr(u32 size, BOOL fromEnd, u32 align, u32 boundary, u32 boundaryCount, u32 limit);
    void Free(MHeapBlock *block);
    MHeapIterator GetUsedBlocks();
    BOOL Contains(MHeapBlock *block);
    void Walk();

    MHeapBlock *mFree;      // 0x0C: sorted by address
    MHeapBlock *mUsed;      // 0x10
    MHeapBlock *mFallback;  // 0x14: allocated from main memory because the range was full
    u32 mBlockSize;         // 0x18: the size of the block records (subclasses keep more in them)
    OSMutex mMutex;         // 0x1C
    u8 mUnk34;              // 0x34
};

#endif
