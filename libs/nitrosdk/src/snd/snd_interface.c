// NitroSDK SND: the calls that queue commands for the ARM7 sound driver.

#include "nitro/snd.h"

enum {
    SND_COMMAND_START_SEQ,
    SND_COMMAND_STOP_SEQ,
    SND_COMMAND_PREPARE_SEQ,
    SND_COMMAND_START_PREPARED_SEQ,
    SND_COMMAND_PAUSE_SEQ,
    SND_COMMAND_SKIP_SEQ,
    SND_COMMAND_PLAYER_PARAM,
    SND_COMMAND_TRACK_PARAM,
    SND_COMMAND_MUTE_TRACK,
    SND_COMMAND_ALLOCATABLE_CHANNEL,
    SND_COMMAND_PLAYER_LOCAL_VAR,
    SND_COMMAND_PLAYER_GLOBAL_VAR,
    SND_COMMAND_START_TIMER,
    SND_COMMAND_STOP_TIMER,
    SND_COMMAND_SETUP_CHANNEL_PCM,
    SND_COMMAND_SETUP_CHANNEL_PSG,
    SND_COMMAND_SETUP_CHANNEL_NOISE,
    SND_COMMAND_SETUP_CAPTURE,
    SND_COMMAND_SETUP_ALARM,
    SND_COMMAND_CHANNEL_TIMER,
    SND_COMMAND_CHANNEL_VOLUME,
    SND_COMMAND_CHANNEL_PAN,
    SND_COMMAND_SURROUND_DECAY,
    SND_COMMAND_MASTER_VOLUME,
    SND_COMMAND_MASTER_PAN,
    SND_COMMAND_OUTPUT_SELECTOR,
    SND_COMMAND_LOCK_CHANNEL,
    SND_COMMAND_UNLOCK_CHANNEL,
    SND_COMMAND_STOP_UNLOCKED_CHANNEL,
    SND_COMMAND_SHARED_WORK,
    SND_COMMAND_INVALIDATE_SEQ,
    SND_COMMAND_INVALIDATE_BANK,
    SND_COMMAND_INVALIDATE_WAVE
};

static void PushCommand_impl(int command, u32 p1, u32 p2, u32 p3, u32 p4);

static inline void PushCommand(int command, u32 p1, u32 p2, u32 p3, u32 p4) {
    PushCommand_impl(command, p1, p2, p3, p4);
}

void SNDi_SetPlayerParam(int playerNo, u32 offset, u32 data, int size);
void SNDi_SetTrackParam(int playerNo, u32 trackBitMask, u32 offset, u32 data, int size);
int SNDi_SetAlarmHandler(int alarmNo, SNDAlarmHandler handler, void *arg);
void SNDi_IncAlarmId(int alarmNo);

void SND_StopSeq(int playerNo) {
    PushCommand(SND_COMMAND_STOP_SEQ, playerNo, 0, 0, 0);
}

void SND_PrepareSeq(int playerNo, const void *seqBase, u32 seqOffset, const void *bank) {
    PushCommand(SND_COMMAND_PREPARE_SEQ, playerNo, (u32) seqBase, seqOffset, (u32) bank);
}

void SND_StartPreparedSeq(int playerNo) {
    PushCommand(SND_COMMAND_START_PREPARED_SEQ, playerNo, 0, 0, 0);
}

void SND_PauseSeq(int playerNo, BOOL flag) {
    PushCommand(SND_COMMAND_PAUSE_SEQ, playerNo, flag, 0, 0);
}

void SND_SetPlayerTempoRatio(int playerNo, int ratio) {
    SNDi_SetPlayerParam(playerNo, 0x1a, ratio, 2);
}

void SND_SetPlayerVolume(int playerNo, int volume) {
    SNDi_SetPlayerParam(playerNo, 6, volume, 2);
}

void SND_SetPlayerChannelPriority(int playerNo, int prio) {
    SNDi_SetPlayerParam(playerNo, 4, prio, 1);
}

void SND_SetPlayerGlobalVariable(int varNo, s16 var) {
    PushCommand(SND_COMMAND_PLAYER_GLOBAL_VAR, varNo, var, 0, 0);
}

void SND_SetTrackVolume(int playerNo, u32 trackBitMask, int volume) {
    SNDi_SetTrackParam(playerNo, trackBitMask, 10, volume, 2);
}

void SND_SetTrackPitch(int playerNo, u32 trackBitMask, int pitch) {
    SNDi_SetTrackParam(playerNo, trackBitMask, 12, pitch, 2);
}

void SND_SetTrackPan(int playerNo, u32 trackBitMask, int pan) {
    SNDi_SetTrackParam(playerNo, trackBitMask, 9, pan, 1);
}

void SND_SetTrackAllocatableChannel(int playerNo, u32 trackBitMask, u32 chBitMask) {
    PushCommand(SND_COMMAND_ALLOCATABLE_CHANNEL, playerNo, trackBitMask, chBitMask, 0);
}

void SND_StartTimer(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags) {
    PushCommand(SND_COMMAND_START_TIMER, chBitMask, capBitMask, alarmBitMask, flags);
}

void SND_StopTimer(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags) {
    int i = 0;
    u32 mask = alarmBitMask;
    while (i < 8 && mask != 0) {
        if (mask & 1) {
            SNDi_IncAlarmId(i);
        }
        i++;
        mask >>= 1;
    }
    PushCommand(SND_COMMAND_STOP_TIMER, chBitMask, capBitMask, alarmBitMask, flags);
}

void SND_SetupAlarm(int alarmNo, u32 tick, u32 period, SNDAlarmHandler handler, void *arg) {
    PushCommand(SND_COMMAND_SETUP_ALARM, alarmNo, tick, period, SNDi_SetAlarmHandler(alarmNo, handler, arg));
}

void SND_SetTrackMute(int playerNo, u32 trackBitMask, BOOL flag) {
    PushCommand(SND_COMMAND_MUTE_TRACK, playerNo, trackBitMask, flag ? 3 : 0, 0);
}

void SND_LockChannel(u32 chBitMask, u32 flags) {
    PushCommand(SND_COMMAND_LOCK_CHANNEL, chBitMask, flags, 0, 0);
}

void SND_UnlockChannel(u32 chBitMask, u32 flags) {
    PushCommand(SND_COMMAND_UNLOCK_CHANNEL, chBitMask, flags, 0, 0);
}

void SND_SetChannelVolume(u32 chBitMask, int volume, int shift) {
    PushCommand(SND_COMMAND_CHANNEL_VOLUME, chBitMask, volume, shift, 0);
}

void SND_SetChannelPan(u32 chBitMask, int pan) {
    PushCommand(SND_COMMAND_CHANNEL_PAN, chBitMask, pan, 0, 0);
}

void SND_SetupChannelPcm(int chNo, int format, const void *dataAddr, int loop, int loopStart, int loopLen,
                         int volume, int shift, int timer, int pan) {
    PushCommand(SND_COMMAND_SETUP_CHANNEL_PCM, chNo | (timer << 16), (u32) dataAddr,
                (volume << 24) | (shift << 22) | loopLen, (loop << 26) | (format << 24) | (pan << 16) | loopStart);
}

void SND_InvalidateSeqData(const void *start, const void *end) {
    PushCommand(SND_COMMAND_INVALIDATE_SEQ, (u32) start, (u32) end, 0, 0);
}

void SND_InvalidateBankData(const void *start, const void *end) {
    PushCommand(SND_COMMAND_INVALIDATE_BANK, (u32) start, (u32) end, 0, 0);
}

void SND_InvalidateWaveData(const void *start, const void *end) {
    PushCommand(SND_COMMAND_INVALIDATE_WAVE, (u32) start, (u32) end, 0, 0);
}

void SND_SetOutputSelector(int leftOutput, int rightOutput, int channel1Output, int channel3Output) {
    PushCommand(SND_COMMAND_OUTPUT_SELECTOR, leftOutput, rightOutput, channel1Output, channel3Output);
}

void SNDi_SetPlayerParam(int playerNo, u32 offset, u32 data, int size) {
    PushCommand(SND_COMMAND_PLAYER_PARAM, playerNo, offset, data, size);
}

void SNDi_SetTrackParam(int playerNo, u32 trackBitMask, u32 offset, u32 data, int size) {
    PushCommand(SND_COMMAND_TRACK_PARAM, playerNo | (size << 24), trackBitMask, offset, data);
}

static void PushCommand_impl(int command, u32 p1, u32 p2, u32 p3, u32 p4) {
    SNDCommand *cmd = SND_AllocCommand(SND_COMMAND_BLOCK);
    if (cmd == NULL) {
        return;
    }
    cmd->id = command;
    cmd->arg[0] = p1;
    cmd->arg[1] = p2;
    cmd->arg[2] = p3;
    cmd->arg[3] = p4;
    SND_PushCommand(cmd);
}
