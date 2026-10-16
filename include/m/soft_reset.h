// Soft reset: L+R+START+SELECT restarts the game (while that's allowed).
#ifndef M_SOFT_RESET_H
#define M_SOFT_RESET_H

#include "m/input.h"

class MSoftResetHandler : public MLooperVSyncHook {
public:
    MSoftResetHandler() : MLooperVSyncHook(FALSE, TRUE), mInput(1) {
        mEnabled = FALSE;
    }
    virtual ~MSoftResetHandler() {}
    virtual void OnVSync();

    MSingleInputHub mInput; // 0x0C
    u8 mEnabled;            // 0x34
};

void MInitSoftReset();
// The parameter the last reset passed to the next run (0 after a power on).
u32 MGetResetParameter();
void MClearResetParameter();
// Allows (or stops allowing) soft resets; the calls nest.
void MEnableSoftReset();
void MDisableSoftReset();
// Fades both screens to white and restarts.
void MSoftReset(u32 parameter);

#endif
