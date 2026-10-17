// NitroSDK OS: where the data TCM is mapped.

#include "nitro/os.h"

#define OS_TCM_REGION_BASE_MASK 0xfffff000

#pragma thumb off
asm u32 OS_GetDTCMAddress(void) {
    mrc p15, 0, r0, c9, c1, 0
    ldr r1, =OS_TCM_REGION_BASE_MASK
    and r0, r0, r1
    bx lr
}
#pragma thumb reset
