// NitroSystem foundation: what every heap shares, and the tree of heaps made inside each other.

#include "nnsys/fnd/heapcommon.h"

static NNSFndList sRootList;    // Heaps not made inside another heap
static BOOL sRootListInitialized;

// The innermost heap whose memory holds memBlock.
static NNSiFndHeapHead *FindContainHeap(NNSFndList *list, const void *memBlock) {
    NNSiFndHeapHead *heap = NULL;
    while ((heap = NNS_FndGetNextListObject(list, heap)) != NULL) {
        if (heap->heapStart <= memBlock && memBlock < heap->heapEnd) {
            NNSiFndHeapHead *child = FindContainHeap(&heap->childList, memBlock);
            if (child != NULL) {
                return child;
            }
            return heap;
        }
    }
    return NULL;
}

// The list a heap made at memBlock goes in.
static NNSFndList *FindListContainHeap(const void *memBlock) {
    NNSFndList *list = &sRootList;
    NNSiFndHeapHead *heap = FindContainHeap(&sRootList, memBlock);
    if (heap != NULL) {
        list = &heap->childList;
    }
    return list;
}

void NNSi_FndInitHeapHead(NNSiFndHeapHead *heap, u32 signature, void *heapStart, void *heapEnd, u16 optFlag) {
    heap->signature = signature;
    heap->heapStart = heapStart;
    heap->heapEnd = heapEnd;
    heap->attribute = 0;
    heap->attribute &= ~0xff;
    heap->attribute |= (u8) optFlag;
    NNS_FndInitList(&heap->childList, 4);
    if (!sRootListInitialized) {
        NNS_FndInitList(&sRootList, 4);
        sRootListInitialized = TRUE;
    }
    NNS_FndAppendListObject(FindListContainHeap(heap), heap);
}

void NNSi_FndFinalizeHeap(NNSiFndHeapHead *heap) {
    NNS_FndRemoveListObject(FindListContainHeap(heap), heap);
}
