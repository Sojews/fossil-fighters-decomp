// The parts of the NitroSDK PM (power management) library the game code uses (declarations only).
#ifndef NITRO_PM_H
#define NITRO_PM_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PM_TRIGGER_COVER_OPEN 0x4
#define PM_TRIGGER_CARD 0x8

#define PM_LCD_POWER_OFF 0
#define PM_LCD_POWER_ON 1

typedef void (*PMSleepCallback)(void *arg);

typedef struct PMSleepCallbackInfo PMSleepCallbackInfo;
struct PMSleepCallbackInfo {
    PMSleepCallback callback;
    void *arg;
    PMSleepCallbackInfo *next;
};

static inline void PM_SetSleepCallback(PMSleepCallbackInfo *info, PMSleepCallback callback, void *arg) {
    info->callback = callback;
    info->arg = arg;
}

void PM_PrependPreSleepCallback(PMSleepCallbackInfo *info);
void PM_AppendPostSleepCallback(PMSleepCallbackInfo *info);
void PM_DeletePreSleepCallback(PMSleepCallbackInfo *info);
void PM_DeletePostSleepCallback(PMSleepCallbackInfo *info);
void PM_Init(void);
void PM_GoSleepMode(u32 trigger, u32 logic, u16 keyPattern);
u32 PM_SetAmp(u32 status);
u32 PM_SetAmpGain(u32 gain);
u32 PM_GetLCDPower(void);
BOOL PM_SetLCDPower(u32 sw);

#ifdef __cplusplus
}
#endif

#endif
