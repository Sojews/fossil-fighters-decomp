// MSL console I/O over ARM semihosting (SWI 0xAB).

#include "types.h"

// SYS_WRITEC: write the character at *c to the debug console.
asm void func_02086cfc(const char *c) {
    push {lr}
    add r1, r0, #0
    mov r0, #3
    swi 0xab
    pop {pc}
}

// SYS_READC: read one character from the debug console.
asm int func_02086d08(void) {
    push {lr}
    mov r1, #0
    mov r0, #7
    swi 0xab
    pop {pc}
}

// SYS_EXIT
asm void func_02086d14(void) {
    mov r1, #0
    mov r0, #0x18
    swi 0xab
    mov pc, lr
}

int __read_console(u32 handle, u8 *buffer, u32 *count, void *idle_proc) {
    u32 n = *count;
    u32 i;

    for (i = 0; i < n; i++) {
        buffer[i] = func_02086d08();
        if (buffer[i] == '\r' || buffer[i] == '\n') {
            *count = i + 1;
            break;
        }
    }
    return 0;
}

int __write_console(u32 handle, u8 *buffer, u32 *count, void *idle_proc) {
    u32 i;
    u32 n = *count;

    for (i = 0; i < n; i++) {
        func_02086cfc((const char *)&buffer[i]);
    }
    return 0;
}

int __close_console(u32 handle) {
    return 0;
}
