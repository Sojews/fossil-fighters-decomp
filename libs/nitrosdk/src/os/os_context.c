// NitroSDK OS: thread contexts (the registers, the SVC-mode stack pointer and the divider/sqrt unit's state).

#include "nitro/os.h"

#define HW_SVC_STACK_SIZE 0x40
#define OSi_CONTEXT_CP 0x48

void CP_SaveContext(void *context);
void CPi_RestoreContext(const void *context);

#pragma thumb off
asm void OS_InitContext(register OSContext *context, register u32 newpc, register u32 newsp) {
    // pc + 4, so a restore can return with subs pc, lr, #4
    add r1, r1, #4
    str r1, [r0, #64]
    str newsp, [r0, #68]
    // The thread's stack starts below its SVC stack, 8-byte aligned.
    sub r2, r2, #HW_SVC_STACK_SIZE
    tst r2, #4
    subne r2, r2, #4
    str r2, [r0, #56]
    // System mode, Thumb if the entry point is.
    ands r1, r1, #1
    movne r1, #0x3f
    moveq r1, #0x1f
    str r1, [r0]
    mov r1, #0
    str r1, [r0, #4]
    str r1, [r0, #8]
    str r1, [r0, #12]
    str r1, [r0, #16]
    str r1, [r0, #20]
    str r1, [r0, #24]
    str r1, [r0, #28]
    str r1, [r0, #32]
    str r1, [r0, #36]
    str r1, [r0, #40]
    str r1, [r0, #44]
    str r1, [r0, #48]
    str r1, [r0, #52]
    str r1, [r0, #60]
    bx lr
}

asm BOOL OS_SaveContext(register OSContext *context) {
    stmfd sp!, {r0, lr}
    add r0, r0, #OSi_CONTEXT_CP
    ldr r1, =CP_SaveContext
    blx r1
    ldmfd sp!, {r0, lr}
    add r1, r0, #0
    mrs r2, cpsr
    str r2, [r1], #4
    mov r0, #0xd3
    msr cpsr_c, r0
    str sp, [r1, #64]
    msr cpsr_c, r2
    // A restore resumes here with r0 = TRUE.
    mov r0, #1
    stmia r1, {r0-r14}
    add r0, pc, #8
    str r0, [r1, #60]
    mov r0, #0
    bx lr
}

asm void OS_LoadContext(register OSContext *context) {
    stmfd sp!, {r0, lr}
    add r0, r0, #OSi_CONTEXT_CP
    ldr r1, =CPi_RestoreContext
    blx r1
    ldmfd sp!, {r0, lr}
    mrs r1, cpsr
    bic r1, r1, #0x1f
    orr r1, r1, #0xd3
    msr cpsr_c, r1
    ldr r1, [r0], #4
    msr spsr_fsxc, r1
    ldr sp, [r0, #64]
    ldr lr, [r0, #60]
    ldmia r0, {r0-r14}^
    nop
    subs pc, lr, #4
}
#pragma thumb reset
