// The game's heap: Doug Lea's malloc 2.7.0 (libs/dlmalloc) running from ITCM, on a bump-allocated arena. The heap
// state lives in DTCM.

#pragma ipa file

#include <stddef.h>
#include "nitro/os.h"
#include "nitro/section.h"

#define REG_IME (*(volatile u16 *)0x04000208)

static inline BOOL DisableIrq(void) {
    u16 prev = REG_IME;
    REG_IME = 0;
    return prev;
}

static inline BOOL RestoreIrq(BOOL enable) {
    u16 prev = REG_IME;
    REG_IME = (u16)enable;
    return prev;
}

void *MemSbrk(int size);
void *malloc(size_t bytes);
void free(void *mem);

#pragma section DTCM begin
u8 *sArenaEnd;
u32 sArenaSize;
u8 *sArenaCur;
BOOL sMemDisableIrq;    // heap calls also block interrupts
BOOL sMemSavedIrq;
OSMutex sMemMutex;
#pragma section DTCM end

size_t gMemAllocated;   // bytes in allocated chunks

void MemInit(void *lo, void *hi) {
    sArenaCur = lo;
    sArenaEnd = hi;
    sArenaSize = (u8 *)hi - (u8 *)lo;
    sMemDisableIrq = FALSE;
    sMemSavedIrq = FALSE;
    OS_InitMutex(&sMemMutex);
}

void *MemSbrk(int size) {
    u8 *prev;

    if (size == 0) {
        return sArenaCur;
    }
    prev = sArenaCur;
    sArenaCur += size;
    if (sArenaCur > sArenaEnd) {
        OS_Terminate();
    }
    return prev;
}

#pragma section ITCM begin

void *MemAlloc(size_t size) {
    void *mem;

    OS_LockMutex(&sMemMutex);
    if (sMemDisableIrq) {
        sMemSavedIrq = DisableIrq();
    }
    mem = malloc(size);
    if (sMemDisableIrq) {
        RestoreIrq(sMemSavedIrq);
    }
    OS_UnlockMutex(&sMemMutex);
    return mem;
}

void MemFree(void *mem) {
    OS_LockMutex(&sMemMutex);
    if (sMemDisableIrq) {
        sMemSavedIrq = DisableIrq();
    }
    free(mem);
    if (sMemDisableIrq) {
        RestoreIrq(sMemSavedIrq);
    }
    OS_UnlockMutex(&sMemMutex);
}

// dlmalloc configuration
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H
#define HAVE_MMAP 0
#define MORECORE MemSbrk
#define MORECORE_CANNOT_TRIM
#define malloc_getpagesize 4096
#define MALLOC_FREE_HOOK(size) (gMemAllocated -= (size))
#define check_malloced_chunk(P, N) (gMemAllocated += chunksize(P))
#define check_remalloced_chunk(P, N) (gMemAllocated += chunksize(P))

#include "../../libs/dlmalloc/malloc.c"

#pragma section ITCM end
