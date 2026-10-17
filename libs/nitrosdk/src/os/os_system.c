// NitroSDK OS: the CPU's interrupt-disable bits, its mode, and busy waiting.

#include "nitro/os.h"

#define HW_PSR_IRQ_DISABLE 0x80
#define HW_PSR_IRQ_FIQ_DISABLE 0xc0
#define HW_PSR_CPU_MODE_MASK 0x1f

void WaitByLoop(s32 count);
void OS_WaitIrq(BOOL clear, u32 irqFlags);

#pragma thumb off
asm OSIntrMode OS_EnableInterrupts(void) {
    mrs r0, cpsr
    bic r1, r0, #HW_PSR_IRQ_DISABLE
    msr cpsr_c, r1
    and r0, r0, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm OSIntrMode OS_DisableInterrupts(void) {
    mrs r0, cpsr
    orr r1, r0, #HW_PSR_IRQ_DISABLE
    msr cpsr_c, r1
    and r0, r0, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm OSIntrMode OS_RestoreInterrupts(register OSIntrMode state) {
    mrs r1, cpsr
    bic r2, r1, #HW_PSR_IRQ_DISABLE
    orr r2, r2, r0
    msr cpsr_c, r2
    and r0, r1, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm OSIntrMode OS_DisableInterrupts_IrqAndFiq(void) {
    mrs r0, cpsr
    orr r1, r0, #HW_PSR_IRQ_FIQ_DISABLE
    msr cpsr_c, r1
    and r0, r0, #HW_PSR_IRQ_FIQ_DISABLE
    bx lr
}

asm OSIntrMode OS_RestoreInterrupts_IrqAndFiq(register OSIntrMode state) {
    mrs r1, cpsr
    bic r2, r1, #HW_PSR_IRQ_FIQ_DISABLE
    orr r2, r2, r0
    msr cpsr_c, r2
    and r0, r1, #HW_PSR_IRQ_FIQ_DISABLE
    bx lr
}

asm OSIntrMode OS_GetCpsrIrq(void) {
    mrs r0, cpsr
    and r0, r0, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm u32 OS_GetProcMode(void) {
    mrs r0, cpsr
    and r0, r0, #HW_PSR_CPU_MODE_MASK
    bx lr
}

asm void OS_SpinWait(register u32 cycle) {
@loop:
    subs r0, r0, #4
    bcs @loop
    bx lr
}
#pragma thumb reset

void OS_WaitVBlankIntr(void) {
    WaitByLoop(1);
    OS_WaitIrq(TRUE, OS_IE_V_BLANK);
}
