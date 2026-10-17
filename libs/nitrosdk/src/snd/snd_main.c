// NitroSDK SND: setting up the ARM9 side of the sound library.

#include "nitro/os.h"
#include "nitro/snd.h"

static OSMutex sSndMutex;
static BOOL sInitialized;

void SND_Init(void) {
    if (sInitialized) {
        return;
    }
    sInitialized = TRUE;
    OS_InitMutex(&sSndMutex);
    SND_CommandInit();
    SND_AlarmInit();
}

void SNDi_LockMutex(void) {
    OS_LockMutex(&sSndMutex);
}

void SNDi_UnlockMutex(void) {
    OS_UnlockMutex(&sSndMutex);
}
