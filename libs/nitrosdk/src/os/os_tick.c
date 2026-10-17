// NitroSDK OS: the tick counter (timer 0 at system clock / 64, extended to 64 bits by its overflow interrupt).

#include "nitro/os.h"

#define REG_TM0CNT_L (*(volatile u16 *) 0x04000100)
#define REG_TM0CNT_H (*(volatile u16 *) 0x04000102)
#define OS_IE_TIMER0 (1 << 3)

void OSi_SetTimerReserved(int timerNum);
void OSi_EnterTimerCallback(u32 timerNo, void (*callback)(void *), void *arg);

static void OSi_CountUpTick(void);

volatile u64 OSi_TickCounter;
static u16 OSi_UseTick;
static BOOL OSi_NeedResetTimer;

void OS_InitTick(void) {
    if (OSi_UseTick) {
        return;
    }
    OSi_UseTick = TRUE;
    OSi_SetTimerReserved(0);
    OSi_TickCounter = 0;
    REG_TM0CNT_H = 0;
    REG_TM0CNT_L = 0;
    REG_TM0CNT_H = 0xc1;
    OS_SetIrqFunction(OS_IE_TIMER0, OSi_CountUpTick);
    OS_EnableIrqMask(OS_IE_TIMER0);
    OSi_NeedResetTimer = FALSE;
}

static void OSi_CountUpTick(void) {
    OSi_TickCounter++;
    if (OSi_NeedResetTimer) {
        REG_TM0CNT_H = 0;
        REG_TM0CNT_L = 0;
        REG_TM0CNT_H = 0xc1;
        OSi_NeedResetTimer = FALSE;
    }
    OSi_EnterTimerCallback(0, (void (*)(void *)) OSi_CountUpTick, NULL);
}

u16 OS_GetTickLo(void) {
    return REG_TM0CNT_L;
}
