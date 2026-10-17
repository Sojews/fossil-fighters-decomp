// NitroSDK OS: exclusive locks on the VRAM banks, by lock id.

#include "nitro/os.h"

#define OS_VRAM_BANK_ALL 0x1ff

static u16 OSi_vramLockId[9];
static u32 OSi_vramExclusive;

#pragma thumb off
static asm u32 OsCountZeroBits(u32 bitmap) {
    clz r0, r0
    bx lr
}
#pragma thumb reset

void OSi_InitVramExclusive(void) {
    int i;
    OSi_vramExclusive = 0;
    for (i = 0; i < 9; i++) {
        OSi_vramLockId[i] = 0;
    }
}

BOOL OSi_TryLockVram(u16 bank, u16 lockId) {
    u32 workMap;
    s32 zeroBits;
    OSIntrMode enabled = OS_DisableInterrupts();
    workMap = (u32) (bank & OSi_vramExclusive);
    for (;;) {
        zeroBits = (s32) (31 - OsCountZeroBits(workMap));
        if (zeroBits < 0) {
            break;
        }
        workMap &= ~(1 << zeroBits);
        if (OSi_vramLockId[zeroBits] != lockId) {
            OS_RestoreInterrupts(enabled);
            return FALSE;
        }
    }
    workMap = (u32) (bank & OS_VRAM_BANK_ALL);
    for (;;) {
        zeroBits = (s32) (31 - OsCountZeroBits(workMap));
        if (zeroBits < 0) {
            break;
        }
        workMap &= ~(1 << zeroBits);
        OSi_vramLockId[zeroBits] = lockId;
        OSi_vramExclusive |= (1 << zeroBits);
    }
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

void OSi_UnlockVram(u16 bank, u16 lockId) {
    u32 workMap;
    s32 zeroBits;
    OSIntrMode enabled = OS_DisableInterrupts();
    workMap = (u32) (bank & OSi_vramExclusive & OS_VRAM_BANK_ALL);
    for (;;) {
        zeroBits = (s32) (31 - OsCountZeroBits(workMap));
        if (zeroBits < 0) {
            break;
        }
        workMap &= ~(1 << zeroBits);
        if (OSi_vramLockId[zeroBits] == lockId) {
            OSi_vramLockId[zeroBits] = 0;
            OSi_vramExclusive &= ~(1 << zeroBits);
        }
    }
    OS_RestoreInterrupts(enabled);
}
