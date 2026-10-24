// Soft reset.

#include "m/soft_reset.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/irq.h"
#include "m/lcd.h"

#define HW_RESET_PARAMETER (*(u32 *) 0x027FFC20)
#define SOFT_RESET_BUTTONS 0x30C   // L, R, START and SELECT

static u32 sResetParameter;
static int sSoftResetEnableCount;
static MSoftResetHandler *sSoftResetHandler;

void MInitSoftReset() {
    sSoftResetHandler = new MSoftResetHandler();
    sResetParameter = HW_RESET_PARAMETER;
}

u32 MGetResetParameter() {
    return sResetParameter;
}

void MClearResetParameter() {
    sResetParameter = 0;
}

void MEnableSoftReset() {
    MIrqLock lock;
    if (++sSoftResetEnableCount == 1) {
        sSoftResetHandler->mEnabled = TRUE;
    }
}

void MDisableSoftReset() {
    MIrqLock lock;
    if (--sSoftResetEnableCount == 0) {
        sSoftResetHandler->mEnabled = FALSE;
    }
}

void MSoftReset(u32 parameter) {
    gLcd->mMain->mBright->SetMode(1);
    gLcd->mMain->mBright->SetFactor(16);
    gLcd->mMain->mBright->Update();
    gLcd->mSub->mBright->SetMode(1);
    gLcd->mSub->mBright->SetFactor(16);
    gLcd->mSub->mBright->Update();
    OS_ResetSystem(parameter);
}

void MSoftResetHandler::OnVSync() {
    if (mEnabled) {
        MInput *input = mInput.GetInput(0);
        s32 buttons = input->mButtons;
        s32 pressed = buttons & (input->mPrevButtons ^ buttons);
        BOOL reset = FALSE;
        if ((pressed & SOFT_RESET_BUTTONS) && (buttons & SOFT_RESET_BUTTONS) == SOFT_RESET_BUTTONS) {
            reset = TRUE;
        }
        if (reset) {
            MSoftReset(0);
        }
    }
}
