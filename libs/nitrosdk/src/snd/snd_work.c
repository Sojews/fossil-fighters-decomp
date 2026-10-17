// NitroSDK SND: the work area shared with the ARM7 driver (status, tags and sequence variables).

#include "nitro/mi.h"
#include "nitro/snd.h"

SNDSharedWork *SNDi_SharedWork;

void DC_InvalidateRange(const void *startAddr, u32 nBytes);

u32 SND_GetPlayerStatus(void) {
    DC_InvalidateRange(&SNDi_SharedWork->playerStatus, sizeof(u32));
    return SNDi_SharedWork->playerStatus;
}

u32 SNDi_GetFinishedCommandTag(void) {
    DC_InvalidateRange(&SNDi_SharedWork->finishCommandTag, sizeof(u32));
    return SNDi_SharedWork->finishCommandTag;
}

void SNDi_InitSharedWork(SNDSharedWork *work) {
    int i;
    int j;
    work->playerStatus = 0;
    work->channelStatus = 0;
    work->captureStatus = 0;
    work->finishCommandTag = 0;
    for (i = 0; i < 16; i++) {
        work->player[i].tickCounter = 0;
        for (j = 0; j < 16; j++) {
            work->player[i].localVar[j] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        work->globalVar[i] = -1;
    }
    DC_FlushRange(work, sizeof(SNDSharedWork));
}
