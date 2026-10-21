// NitroSDK OS: the IRQ handler (in ITCM). It calls the handler for the lowest pending enabled interrupt, then on
// the way out wakes the threads waiting for an interrupt and switches threads if that is needed.

#include "nitro/os.h"
#include "nitro/section.h"

extern OSThreadQueue OSi_IrqThreadQueue;
void CP_SaveContext(void *context);
void CPi_RestoreContext(void *context);
void OS_IrqHandler_ThreadSwitch(void);

#pragma section ITCM begin
#pragma thumb off
asm void OS_IrqHandler(void) {
    stmfd sp!, {lr}
    mov r12, #0x04000000
    add r12, r12, #0x210
    ldr r1, [r12, #-8]
    cmp r1, #0
    ldmeqfd sp!, {pc}
    ldmia r12, {r1-r2}
    ands r1, r1, r2
    ldmeqfd sp!, {pc}
    mov r3, #0x80000000
@1:
    clz r0, r1
    bics r1, r1, r3, lsr r0
    bne @1
    mov r1, r3, lsr r0
    str r1, [r12, #4]
    rsbs r0, r0, #31
    ldr r1, =SDK_AUTOLOAD_DTCM_START
    ldr r0, [r1, r0, lsl #2]
    ldr lr, =OS_IrqHandler_ThreadSwitch
    bx r0
}

asm void OS_IrqHandler_ThreadSwitch(void) {
    ldr r12, =OSi_IrqThreadQueue
    mov r3, #0
    ldr r12, [r12, #0]
    mov r2, #1
    cmp r12, #0
    beq @thread_switch
@1:
    str r2, [r12, #0x64]
    str r3, [r12, #0x78]
    str r3, [r12, #0x7c]
    ldr r0, [r12, #0x80]
    str r3, [r12, #0x80]
    mov r12, r0
    cmp r12, #0
    bne @1
    ldr r12, =OSi_IrqThreadQueue
    str r3, [r12, #0]
    str r3, [r12, #4]
    ldr r12, =OSi_ThreadInfo
    mov r1, #1
    strh r1, [r12, #0]
@thread_switch:
    ldr r12, =OSi_ThreadInfo
    ldrh r1, [r12, #0]
    cmp r1, #0
    ldreq pc, [sp], #4
    mov r1, #0
    strh r1, [r12, #0]
    mov r3, #0xd2
    msr cpsr_c, r3
    add r2, r12, #8
    ldr r1, [r2]
@2:
    cmp r1, #0
    ldrneh r0, [r1, #0x64]
    cmpne r0, #1
    ldrne r1, [r1, #0x68]
    bne @2
    cmp r1, #0
    bne @3
@4:
    mov r3, #0x92
    msr cpsr_c, r3
    ldr pc, [sp], #4
@3:
    ldr r0, [r12, #4]
    cmp r1, r0
    beq @4
    ldr r3, [r12, #0xc]
    cmp r3, #0
    beq @5
    stmfd sp!, {r0, r1, r12}
    mov lr, pc
    bx r3
    ldmfd sp!, {r0, r1, r12}
@5:
    str r1, [r12, #4]
    mrs r2, spsr
    str r2, [r0, #0]!
    stmfd sp!, {r0, r1}
    add r0, r0, #0
    add r0, r0, #0x48
    ldr r1, =CP_SaveContext
    blx r1
    ldmfd sp!, {r0, r1}
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, r12, lr}
    stmib r0!, {r2-r14}^
    stmib r0!, {r14}
    mov r3, #0xd3
    msr cpsr_c, r3
    stmib r0!, {sp}
    stmfd sp!, {r1}
    add r0, r1, #0
    add r0, r0, #0x48
    ldr r1, =CPi_RestoreContext
    blx r1
    ldmfd sp!, {r1}
    ldr sp, [r1, #0x44]
    mov r3, #0xd2
    msr cpsr_c, r3
    ldr r2, [r1, #0]!
    msr spsr_fc, r2
    ldr lr, [r1, #0x40]
    ldmib r1, {r0-r14}^
    nop
    stmda sp!, {r0-r3, r12, lr}
    ldmfd sp!, {pc}
}
#pragma thumb reset
#pragma section ITCM end
