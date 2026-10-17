// NitroSDK OS: V-count alarms. Only the setup is left in the game.

#include "nitro/os.h"

#define OS_IE_V_COUNT 0x4

static s32 OSi_PreviousVCount;
static u16 OSi_UseVAlarm;
static s32 OSi_VFrameCount;
static OSThreadQueue OSi_VAlarmQueue;

void OS_InitVAlarm(void) {
    if (OSi_UseVAlarm) {
        return;
    }
    OSi_UseVAlarm = TRUE;
    OSi_VAlarmQueue.head = NULL;
    OSi_VAlarmQueue.tail = NULL;
    OS_DisableIrqMask(OS_IE_V_COUNT);
    OSi_PreviousVCount = 0;
    OSi_VFrameCount = 0;
}
