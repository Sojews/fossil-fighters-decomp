// Sleep mode: the system sleeps when the lid is closed (while that's allowed), telling the sleep hooks before and
// after.
#ifndef M_SLEEP_H
#define M_SLEEP_H

#include "m/looper.h"

// Told before the system sleeps and after it wakes, while registered.
class MSleepHook {
public:
    MSleepHook();
    virtual ~MSleepHook();
    virtual void OnSleep() = 0;
    virtual void OnWake() = 0;

    void Register();
    void Unregister();

    u8 mRegistered; // 0x04
};

// Watches the lid each frame.
class MFoldSleepHandler : public MLooperVSyncHook {
public:
    MFoldSleepHandler() : MLooperVSyncHook(FALSE, TRUE) {
        mEnabled = FALSE;
        mSleeping = FALSE;
    }
    virtual ~MFoldSleepHandler() {}
    virtual void OnVSync();

    u8 mEnabled;    // 0x09: sleeping is allowed
    u8 mSleeping;   // 0x0A
};

void MInitFoldSleep();
// Allows (or stops allowing) sleep; the calls nest.
void MEnableSleep();
void MDisableSleep();
void MSleep();

#endif
