// NitroSystem sound: which channels, captures and alarms the library has taken.
#ifndef NNSYS_SND_RESOURCE_MGR_H
#define NNSYS_SND_RESOURCE_MGR_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL NNS_SndLockChannel(u32 chBitFlag);
void NNS_SndUnlockChannel(u32 chBitFlag);
void NNS_SndUnlockCapture(u32 capBitFlag);
int NNS_SndAllocAlarm(void);
void NNS_SndFreeAlarm(int alarmNo);

#ifdef __cplusplus
}
#endif

#endif
