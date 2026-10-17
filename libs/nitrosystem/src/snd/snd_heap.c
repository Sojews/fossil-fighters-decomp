// NitroSystem sound: the sound heap. A frame heap split into sections, one per saved level; each block can
// have a callback that runs when it's freed. (Not heap.c: src/m/heap.cpp has that object name.)

#include "nitro/snd.h"
#include "nnsys/fnd/frameheap.h"
#include "nnsys/snd/heap.h"

#define BLOCK_ALIGN 32

struct NNSSndHeap {
    NNSFndHeapHandle frmHeap;
    NNSFndList sectionList;
};

typedef struct {
    NNSFndList blockList;
    NNSFndLink link;
} Section;

typedef struct {
    NNSFndLink link;
    u32 size;
    NNSSndHeapDisposeCallback callback;
    u32 data1;
    u32 data2;
    u8 padding[8];
} Block;

static BOOL InitHeap(NNSSndHeapHandle heap, NNSFndHeapHandle frmHeap);
static BOOL NewSection(NNSSndHeapHandle heap);
static void EraseSync(void);

NNSSndHeapHandle NNS_SndHeapCreate(void *startAddress, u32 size) {
    void *endAddress = (u8 *) startAddress + size;
    NNSSndHeapHandle heap;
    NNSFndHeapHandle frmHeap;
    startAddress = (void *) (((u32) startAddress + 3) & ~3);
    heap = startAddress;
    if ((u32) heap > (u32) endAddress) {
        return NULL;
    }
    if ((u32) endAddress - (u32) heap < sizeof(struct NNSSndHeap)) {
        return NULL;
    }
    frmHeap = NNS_FndCreateFrmHeapEx(heap + 1, (u32) endAddress - (u32) heap - sizeof(struct NNSSndHeap), 0);
    if (frmHeap == NULL) {
        return NULL;
    }
    if (!InitHeap(heap, frmHeap)) {
        NNS_FndDestroyFrmHeap(frmHeap);
        return NULL;
    }
    return heap;
}

void NNS_SndHeapDestroy(NNSSndHeapHandle heap) {
    NNS_SndHeapClear(heap);
    NNS_FndDestroyFrmHeap(heap->frmHeap);
}

void NNS_SndHeapClear(NNSSndHeapHandle heap) {
    Section *section;
    Block *block;
    BOOL doCallback = FALSE;
    for (section = NNS_FndGetPrevListObject(&heap->sectionList, NULL); section != NULL;
         section = NNS_FndGetPrevListObject(&heap->sectionList, NULL)) {
        for (block = NNS_FndGetPrevListObject(&section->blockList, NULL); block != NULL;
             block = NNS_FndGetPrevListObject(&section->blockList, block)) {
            if (block->callback != NULL) {
                block->callback(block + 1, block->size, block->data1, block->data2);
                doCallback = TRUE;
            }
        }
        NNS_FndRemoveListObject(&heap->sectionList, section);
    }
    NNS_FndFreeToFrmHeap(heap->frmHeap, NNS_FND_FRMHEAP_FREE_HEAD | NNS_FND_FRMHEAP_FREE_TAIL);
    if (doCallback) {
        EraseSync();
    }
    NewSection(heap);
}

void *NNS_SndHeapAlloc(NNSSndHeapHandle heap, u32 size, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2) {
    Block *block = NNS_FndAllocFromFrmHeapEx(heap->frmHeap, ((size + BLOCK_ALIGN - 1) & ~(BLOCK_ALIGN - 1)) + sizeof(Block),
                                             BLOCK_ALIGN);
    Section *section;
    if (block == NULL) {
        return NULL;
    }
    section = NNS_FndGetPrevListObject(&heap->sectionList, NULL);
    block->size = size;
    block->callback = callback;
    block->data1 = data1;
    block->data2 = data2;
    NNS_FndAppendListObject(&section->blockList, block);
    return block + 1;
}

int NNS_SndHeapSaveState(NNSSndHeapHandle heap) {
    if (!NNS_FndRecordStateForFrmHeap(heap->frmHeap, heap->sectionList.numObjects)) {
        return -1;
    }
    if (!NewSection(heap)) {
        NNS_FndFreeByStateToFrmHeap(heap->frmHeap, 0);
        return -1;
    }
    return heap->sectionList.numObjects - 1;
}

void NNS_SndHeapLoadState(NNSSndHeapHandle heap, int level) {
    Section *section;
    Block *block = NULL;
    BOOL doCallback = FALSE;
    if (level == 0) {
        NNS_SndHeapClear(heap);
        return;
    }
    while (level < heap->sectionList.numObjects) {
        section = NNS_FndGetPrevListObject(&heap->sectionList, NULL);
        while ((block = NNS_FndGetPrevListObject(&section->blockList, block)) != NULL) {
            if (block->callback != NULL) {
                block->callback(block + 1, block->size, block->data1, block->data2);
                doCallback = TRUE;
            }
        }
        NNS_FndRemoveListObject(&heap->sectionList, section);
    }
    NNS_FndFreeByStateToFrmHeap(heap->frmHeap, level);
    if (doCallback) {
        EraseSync();
    }
    NNS_FndRecordStateForFrmHeap(heap->frmHeap, heap->sectionList.numObjects);
    NewSection(heap);
}

u32 NNS_SndHeapGetFreeSize(NNSSndHeapHandle heap) {
    u32 size = NNS_FndGetAllocatableSizeForFrmHeapEx(heap->frmHeap, BLOCK_ALIGN);
    if (size < sizeof(Block)) {
        return 0;
    }
    return (size - sizeof(Block)) & ~(BLOCK_ALIGN - 1);
}

static void InitHeapSection(Section *section) {
    NNS_FndInitList(&section->blockList, 0);
}

static BOOL InitHeap(NNSSndHeapHandle heap, NNSFndHeapHandle frmHeap) {
    NNS_FndInitList(&heap->sectionList, 0xc);
    heap->frmHeap = frmHeap;
    if (!NewSection(heap)) {
        return FALSE;
    }
    return TRUE;
}

static BOOL NewSection(NNSSndHeapHandle heap) {
    Section *section = NNS_FndAllocFromFrmHeapEx(heap->frmHeap, sizeof(Section), 4);
    if (section == NULL) {
        return FALSE;
    }
    InitHeapSection(section);
    NNS_FndAppendListObject(&heap->sectionList, section);
    return TRUE;
}

static void EraseSync(void) {
    u32 tag = SND_GetCurrentCommandTag();
    SND_FlushCommand(SND_COMMAND_BLOCK);
    SND_WaitForCommandProc(tag);
}
