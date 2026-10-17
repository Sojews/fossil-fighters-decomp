// NitroSystem foundation: what every heap starts with.
#ifndef NNSYS_FND_HEAPCOMMON_H
#define NNSYS_FND_HEAPCOMMON_H

#include "nnsys/fnd/list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NNSiFndHeapHead NNSiFndHeapHead;
typedef NNSiFndHeapHead *NNSFndHeapHandle;

struct NNSiFndHeapHead {
    u32 signature;
    NNSFndLink link;        // In the list of heaps made in the same memory as this one
    NNSFndList childList;   // The heaps made in this one's memory
    void *heapStart;
    void *heapEnd;
    u32 attribute;          // Bits 0-7: options (NNS_FND_HEAP_OPT_*)
};

#define NNS_FND_HEAP_OPT_0_CLEAR (1 << 0)   // Clear allocated memory

void NNSi_FndInitHeapHead(NNSiFndHeapHead *heap, u32 signature, void *heapStart, void *heapEnd, u16 optFlag);
void NNSi_FndFinalizeHeap(NNSiFndHeapHead *heap);

#ifdef __cplusplus
}
#endif

#endif
