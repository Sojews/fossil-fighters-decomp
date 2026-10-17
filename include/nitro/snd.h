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

typedef void (*SNDAlarmHandler)(void *arg);

typedef struct SNDCommand SNDCommand;
struct SNDCommand {
    SNDCommand *next;
    int id;
    u32 arg[4];
};

typedef struct {
    s16 localVar[16];
    u32 tickCounter;
} SNDPlayerWork;

typedef struct SNDSharedWork {
    u32 finishCommandTag;
    u32 playerStatus;
    u16 channelStatus;
    u16 captureStatus;
    u32 padding[5];
    SNDPlayerWork player[16];
    s16 globalVar[16];
} SNDSharedWork;
extern SNDSharedWork *SNDi_SharedWork;

int SNDi_SetAlarmHandler(int alarmNo, SNDAlarmHandler handler, void *arg);
void SNDi_IncAlarmId(int alarmNo);

void SND_CommandInit(void);
void SND_AlarmInit(void);
void SNDi_InitSharedWork(SNDSharedWork *work);
u32 SNDi_GetFinishedCommandTag(void);
void SNDi_CallAlarmHandler(int alarmNo);
void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);
int SND_CountFreeCommand(void);
int SND_CountReservedCommand(void);
int SND_CountWaitingCommand(void);
SNDCommand *SND_AllocCommand(u32 flags);
void SND_PushCommand(SNDCommand *command);

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
void SND_StartTimer(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags);
void SND_SetupChannelPcm(int chNo, int format, const void *dataAddr, int loop, int loopStart, int loopLen,
                         int volume, int shift, int timer, int pan);
void SND_SetupAlarm(int alarmNo, u32 tick, u32 period, SNDAlarmHandler handler, void *arg);
int SND_CalcChannelVolume(int dB);
void SND_SetChannelVolume(u32 chBitMask, int volume, int shift);
void SND_SetChannelPan(u32 chBitMask, int pan);
void SND_SetOutputSelector(int leftOutput, int rightOutput, int channel1Output, int channel3Output);
typedef struct SNDBankData SNDBankData;
typedef struct SNDWaveData SNDWaveData;

typedef struct {
    u32 signature;
    u16 byteOrder;
    u16 version;
    u32 fileSize;
    u16 headerSize;
    u16 dataBlocks;
} SNDBinaryFileHeader;

typedef struct SNDWaveArc {
    SNDBinaryFileHeader fileHeader;
    u32 blockKind;
    u32 blockSize;
    void *topLink;
    u32 reserved[7];
    u32 waveCount;
    u32 waveOffset[1];
} SNDWaveArc;

typedef struct {
    u32 prgNo;
    u32 index;
} SNDInstPos;

typedef struct {
    u8 type;
    u8 padding;
    u16 wave[2];        // Wave index, wave archive number
    u8 originalKey;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 pan;
} SNDInstData;

void SND_AssignWaveArc(SNDBankData *bank, int index, SNDWaveArc *waveArc);
void SND_DestroyBank(SNDBankData *bank);
void SND_DestroyWaveArc(SNDWaveArc *waveArc);
void SND_InvalidateSeqData(const void *start, const void *end);
void SND_InvalidateBankData(const void *start, const void *end);
void SND_InvalidateWaveData(const void *start, const void *end);
const SNDWaveData *SND_GetWaveDataAddress(const SNDWaveArc *waveArc, int index);
void SND_SetWaveDataAddress(SNDWaveArc *waveArc, int index, const SNDWaveData *waveData);
u32 SND_GetWaveDataCount(const SNDWaveArc *waveArc);
SNDInstPos SND_GetFirstInstDataPos(const SNDBankData *bank);
BOOL SND_GetNextInstData(const SNDBankData *bank, SNDInstData *inst, SNDInstPos *pos);
void SND_LockChannel(u32 chBitMask, u32 flags);
void SND_UnlockChannel(u32 chBitMask, u32 flags);

#ifdef __cplusplus
}
#endif

#endif
