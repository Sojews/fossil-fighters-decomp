// The parts of the NitroSDK SND (sound) library NitroSystem uses (declarations only).
#ifndef NITRO_SND_H
#define NITRO_SND_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SND_COMMAND_NOBLOCK 0
#define SND_COMMAND_BLOCK 1
#define SND_COMMAND_IMMEDIATE 2

typedef struct SNDCommand SNDCommand;

void SND_Init(void);
const SNDCommand *SND_RecvCommandReply(u32 flags);
BOOL SND_FlushCommand(u32 flags);
u32 SND_GetCurrentCommandTag(void);
void SND_WaitForCommandProc(u32 tag);
void SND_StopTimer(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags);
void SND_LockChannel(u32 chBitMask, u32 flags);
void SND_UnlockChannel(u32 chBitMask, u32 flags);

#ifdef __cplusplus
}
#endif

#endif
