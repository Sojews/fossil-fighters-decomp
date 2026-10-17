// NitroSystem sound: which channels, captures and alarms the library has taken.

#include "nitro/snd.h"
#include "nnsys/snd/main.h"
#include "nnsys/snd/resource_mgr.h"

#define SND_ALARM_NUM 8

static struct {
    u32 lockedCaptures;
    u32 allocatedAlarms;
    u32 lockedChannels;
} sResources;

BOOL NNS_SndLockChannel(u32 chBitFlag) {
    if (chBitFlag == 0) {
        return TRUE;
    }
    if (chBitFlag & sResources.lockedChannels) {
        return FALSE;
    }
    SND_LockChannel(chBitFlag, 0);
    sResources.lockedChannels |= chBitFlag;
    return TRUE;
}

void NNS_SndUnlockChannel(u32 chBitFlag) {
    if (chBitFlag == 0) {
        return;
    }
    SND_UnlockChannel(chBitFlag, 0);
    sResources.lockedChannels &= ~chBitFlag;
}

void NNS_SndUnlockCapture(u32 capBitFlag) {
    sResources.lockedCaptures &= ~capBitFlag;
}

int NNS_SndAllocAlarm(void) {
    int alarmNo;
    u32 bit = 1;
    for (alarmNo = 0; alarmNo < SND_ALARM_NUM; alarmNo++, bit <<= 1) {
        if (!(sResources.allocatedAlarms & bit)) {
            sResources.allocatedAlarms |= bit;
            return alarmNo;
        }
    }
    return -1;
}

void NNS_SndFreeAlarm(int alarmNo) {
    sResources.allocatedAlarms &= ~(1 << alarmNo);
}

void NNSi_SndInitResourceMgr(void) {
    sResources.lockedChannels = 0;
    sResources.lockedCaptures = 0;
    sResources.allocatedAlarms = 0;
}
