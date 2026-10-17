// NitroSDK OS: memory arenas (the free ranges of each memory left over after the program).

#include "nitro/os.h"

#define OS_CONSOLE_SIZE_MASK 0x3
#define OS_CONSOLE_SIZE_4MB 0x1

// The arena bounds live in the system work area at the end of main memory.
#define ARENA_LO ((void **) 0x027ffda0)
#define ARENA_HI ((void **) 0x027ffdc4)

#define MAIN_ARENA_LO ((void *) 0x021994a0)
#define MAINEX_ARENA_LO ((void *) 0x023e0000)
#define MAINEX_ARENA_HI ((void *) 0x02700000)
#define MAIN_ARENA_HI ((void *) 0x023e0000)
#define ITCM_ARENA_LO ((void *) 0x01ffa940)
#define ITCM_ARENA_HI ((void *) 0x02000000)
#define DTCM_ARENA_LO ((void *) 0x027e0520)
#define SHARED_ARENA_LO ((void *) 0x027ff000)
#define SHARED_ARENA_HI ((void *) 0x027ff680)
#define WRAM_MAIN_ARENA ((void *) 0x037f8000)

#define IRQ_STACK_END ((u32) SDK_AUTOLOAD_DTCM_START + 0x3f80)
#define IRQ_STACK_SIZE ((u32) SDK_IRQ_STACKSIZE)

// Linker-defined sizes.
extern char SDK_SYS_STACKSIZE[];
extern char SDK_IRQ_STACKSIZE[];

static BOOL OSi_MainExArenaEnabled;
static BOOL OSi_Initialized;

u32 OS_GetConsoleType(void);
void OS_SetProtectionRegion1(u32 param);
void OS_SetProtectionRegion2(u32 param);
void *OS_GetInitArenaHi(OSArenaId id);
void *OS_GetInitArenaLo(OSArenaId id);

void OS_InitArena(void) {
    if (OSi_Initialized) {
        return;
    }
    OSi_Initialized = TRUE;
    OS_SetArenaHi(OS_ARENA_MAIN, OS_GetInitArenaHi(OS_ARENA_MAIN));
    OS_SetArenaLo(OS_ARENA_MAIN, OS_GetInitArenaLo(OS_ARENA_MAIN));
    OS_SetArenaLo(OS_ARENA_MAINEX, NULL);
    OS_SetArenaHi(OS_ARENA_MAINEX, NULL);
    OS_SetArenaHi(OS_ARENA_ITCM, OS_GetInitArenaHi(OS_ARENA_ITCM));
    OS_SetArenaLo(OS_ARENA_ITCM, OS_GetInitArenaLo(OS_ARENA_ITCM));
    OS_SetArenaHi(OS_ARENA_DTCM, OS_GetInitArenaHi(OS_ARENA_DTCM));
    OS_SetArenaLo(OS_ARENA_DTCM, OS_GetInitArenaLo(OS_ARENA_DTCM));
    OS_SetArenaHi(OS_ARENA_SHARED, OS_GetInitArenaHi(OS_ARENA_SHARED));
    OS_SetArenaLo(OS_ARENA_SHARED, OS_GetInitArenaLo(OS_ARENA_SHARED));
    OS_SetArenaHi(OS_ARENA_WRAM_MAIN, OS_GetInitArenaHi(OS_ARENA_WRAM_MAIN));
    OS_SetArenaLo(OS_ARENA_WRAM_MAIN, OS_GetInitArenaLo(OS_ARENA_WRAM_MAIN));
}

void OS_InitArenaEx(void) {
    OS_SetArenaHi(OS_ARENA_MAINEX, OS_GetInitArenaHi(OS_ARENA_MAINEX));
    OS_SetArenaLo(OS_ARENA_MAINEX, OS_GetInitArenaLo(OS_ARENA_MAINEX));
    if (!OSi_MainExArenaEnabled || (OS_GetConsoleType() & OS_CONSOLE_SIZE_MASK) == OS_CONSOLE_SIZE_4MB) {
        OS_SetProtectionRegion1(0x0200002b);
        OS_SetProtectionRegion2(0x023e0021);
    }
}

void *OS_GetArenaHi(OSArenaId id) {
    return ARENA_HI[id];
}

void *OS_GetArenaLo(OSArenaId id) {
    return ARENA_LO[id];
}

void *OS_GetInitArenaHi(OSArenaId id) {
    switch (id) {
    case OS_ARENA_MAIN:
        return MAIN_ARENA_HI;
    case OS_ARENA_MAINEX:
        if (!OSi_MainExArenaEnabled || (OS_GetConsoleType() & OS_CONSOLE_SIZE_MASK) == OS_CONSOLE_SIZE_4MB) {
            return NULL;
        }
        return MAINEX_ARENA_HI;
    case OS_ARENA_ITCM:
        return ITCM_ARENA_HI;
    case OS_ARENA_DTCM: {
        u32 irqStackLo = IRQ_STACK_END - IRQ_STACK_SIZE;
        u32 sysStackLo;
        if ((u32) SDK_SYS_STACKSIZE == 0) {
            sysStackLo = (u32) SDK_AUTOLOAD_DTCM_START;
            if (sysStackLo < (u32) DTCM_ARENA_LO) {
                sysStackLo = (u32) DTCM_ARENA_LO;
            }
        } else if ((s32) SDK_SYS_STACKSIZE < 0) {
            sysStackLo = (u32) DTCM_ARENA_LO - (u32) SDK_SYS_STACKSIZE;
        } else {
            sysStackLo = irqStackLo - (u32) SDK_SYS_STACKSIZE;
        }
        return (void *) sysStackLo;
    }
    case OS_ARENA_SHARED:
        return SHARED_ARENA_HI;
    case OS_ARENA_WRAM_MAIN:
        return WRAM_MAIN_ARENA;
    default:
        return NULL;
    }
}

void *OS_GetInitArenaLo(OSArenaId id) {
    switch (id) {
    case OS_ARENA_MAIN:
        return MAIN_ARENA_LO;
    case OS_ARENA_MAINEX:
        if (!OSi_MainExArenaEnabled || (OS_GetConsoleType() & OS_CONSOLE_SIZE_MASK) == OS_CONSOLE_SIZE_4MB) {
            return NULL;
        }
        return MAINEX_ARENA_LO;
    case OS_ARENA_ITCM:
        return ITCM_ARENA_LO;
    case OS_ARENA_DTCM:
        return DTCM_ARENA_LO;
    case OS_ARENA_SHARED:
        return SHARED_ARENA_LO;
    case OS_ARENA_WRAM_MAIN:
        return WRAM_MAIN_ARENA;
    default:
        return NULL;
    }
}

void OS_SetArenaHi(OSArenaId id, void *newHi) {
    ARENA_HI[id] = newHi;
}

void OS_SetArenaLo(OSArenaId id, void *newLo) {
    ARENA_LO[id] = newLo;
}
