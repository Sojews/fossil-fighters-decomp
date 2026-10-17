// NitroSDK OS: stopping the processor.

#include "nitro/os.h"

void OS_Halt(void);

void OS_Terminate(void) {
    for (;;) {
        OS_DisableInterrupts();
        OS_Halt();
    }
}

#pragma thumb off
asm void OS_Halt(void) {
    mov r0, #0
    mcr p15, 0, r0, c7, c0, 4
    bx lr
}
#pragma thumb reset
