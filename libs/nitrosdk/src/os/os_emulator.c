// NitroSDK OS: which console this is. A retail build always answers "retail Nitro".

#include "nitro/os.h"

#define OS_CONSOLE_NITRO 0x80000000
#define OS_CONSOLE_DEV_CARD 0x02000000
#define OS_CONSOLE_SIZE_4MB 0x00000001

static u32 OSi_ConsoleTypeCache = 0xffffffff;

BOOL OS_IsRunOnEmulator(void) {
    return FALSE;
}

u32 OS_GetConsoleType(void) {
    OSi_ConsoleTypeCache = OS_CONSOLE_NITRO | OS_CONSOLE_DEV_CARD | OS_CONSOLE_SIZE_4MB;
    return OSi_ConsoleTypeCache;
}
