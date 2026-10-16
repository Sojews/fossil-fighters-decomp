// A fixed-capacity stack of ints in caller-provided memory. Built as ARM code.

#pragma thumb off

#include "sound.h"

IntStack::IntStack() {
    initialized = FALSE;
    buf = NULL;
    count = 0;
    capacity = 0;
}

void IntStack::Init(int *b, int cap) {
    buf = b;
    count = 0;
    capacity = cap;
    initialized = TRUE;
}

#ifdef NONMATCHING
// Equivalent, but mwcc swaps the registers of i and n.
void IntStack::Truncate(int n) {
    int i;

    for (i = n; i < count; i++) {
        buf[i] = -1;
    }
    count = n;
    OnChange();
}
#else
asm void IntStack::Truncate(int n) {
    nofralloc
    stmdb sp!, {r3, lr}
    ldr r2, [r0, #0x8]
    mov ip, r1
    cmp r1, r2
    bge L_02004d24
    mvn r3, #0x0
L_02004d0c:
    ldr r2, [r0, #0x4]
    str r3, [r2, ip, lsl #0x2]
    ldr r2, [r0, #0x8]
    add ip, ip, #0x1
    cmp ip, r2
    blt L_02004d0c
L_02004d24:
    str r1, [r0, #0x8]
    bl OnChange
    ldmia sp!, {r3, pc}
}
#endif

void IntStack::Push(int value) {
    buf[count] = value;
    count++;
    OnChange();
}

int IntStack::Get(int index) {
    return buf[index];
}

int IntStack::Count() {
    return count;
}

void IntStack::OnChange() {
}
