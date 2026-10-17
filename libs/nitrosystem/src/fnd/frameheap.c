// NitroSystem foundation: frame heaps, which allocate from either end and free everything at once.

#include <stdlib.h>
#include "nitro/mi.h"
#include "nnsys/fnd/frameheap.h"

#define NNSI_FRMHEAP_SIGNATURE 0x46524d48   // 'FRMH'

#define ROUND_UP(value, align) ((((align) - 1) + (u32) (value)) & ~((align) - 1))
#define ROUND_DOWN(value, align) ((u32) (value) & ~((align) - 1))

static inline NNSiFndFrmHeapHead *GetFrmHeapHead(NNSiFndHeapHead *heap) {
    return (NNSiFndFrmHeapHead *) ((u8 *) heap + sizeof(NNSiFndHeapHead));
}

static inline NNSiFndHeapHead *GetHeapHead(NNSiFndFrmHeapHead *frmHeap) {
    return (NNSiFndHeapHead *) ((u8 *) frmHeap - sizeof(NNSiFndHeapHead));
}

static inline u16 GetOpt(NNSiFndHeapHead *heap) {
    return (u16) (heap->attribute & 0xff);
}

static inline void FillAllocMemory(NNSiFndHeapHead *heap, void *address, u32 size) {
    if (GetOpt(heap) & NNS_FND_HEAP_OPT_0_CLEAR) {
        MIi_CpuClear32(0, address, size);
    }
}

static NNSiFndHeapHead *InitFrameHeap(void *startAddress, void *endAddress, u16 optFlag) {
    NNSiFndHeapHead *heap = startAddress;
    NNSiFndFrmHeapHead *frmHeap = GetFrmHeapHead(heap);
    NNSi_FndInitHeapHead(heap, NNSI_FRMHEAP_SIGNATURE, frmHeap + 1, endAddress, optFlag);
    frmHeap->headAllocator = heap->heapStart;
    frmHeap->tailAllocator = heap->heapEnd;
    frmHeap->pState = NULL;
    return heap;
}

static void *AllocFromHead(NNSiFndFrmHeapHead *frmHeap, u32 size, int alignment) {
    void *newBlock = (void *) ROUND_UP(frmHeap->headAllocator, alignment);
    void *endAddress = (void *) (size + (u32) newBlock);
    if ((u32) endAddress > (u32) frmHeap->tailAllocator) {
        return NULL;
    }
    FillAllocMemory(GetHeapHead(frmHeap), frmHeap->headAllocator,
                    (u32) endAddress - (u32) frmHeap->headAllocator);
    frmHeap->headAllocator = endAddress;
    return newBlock;
}

static void *AllocFromTail(NNSiFndFrmHeapHead *frmHeap, u32 size, int alignment) {
    void *newBlock = (void *) ROUND_DOWN((u32) frmHeap->tailAllocator - size, alignment);
    if ((u32) newBlock < (u32) frmHeap->headAllocator) {
        return NULL;
    }
    FillAllocMemory(GetHeapHead(frmHeap), newBlock, (u32) frmHeap->tailAllocator - (u32) newBlock);
    frmHeap->tailAllocator = newBlock;
    return newBlock;
}

static void FreeHead(NNSiFndHeapHead *heap) {
    NNSiFndFrmHeapHead *frmHeap = GetFrmHeapHead(heap);
    frmHeap->headAllocator = heap->heapStart;
    frmHeap->pState = NULL;
}

static void FreeTail(NNSiFndHeapHead *heap) {
    NNSiFndFrmHeapHead *frmHeap = GetFrmHeapHead(heap);
    NNSiFndFrmHeapState *state;
    for (state = frmHeap->pState; state != NULL; state = state->pPrevState) {
        state->tailAllocator = heap->heapEnd;
    }
    frmHeap->tailAllocator = heap->heapEnd;
}

NNSFndHeapHandle NNS_FndCreateFrmHeapEx(void *startAddress, u32 size, u16 optFlag) {
    void *endAddress = (void *) ROUND_DOWN(size + (u32) startAddress, 4);
    startAddress = (void *) ROUND_UP(startAddress, 4);
    if ((u32) startAddress > (u32) endAddress ||
        (u32) endAddress - (u32) startAddress < sizeof(NNSiFndHeapHead) + sizeof(NNSiFndFrmHeapHead)) {
        return NULL;
    }
    return InitFrameHeap(startAddress, endAddress, optFlag);
}

void NNS_FndDestroyFrmHeap(NNSFndHeapHandle heap) {
    NNSi_FndFinalizeHeap(heap);
}

void *NNS_FndAllocFromFrmHeapEx(NNSFndHeapHandle heap, u32 size, int alignment) {
    NNSiFndFrmHeapHead *frmHeap = GetFrmHeapHead(heap);
    if (size == 0) {
        size = 1;
    }
    size = ROUND_UP(size, 4);
    if (alignment >= 0) {
        return AllocFromHead(frmHeap, size, alignment);
    }
    return AllocFromTail(frmHeap, size, -alignment);
}

void NNS_FndFreeToFrmHeap(NNSFndHeapHandle heap, int mode) {
    if (mode & NNS_FND_FRMHEAP_FREE_HEAD) {
        FreeHead(heap);
    }
    if (mode & NNS_FND_FRMHEAP_FREE_TAIL) {
        FreeTail(heap);
    }
}

u32 NNS_FndGetAllocatableSizeForFrmHeapEx(NNSFndHeapHandle heap, int alignment) {
    NNSiFndFrmHeapHead *frmHeap;
    void *newBlock;
    alignment = abs(alignment);
    frmHeap = GetFrmHeapHead(heap);
    newBlock = (void *) ROUND_UP(frmHeap->headAllocator, alignment);
    if ((u32) newBlock > (u32) frmHeap->tailAllocator) {
        return 0;
    }
    return (u32) frmHeap->tailAllocator - (u32) newBlock;
}

BOOL NNS_FndRecordStateForFrmHeap(NNSFndHeapHandle heap, u32 tagName) {
    NNSiFndFrmHeapHead *frmHeap = GetFrmHeapHead(heap);
    void *oldHead = frmHeap->headAllocator;
    NNSiFndFrmHeapState *state = AllocFromHead(frmHeap, sizeof(NNSiFndFrmHeapState), 4);
    if (state == NULL) {
        return FALSE;
    }
    state->tagName = tagName;
    state->headAllocator = oldHead;
    state->tailAllocator = frmHeap->tailAllocator;
    state->pPrevState = frmHeap->pState;
    frmHeap->pState = state;
    return TRUE;
}

BOOL NNS_FndFreeByStateToFrmHeap(NNSFndHeapHandle heap, u32 tagName) {
    NNSiFndFrmHeapHead *frmHeap = GetFrmHeapHead(heap);
    NNSiFndFrmHeapState *state = frmHeap->pState;
    if (tagName != 0) {
        for (; state != NULL; state = state->pPrevState) {
            if (state->tagName == tagName) {
                break;
            }
        }
    }
    if (state == NULL) {
        return FALSE;
    }
    frmHeap->headAllocator = state->headAllocator;
    frmHeap->tailAllocator = state->tailAllocator;
    frmHeap->pState = state->pPrevState;
    return TRUE;
}
