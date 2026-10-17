// NitroSDK OS: interrupt handlers, masks and the per-DMA/timer callbacks.

#include "nitro/os.h"

#define REG_IME (*(volatile u16 *) 0x04000208)
#define REG_IE (*(volatile u32 *) 0x04000210)
#define REG_IF (*(volatile u32 *) 0x04000214)

// The handler table is at the start of DTCM; the IRQ stack ends below the check flags at its end.
#define OS_IRQTable ((OSIrqFunction *) SDK_AUTOLOAD_DTCM_START)
#define IRQ_STACK_END ((u32) SDK_AUTOLOAD_DTCM_START + 0x3f80)
#define IRQ_STACK_SIZE 0x800
#define IRQ_STACK_CHECKNUM_BOTTOM 0xfddb597d
#define IRQ_STACK_CHECKNUM_TOP 0x7bf9dd5b
#define IRQ_STACK_CHECKNUM_WARN 0x597dfbd9

typedef struct {
    void (*func)(void *);
    u32 enable;
    void *arg;
} OSIrqCallbackInfo;

extern OSThreadQueue OSi_IrqThreadQueue;
extern const u16 OSi_IrqCallbackInfoIndex[8];

static u32 OSi_IrqStackWarningOffset;
static OSIrqCallbackInfo OSi_IrqCallbackInfo[8];     // DMA 0-3, then timers 0-3

static inline u16 OS_DisableIrq(void) {
    u16 prev = REG_IME;
    REG_IME = 0;
    return prev;
}

static inline u16 OS_RestoreIrq(u16 enable) {
    u16 prev = REG_IME;
    REG_IME = enable;
    return prev;
}

#pragma thumb off
void OS_WaitIrq(BOOL clear, u32 irqFlags) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (clear) {
        OS_ClearIrqCheckFlag(irqFlags);
    }
    OS_RestoreInterrupts(enabled);
    while (!(OS_GetIrqCheckFlag() & irqFlags)) {
        OS_SleepThread(&OSi_IrqThreadQueue);
    }
}
#pragma thumb reset

void OS_IrqDummy(void) {
}

static void OSi_IrqCallback(int index) {
    u32 mask = 1 << OSi_IrqCallbackInfoIndex[index];
    void (*callback)(void *) = OSi_IrqCallbackInfo[index].func;
    OSi_IrqCallbackInfo[index].func = NULL;
    if (callback != NULL) {
        callback(OSi_IrqCallbackInfo[index].arg);
    }
    OS_SetIrqCheckFlag(mask);
    if (!OSi_IrqCallbackInfo[index].enable) {
        OS_DisableIrqMask(mask);
    }
}

void OSi_IrqDma0(void) {
    OSi_IrqCallback(0);
}

void OSi_IrqDma1(void) {
    OSi_IrqCallback(1);
}

void OSi_IrqDma2(void) {
    OSi_IrqCallback(2);
}

void OSi_IrqDma3(void) {
    OSi_IrqCallback(3);
}

void OSi_IrqTimer0(void) {
    OSi_IrqCallback(4);
}

void OSi_IrqTimer1(void) {
    OSi_IrqCallback(5);
}

void OSi_IrqTimer2(void) {
    OSi_IrqCallback(6);
}

void OSi_IrqTimer3(void) {
    OSi_IrqCallback(7);
}

void OS_InitIrqTable(void) {
    OSi_IrqThreadQueue.head = OSi_IrqThreadQueue.tail = NULL;
}

void OS_SetIrqFunction(u32 intrBit, OSIrqFunction function) {
    int i;
    for (i = 0; i < 22; i++) {
        if (intrBit & 1) {
            OSIrqCallbackInfo *info = NULL;
            if (8 <= i && i <= 11) {
                info = &OSi_IrqCallbackInfo[i - 8];
            } else if (3 <= i && i <= 6) {
                info = &OSi_IrqCallbackInfo[i - 3 + 4];
            } else {
                OS_IRQTable[i] = function;
            }
            if (info != NULL) {
                info->func = (void (*)(void *)) function;
                info->arg = NULL;
                info->enable = TRUE;
            }
        }
        intrBit >>= 1;
    }
}

void OSi_EnterDmaCallback(u32 dmaNo, void (*callback)(void *), void *arg) {
    u32 mask = 1 << (dmaNo + 8);
    OSi_IrqCallbackInfo[dmaNo].func = callback;
    OSi_IrqCallbackInfo[dmaNo].arg = arg;
    OSi_IrqCallbackInfo[dmaNo].enable = OS_EnableIrqMask(mask) & mask;
}

void OSi_EnterTimerCallback(u32 timerNo, void (*callback)(void *), void *arg) {
    OSi_IrqCallbackInfo[timerNo + 4].func = callback;
    OSi_IrqCallbackInfo[timerNo + 4].arg = arg;
    OS_EnableIrqMask(1 << (timerNo + 3));
    OSi_IrqCallbackInfo[timerNo + 4].enable = TRUE;
}

u32 OS_SetIrqMask(u32 intr) {
    u16 ime = OS_DisableIrq();
    u32 prev = REG_IE;
    REG_IE = intr;
    OS_RestoreIrq(ime);
    return prev;
}

u32 OS_EnableIrqMask(u32 intr) {
    u16 ime = OS_DisableIrq();
    u32 prev = REG_IE;
    REG_IE = prev | intr;
    OS_RestoreIrq(ime);
    return prev;
}

u32 OS_DisableIrqMask(u32 intr) {
    u16 ime = OS_DisableIrq();
    u32 prev = REG_IE;
    REG_IE = prev & ~intr;
    OS_RestoreIrq(ime);
    return prev;
}

u32 OS_ResetRequestIrqMask(u32 intr) {
    u16 ime = OS_DisableIrq();
    u32 prev = REG_IF;
    REG_IF = intr;
    OS_RestoreIrq(ime);
    return prev;
}

void OS_SetIrqStackChecker(void) {
    *(u32 *) (IRQ_STACK_END - sizeof(u32)) = IRQ_STACK_CHECKNUM_BOTTOM;
    *(u32 *) (IRQ_STACK_END - IRQ_STACK_SIZE) = IRQ_STACK_CHECKNUM_TOP;
}

void OS_SetIrqStackWarningOffset(u32 offset) {
    OSi_IrqStackWarningOffset = offset;
    if (offset != 0) {
        *(u32 *) (IRQ_STACK_END - IRQ_STACK_SIZE + offset) = IRQ_STACK_CHECKNUM_WARN;
    }
}
