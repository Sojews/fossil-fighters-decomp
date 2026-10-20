// Sleep mode.

#pragma ipa file
#include <algorithm>
#include <list>
#include "m/sleep.h"
#include "m/heap.h"
#include "m/irq.h"
#include "m/task.h"
#include "nitro/pm.h"

#define HW_BOOT_TYPE (*(volatile u16 *) 0x027FFC40)
#define HW_BUTTON_XY (*(volatile u16 *) 0x027FFFA8)
#define BOOT_TYPE_DOWNLOAD 2

// static_order: begin
static int sSleepEnableCount;
static OSMutex sSleepHooksMutex;
static MFoldSleepHandler *sFoldSleepHandler;
static std::list<MSleepHook *> sSleepHooks;
// static_order: end

// Download Play children have no card to wake on.
static inline BOOL IsDownloadPlayChild() {
    return HW_BOOT_TYPE == BOOT_TYPE_DOWNLOAD;
}

static inline BOOL IsFolded() {
    return (HW_BUTTON_XY & 0x8000) >> 15;
}

void MInitFoldSleep() {
    sFoldSleepHandler = new MFoldSleepHandler();
}

void MEnableSleep() {
    MIrqLock lock;
    if (++sSleepEnableCount == 1) {
        sFoldSleepHandler->mEnabled = TRUE;
    }
}

void MDisableSleep() {
    MIrqLock lock;
    if (--sSleepEnableCount == 0) {
        sFoldSleepHandler->mEnabled = FALSE;
    }
}

void MSleep() {
    MScopedLock lock(&sSleepHooksMutex, TRUE);
    for (std::list<MSleepHook *>::iterator it = sSleepHooks.begin(); it != sSleepHooks.end(); it++) {
        (*it)->OnSleep();
    }
    if (!IsDownloadPlayChild()) {
        PM_GoSleepMode(PM_TRIGGER_COVER_OPEN | PM_TRIGGER_CARD, 0, 0);
    } else {
        PM_GoSleepMode(PM_TRIGGER_COVER_OPEN, 0, 0);
    }
    for (std::list<MSleepHook *>::iterator it = sSleepHooks.begin(); it != sSleepHooks.end(); it++) {
        (*it)->OnWake();
    }
}

MSleepHook::MSleepHook() {
    mRegistered = FALSE;
}

MSleepHook::~MSleepHook() {
    // A hook must be unregistered before it's deleted.
    if (mRegistered) {
        OS_Terminate();
    }
}

void MSleepHook::Register() {
    if (!mRegistered) {
        mRegistered = TRUE;
        MScopedLock lock(&sSleepHooksMutex, TRUE);
        sSleepHooks.push_back(this);
    }
}

void MSleepHook::Unregister() {
    if (mRegistered) {
        mRegistered = FALSE;
        OSMutex *mutex = &sSleepHooksMutex;
        OS_LockMutex(mutex);
        sSleepHooks.erase(std::find(sSleepHooks.begin(), sSleepHooks.end(), this));
        OS_UnlockMutex(mutex);
    }
}

void MFoldSleepHandler::OnVSync() {
    bool folded = IsFolded();
    bool lcdOn = PM_GetLCDPower() == PM_LCD_POWER_ON;
    if (!lcdOn && !folded) {
        PM_SetLCDPower(PM_LCD_POWER_ON);
    }
    if (!mEnabled) {
        if (lcdOn && folded) {
            PM_SetLCDPower(PM_LCD_POWER_OFF);
        }
        return;
    }
    mSleeping = FALSE;
    if (folded) {
        mSleeping = TRUE;
        MSleep();
    }
}
