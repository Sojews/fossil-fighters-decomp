// The parts of the NitroSDK OS library the game code uses (declarations only).
#ifndef NITRO_OS_H
#define NITRO_OS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSThread OSThread;
typedef struct OSMutex OSMutex;

typedef struct OSThreadQueue {
    OSThread *head;
    OSThread *tail;
} OSThreadQueue;

typedef struct OSMutexLink {
    OSMutex *next;
    OSMutex *prev;
} OSMutexLink;

struct OSMutex {
    OSThreadQueue queue;
    OSThread *thread;   // owner
    s32 count;          // lock depth
    OSMutexLink link;
};

// The SDK's thread structure; only its size matters to the game code, which embeds threads in its objects.
struct OSThread {
    u8 opaque[0xC0];
};

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

void OS_InitMutex(OSMutex *mutex);
void OS_LockMutex(OSMutex *mutex);
void OS_UnlockMutex(OSMutex *mutex);
void OS_Terminate(void);
void OS_InitTick(void);
int OS_SPrintf(char *dst, const char *fmt, ...);
void OS_GetLowEntropyData(u32 buffer[8]);

#ifdef __cplusplus
}
#endif

#endif
