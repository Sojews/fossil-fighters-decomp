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

void PM_GoSleepMode(u32 trigger, u32 logic, u16 keyPattern);
u32 PM_GetLCDPower(void);
BOOL PM_SetLCDPower(u32 sw);

#ifdef __cplusplus
}
#endif

#endif
