// Startup: the heap, operator new and delete, and NitroStartUp, which sets up the engine before main.

#include "m/memory.h"
#include "m/irq.h"
#include "m/looper.h"
#include "m/sleep.h"
#include "m/soft_reset.h"
#include "m/bg_task.h"
#include "m/seed_generator.h"
#include "nitro/os.h"

extern "C" void GX_Init(void);

void InitLcd();
void MInitFile();
void MInitArchive();
void RegisterMultiMessages();

// (Empty in this build.)
void func_02052e34() {
}

void *operator new(size_t size) {
    return MemAlloc(size);
}

void *operator new[](size_t size) {
    return MemAlloc(size);
}

void operator delete(void *p) {
    MemFree(p);
}

void operator delete[](void *p) {
    MemFree(p);
}

// (Empty in this build.)
static void func_02052e58() {
}

extern "C" void NitroStartUp() {
    OS_Init();
    func_02052e58();
    GX_Init();
    DC_FlushAll();
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x200);
    OS_SetIrqStackWarningOffset(0x100);

    // The heap takes the whole main arena.
    u32 lo = (u32) OS_GetArenaLo(OS_ARENA_MAIN);
    u32 hi = (u32) OS_GetArenaHi(OS_ARENA_MAIN);
    lo = (lo + 31) & ~31;
    hi = (hi + 31) & ~31;
    MemInit((void *) lo, (void *) hi);
    OS_SetArenaLo(OS_ARENA_MAIN, (void *) hi);

    MInitIrq();
    MEnableIntr(0);
    InitLooper();
    InitLcd();
    MInitFile();
    MInitBgTaskManagers();
    SeedGenerator::Create();
    MInitArchive();
    RegisterMultiMessages();
    MInitSoftReset();
    MInitFoldSleep();
}
