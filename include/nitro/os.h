// The parts of the NitroSDK OS library the game code uses (declarations only).
#ifndef NITRO_OS_H
#define NITRO_OS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSThread OSThread;

typedef u32 OSIntrMode;
OSIntrMode OS_DisableInterrupts(void);
OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
typedef struct OSMutex OSMutex;

typedef struct OSThreadQueue {
    OSThread *head;
    OSThread *tail;
} OSThreadQueue;

typedef struct OSMutexLink {
    OSMutex *next;
    OSMutex *prev;
} OSMutexLink;

typedef void *OSMessage;

#define OS_MESSAGE_NOBLOCK 0
#define OS_MESSAGE_BLOCK 1

typedef struct OSMessageQueue {
    OSThreadQueue queueSend;
    OSThreadQueue queueReceive;
    OSMessage *msgArray;
    s32 msgCount;
    s32 firstIndex;
    s32 usedCount;
} OSMessageQueue;

BOOL OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);

struct OSMutex {
    OSThreadQueue queue;
    OSThread *thread;   // owner
    s32 count;          // lock depth
    OSMutexLink link;
};

// The SDK's thread structure; the game code only embeds threads in its objects and reads their ids.
struct OSThread {
    u8 unk0[0x6C];
    u32 id;         // 0x6C
    u8 unk70[0x50];
};

static inline u32 OS_GetThreadId(const OSThread *thread) {
    return thread->id;
}

typedef void (*OSThreadFunc)(void *arg);

static inline void OS_InitThreadQueue(OSThreadQueue *queue) {
    queue->head = queue->tail = NULL;
}

typedef struct OSThreadInfo {
    u16 isNeedRescheduling;
    u16 irqDepth;
    OSThread *current;
    OSThread *list;
    void *switchCallback;
} OSThreadInfo;

extern OSThreadInfo OSi_ThreadInfo;

static inline OSThread *OS_GetCurrentThread(void) {
    return OSi_ThreadInfo.current;
}

void OS_CreateThread(OSThread *thread, OSThreadFunc func, void *arg, void *stack, u32 stackSize, u32 prio);
void OS_SetThreadStackWarningOffset(OSThread *thread, u32 offset);
void OS_SleepThreadDirect(OSThread *thread, OSThreadQueue *queue);
void OS_SleepThread(OSThreadQueue *queue);
void OS_WakeupThread(OSThreadQueue *queue);
void OS_WakeupThreadDirect(OSThread *thread);
u32 OS_GetThreadPriority(const OSThread *thread);
BOOL OS_SetThreadPriority(OSThread *thread, u32 prio);

typedef void (*OSIrqFunction)(void);

#define OS_IE_V_BLANK 0x1
#define OS_IE_H_BLANK 0x2
#define OS_IE_V_COUNT 0x4
#define OS_IE_KEY 0x1000
#define OS_IE_CARTRIDGE 0x2000

extern u8 SDK_AUTOLOAD_DTCM_START[];

// Tells OS_WaitIrq which interrupts have happened (the check flags at the end of DTCM).
static inline void OS_SetIrqCheckFlag(u32 mask) {
    *(volatile u32 *) ((u32) SDK_AUTOLOAD_DTCM_START + 0x3FF8) |= mask;
}

static inline u32 OS_GetIrqCheckFlag(void) {
    return *(volatile u32 *) ((u32) SDK_AUTOLOAD_DTCM_START + 0x3FF8);
}

static inline void OS_ClearIrqCheckFlag(u32 mask) {
    *(volatile u32 *) ((u32) SDK_AUTOLOAD_DTCM_START + 0x3FF8) &= ~mask;
}

void OS_InitIrqTable(void);
void OS_SetIrqFunction(u32 mask, OSIrqFunction func);
u32 OS_SetIrqMask(u32 mask);
u32 OS_EnableIrqMask(u32 mask);
u32 OS_DisableIrqMask(u32 mask);
u32 OS_ResetRequestIrqMask(u32 mask);
void OS_WaitIrq(BOOL clear, u32 irqFlags);
void OS_SleepThread(OSThreadQueue *queue);

void OS_InitMutex(OSMutex *mutex);
void OS_LockMutex(OSMutex *mutex);
BOOL OS_TryLockMutex(OSMutex *mutex);
void OS_UnlockMutex(OSMutex *mutex);
void OS_Terminate(void);
void OS_Init(void);
void OS_SetIrqStackWarningOffset(u32 offset);

typedef enum {
    OS_ARENA_MAIN = 0,
} OSArenaId;

void *OS_GetArenaLo(OSArenaId id);
void *OS_GetArenaHi(OSArenaId id);
void OS_SetArenaLo(OSArenaId id, void *newLo);
void DC_FlushAll(void);
void OS_ResetSystem(u32 parameter);
void OS_InitTick(void);
int OS_SPrintf(char *dst, const char *fmt, ...);
void OS_GetLowEntropyData(u32 buffer[8]);

#ifdef __cplusplus
}
#endif

#endif
