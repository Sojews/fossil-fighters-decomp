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
extern const s16 SNDi_DecibelTable[128];

u32 SND_GetPlayerStatus(void);
BOOL SND_IsFinishedCommandTag(u32 tag);
void SND_PrepareSeq(int playerNo, const void *seqBase, u32 seqOffset, const void *bank);
void SND_StartPreparedSeq(int playerNo);
void SND_StopSeq(int playerNo);
void SND_PauseSeq(int playerNo, BOOL flag);
void SND_SetPlayerVolume(int playerNo, int volume);
void SND_SetPlayerChannelPriority(int playerNo, int prio);
void SND_SetPlayerTempoRatio(int playerNo, int ratio);
void SND_SetPlayerGlobalVariable(int varNo, s16 var);
void SND_SetTrackMute(int playerNo, u32 trackBitMask, BOOL flag);
void SND_SetTrackVolume(int playerNo, u32 trackBitMask, int volume);
void SND_SetTrackPitch(int playerNo, u32 trackBitMask, int pitch);
void SND_SetTrackPan(int playerNo, u32 trackBitMask, int pan);
void SND_SetTrackAllocatableChannel(int playerNo, u32 trackBitMask, u32 chBitMask);
void SND_LockChannel(u32 chBitMask, u32 flags);
void SND_UnlockChannel(u32 chBitMask, u32 flags);

#ifdef __cplusplus
}
#endif

#endif
