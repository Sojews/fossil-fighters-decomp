// NitroSDK SND: the command queue shared with the ARM7 sound driver. Commands are batched in a reserve list
// and sent over the PXI FIFO; the driver hands each batch back when it has run it.

#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "nitro/snd.h"

#define SND_COMMAND_NUM 256
#define SND_PXI_FIFO_MESSAGE_BUFSIZE 8
#define SND_COMMAND_SHARED_WORK 0x1d

// (With -ipa file a file's statics are laid out in declaration order with the first one moved to the end.)
static SNDCommand sCommandArray[SND_COMMAND_NUM];
static SNDCommand *sReserveListEnd;
static SNDCommand *sFreeListEnd;
static SNDCommand *sReserveList;
static SNDCommand *sFreeList;
static u32 sFinishCommandTag;
static int sCommandMesgReadIdx;
static int sCommandMesgWriteIdx;
static int sCommandMesgCount;
static u32 sCurrentTag;
static SNDCommand *sCommandMesgBuffer[SND_PXI_FIFO_MESSAGE_BUFSIZE];
static SNDSharedWork sSharedWork __attribute__((aligned(32)));

static void InitPXI(void);
static void RequestCommandProc(void);
static SNDCommand *AllocCommand(void);
static BOOL IsCommandAvailable(void);

void SND_CommandInit(void) {
    SNDCommand *command;
    int i;
    InitPXI();
    sFreeList = &sCommandArray[0];
    for (i = 0; i < SND_COMMAND_NUM - 1; i++) {
        sCommandArray[i].next = &sCommandArray[i + 1];
    }
    sCommandArray[SND_COMMAND_NUM - 1].next = NULL;
    sFreeListEnd = &sCommandArray[SND_COMMAND_NUM - 1];
    sReserveList = NULL;
    sReserveListEnd = NULL;
    sCommandMesgCount = 0;
    sCommandMesgReadIdx = 0;
    sCommandMesgWriteIdx = 0;
    sCurrentTag = 1;
    sFinishCommandTag = 0;
    SNDi_SharedWork = &sSharedWork;
    SNDi_InitSharedWork(SNDi_SharedWork);
    command = SND_AllocCommand(SND_COMMAND_BLOCK);
    if (command != NULL) {
        command->id = SND_COMMAND_SHARED_WORK;
        command->arg[0] = (u32) SNDi_SharedWork;
        SND_PushCommand(command);
        SND_FlushCommand(SND_COMMAND_BLOCK);
    }
}

const SNDCommand *SND_RecvCommandReply(u32 flags) {
    OSIntrMode enabled = OS_DisableInterrupts();
    SNDCommand *commandList;
    SNDCommand *commandListEnd;
    if (flags & SND_COMMAND_BLOCK) {
        u32 tag = SNDi_GetFinishedCommandTag();
        while (sFinishCommandTag == tag) {
            OS_RestoreInterrupts(enabled);
            OS_SpinWait(100);
            enabled = OS_DisableInterrupts();
            tag = SNDi_GetFinishedCommandTag();
        }
    } else {
        if (sFinishCommandTag == SNDi_GetFinishedCommandTag()) {
            OS_RestoreInterrupts(enabled);
            return NULL;
        }
    }
    commandList = sCommandMesgBuffer[sCommandMesgReadIdx];
    if (++sCommandMesgReadIdx > SND_PXI_FIFO_MESSAGE_BUFSIZE) {
        sCommandMesgReadIdx = 0;
    }
    commandListEnd = commandList;
    while (commandListEnd->next != NULL) {
        commandListEnd = commandListEnd->next;
    }
    if (sFreeListEnd != NULL) {
        sFreeListEnd->next = commandList;
    } else {
        sFreeList = commandList;
    }
    sFreeListEnd = commandListEnd;
    sCommandMesgCount--;
    sFinishCommandTag++;
    OS_RestoreInterrupts(enabled);
    return commandList;
}

SNDCommand *SND_AllocCommand(u32 flags) {
    SNDCommand *command;
    if (!IsCommandAvailable()) {
        return NULL;
    }
    command = AllocCommand();
    if (command != NULL) {
        return command;
    }
    if (!(flags & SND_COMMAND_BLOCK)) {
        return NULL;
    }
    if (SND_CountWaitingCommand() > 0) {
        while (SND_RecvCommandReply(SND_COMMAND_NOBLOCK) != NULL) {
        }
        command = AllocCommand();
        if (command != NULL) {
            return command;
        }
    } else {
        SND_FlushCommand(SND_COMMAND_BLOCK);
    }
    RequestCommandProc();
    do {
        SND_RecvCommandReply(SND_COMMAND_BLOCK);
        command = AllocCommand();
    } while (command == NULL);
    return command;
}

void SND_PushCommand(SNDCommand *command) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (sReserveListEnd == NULL) {
        sReserveList = command;
        sReserveListEnd = command;
    } else {
        sReserveListEnd->next = command;
        sReserveListEnd = command;
    }
    command->next = NULL;
    OS_RestoreInterrupts(enabled);
}

BOOL SND_FlushCommand(u32 flags) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (sReserveList == NULL) {
        OS_RestoreInterrupts(enabled);
        return TRUE;
    }
    if (sCommandMesgCount >= SND_PXI_FIFO_MESSAGE_BUFSIZE) {
        if (!(flags & SND_COMMAND_BLOCK)) {
            OS_RestoreInterrupts(enabled);
            return FALSE;
        }
        do {
            SND_RecvCommandReply(SND_COMMAND_BLOCK);
        } while (sCommandMesgCount >= SND_PXI_FIFO_MESSAGE_BUFSIZE);
        if (sReserveList == NULL) {
            OS_RestoreInterrupts(enabled);
            return TRUE;
        }
    }
    DC_FlushRange(sCommandArray, sizeof(sCommandArray));
    if (PXI_SendWordByFifo(PXI_FIFO_TAG_SOUND, (u32) sReserveList, FALSE) < 0) {
        if (!(flags & SND_COMMAND_BLOCK)) {
            OS_RestoreInterrupts(enabled);
            return FALSE;
        }
        while (sCommandMesgCount >= SND_PXI_FIFO_MESSAGE_BUFSIZE ||
               PXI_SendWordByFifo(PXI_FIFO_TAG_SOUND, (u32) sReserveList, FALSE) < 0) {
            OS_RestoreInterrupts(enabled);
            SND_RecvCommandReply(SND_COMMAND_NOBLOCK);
            enabled = OS_DisableInterrupts();
            DC_FlushRange(sCommandArray, sizeof(sCommandArray));
            if (sReserveList == NULL) {
                OS_RestoreInterrupts(enabled);
                return TRUE;
            }
        }
    }
    sCommandMesgBuffer[sCommandMesgWriteIdx] = sReserveList;
    if (++sCommandMesgWriteIdx > SND_PXI_FIFO_MESSAGE_BUFSIZE) {
        sCommandMesgWriteIdx = 0;
    }
    sReserveList = NULL;
    sReserveListEnd = NULL;
    sCommandMesgCount++;
    sCurrentTag++;
    OS_RestoreInterrupts(enabled);
    if (flags & SND_COMMAND_IMMEDIATE) {
        RequestCommandProc();
    }
    return TRUE;
}

void SND_WaitForCommandProc(u32 tag) {
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    while (SND_RecvCommandReply(SND_COMMAND_NOBLOCK) != NULL) {
    }
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    RequestCommandProc();
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    do {
        SND_RecvCommandReply(SND_COMMAND_BLOCK);
    } while (!SND_IsFinishedCommandTag(tag));
}

u32 SND_GetCurrentCommandTag(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    u32 tag;
    if (sReserveList == NULL) {
        tag = sFinishCommandTag;
    } else {
        tag = sCurrentTag;
    }
    OS_RestoreInterrupts(enabled);
    return tag;
}

BOOL SND_IsFinishedCommandTag(u32 tag) {
    OSIntrMode enabled = OS_DisableInterrupts();
    BOOL result;
    if (tag > sFinishCommandTag) {
        if (tag - sFinishCommandTag < 0x80000000) {
            result = FALSE;
        } else {
            result = TRUE;
        }
    } else {
        if (sFinishCommandTag - tag < 0x80000000) {
            result = TRUE;
        } else {
            result = FALSE;
        }
    }
    OS_RestoreInterrupts(enabled);
    return result;
}

int SND_CountFreeCommand(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    SNDCommand *command;
    int count = 0;
    for (command = sFreeList; command != NULL; command = command->next) {
        count++;
    }
    OS_RestoreInterrupts(enabled);
    return count;
}

int SND_CountReservedCommand(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    SNDCommand *command;
    int count = 0;
    for (command = sReserveList; command != NULL; command = command->next) {
        count++;
    }
    OS_RestoreInterrupts(enabled);
    return count;
}

int SND_CountWaitingCommand(void) {
    int freeCount = SND_CountFreeCommand();
    int reservedCount = SND_CountReservedCommand();
    return SND_COMMAND_NUM - freeCount - reservedCount;
}

static void PxiFifoCallback(int tag, u32 data, BOOL err) {
    OSIntrMode enabled = OS_DisableInterrupts();
    SNDi_CallAlarmHandler((int) data);
    OS_RestoreInterrupts(enabled);
}

static void InitPXI(void) {
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_SOUND, PxiFifoCallback);
    if (IsCommandAvailable()) {
        while (!PXI_IsCallbackReady(PXI_FIFO_TAG_SOUND, PXI_PROC_ARM7)) {
            OS_SpinWait(100);
        }
    }
}

static void RequestCommandProc(void) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_SOUND, 0, FALSE) < 0) {
    }
}

static SNDCommand *AllocCommand(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    SNDCommand *command;
    if (sFreeList == NULL) {
        OS_RestoreInterrupts(enabled);
        return NULL;
    }
    command = sFreeList;
    sFreeList = command->next;
    if (sFreeList == NULL) {
        sFreeListEnd = NULL;
    }
    OS_RestoreInterrupts(enabled);
    return command;
}

static BOOL IsCommandAvailable(void) {
    OSIntrMode enabled;
    u32 result;
    if (!OS_IsRunOnEmulator()) {
        return TRUE;
    }
    enabled = OS_DisableInterrupts();
    *(volatile u32 *) 0x4fff200 = 0x10;
    result = *(volatile u32 *) 0x4fff200;
    OS_RestoreInterrupts(enabled);
    return result ? TRUE : FALSE;
}
