// NitroSDK OS: which hardware timers the system has claimed.

#include "nitro/os.h"

static u16 OSi_TimerReserved;

void OSi_SetTimerReserved(int timerNum) {
    OSi_TimerReserved |= (1 << timerNum);
}
