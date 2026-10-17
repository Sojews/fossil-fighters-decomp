// NitroSDK OS: threads. They run by priority; the lowest number that's ready runs.

#include "nitro/mi.h"
#include "nitro/os.h"

#define OS_THREAD_STATE_WAITING 0
#define OS_THREAD_STATE_READY 1
#define OS_THREAD_STATE_TERMINATED 2

#define OS_THREAD_LAUNCHER_PRIORITY 16
#define OS_THREAD_PRIORITY_MAX 31
#define OS_IDLE_THREAD_STACK_SIZE 200

#define OSi_STACK_CHECKNUM_BOTTOM 0xfddb597d
#define OSi_STACK_CHECKNUM_TOP 0x7bf9dd5b
#define OSi_STACK_CHECKNUM_WARN 0x597dfbd9

#define HW_PSR_IRQ_MODE 0x12
#define HW_PSR_IRQ_DISABLE 0x80

#define DTCM_ARENA_LO 0x027e0520
#define IRQ_STACK_END ((u32) SDK_AUTOLOAD_DTCM_START + 0x3f80)

// Linker-defined sizes.
extern char SDK_SYS_STACKSIZE[];
extern char SDK_IRQ_STACKSIZE[];

void OS_InitContext(OSContext *context, u32 newpc, u32 newsp);
BOOL OS_SaveContext(OSContext *context);
void OS_LoadContext(OSContext *context);
u32 OS_GetProcMode(void);
OSIntrMode OS_EnableInterrupts(void);
void OS_Halt(void);
void OSi_UnlockAllMutex(OSThread *thread);
u32 OS_DisableScheduler(void);
u32 OS_EnableScheduler(void);
void OS_RescheduleThread(void);
OSSwitchThreadCallback OS_SetSwitchThreadCallback(OSSwitchThreadCallback callback);
void OS_SetThreadDestructor(OSThread *thread, OSThreadDestructor dtor);
OSThread *OS_SelectThread(void);
void OS_ExitThread(void);
static void OSi_ExitThread(void *arg);
static void OSi_ExitThread_Destroy(void);
static void OSi_IdleThreadProc(void *arg);

// (With -ipa file a file's statics are laid out in a permutation of their declaration order.)
static u32 OSi_IdleThreadStack[OS_IDLE_THREAD_STACK_SIZE / sizeof(u32)];
static BOOL OSi_IsThreadInitialized;
static u32 OSi_Unused10;
static OSThread **OSi_CurrentThreadPtr;
static OSSwitchThreadCallback OSi_SystemCallbackInSwitchThread;
static u32 OSi_RescheduleCount;
static u32 OSi_Unused14;
static u32 OSi_Unused18;
void *OSi_StackForDestructor;
static s32 OSi_ThreadIdCount;
OSThreadInfo OSi_ThreadInfo;
static OSThread OSi_LauncherThread;
static OSThread OSi_IdleThread;

static s32 OSi_GetUnusedThreadId(void) {
    return ++OSi_ThreadIdCount;
}

static void OSi_InsertLinkToQueue(OSThreadQueue *queue, OSThread *thread) {
    OSThread *next = queue->head;
    while (next != NULL && next->priority <= thread->priority) {
        if (next == thread) {
            return;
        }
        next = next->link.next;
    }
    if (next == NULL) {
        OSThread *prev = queue->tail;
        if (prev == NULL) {
            queue->head = thread;
        } else {
            prev->link.next = thread;
        }
        thread->link.prev = prev;
        thread->link.next = NULL;
        queue->tail = thread;
    } else {
        OSThread *prev = next->link.prev;
        if (prev == NULL) {
            queue->head = thread;
        } else {
            prev->link.next = thread;
        }
        thread->link.prev = prev;
        thread->link.next = next;
        next->link.prev = thread;
    }
}

static OSThread *OSi_RemoveLinkFromQueue(OSThreadQueue *queue) {
    OSThread *t = queue->head;
    if (t != NULL) {
        OSThread *next = t->link.next;
        queue->head = next;
        if (next != NULL) {
            next->link.prev = NULL;
        } else {
            queue->tail = NULL;
            t->queue = NULL;
        }
    }
    return t;
}

static OSThread *OSi_RemoveSpecifiedLinkFromQueue(OSThreadQueue *queue, OSThread *thread) {
    OSThread *t = queue->head;
    while (t != NULL) {
        OSThread *next = t->link.next;
        if (t == thread) {
            OSThread *prev = t->link.prev;
            if (queue->head == t) {
                queue->head = next;
            } else {
                prev->link.next = next;
            }
            if (queue->tail == t) {
                queue->tail = prev;
            } else {
                next->link.prev = prev;
            }
            break;
        }
        t = next;
    }
    return t;
}

OSMutex *OSi_RemoveMutexLinkFromQueue(OSMutexQueue *queue) {
    OSMutex *mutex = queue->head;
    if (mutex != NULL) {
        OSMutex *next = mutex->link.next;
        queue->head = next;
        if (next != NULL) {
            next->link.prev = NULL;
        } else {
            queue->tail = NULL;
        }
    }
    return mutex;
}

static void OSi_InsertThreadToList(OSThread *thread) {
    OSThread *t = OSi_ThreadInfo.list;
    OSThread *pre = NULL;
    while (t != NULL && t->priority < thread->priority) {
        pre = t;
        t = t->next;
    }
    if (pre == NULL) {
        thread->next = OSi_ThreadInfo.list;
        OSi_ThreadInfo.list = thread;
    } else {
        thread->next = pre->next;
        pre->next = thread;
    }
}

static void OSi_RemoveThreadFromList(OSThread *thread) {
    OSThread *t = OSi_ThreadInfo.list;
    OSThread *pre = NULL;
    while (t != NULL && t != thread) {
        pre = t;
        t = t->next;
    }
    if (pre == NULL) {
        OSi_ThreadInfo.list = thread->next;
    } else {
        pre->next = thread->next;
    }
}

static void OSi_RescheduleThread(void) {
    if (OSi_RescheduleCount == 0) {
        OSThreadInfo *info = &OSi_ThreadInfo;
        if (OSi_ThreadInfo.irqDepth > 0 || OS_GetProcMode() == HW_PSR_IRQ_MODE) {
            info->isNeedRescheduling = TRUE;
        } else {
            OSThread *currentThread = *OSi_CurrentThreadPtr;
            OSThread *nextThread = OS_SelectThread();
            if (currentThread == nextThread || nextThread == NULL) {
                return;
            }
            if (currentThread->state != OS_THREAD_STATE_TERMINATED && OS_SaveContext(&currentThread->context)) {
                return;
            }
            if (OSi_SystemCallbackInSwitchThread != NULL) {
                OSi_SystemCallbackInSwitchThread(currentThread, nextThread);
            }
            if (info->switchCallback != NULL) {
                info->switchCallback(currentThread, nextThread);
            }
            OSi_ThreadInfo.current = nextThread;
            OS_LoadContext(&nextThread->context);
        }
    }
}

void OS_InitThread(void) {
    u32 stackLo;
    if (OSi_IsThreadInitialized) {
        return;
    }
    OSi_IsThreadInitialized = TRUE;
    OSi_CurrentThreadPtr = &OSi_ThreadInfo.current;
    OSi_LauncherThread.priority = OS_THREAD_LAUNCHER_PRIORITY;
    OSi_LauncherThread.id = 0;
    OSi_LauncherThread.state = OS_THREAD_STATE_READY;
    OSi_LauncherThread.next = NULL;
    OSi_LauncherThread.profiler = NULL;
    OSi_ThreadInfo.list = &OSi_LauncherThread;
    OSi_ThreadInfo.current = &OSi_LauncherThread;
    if ((s32) SDK_SYS_STACKSIZE <= 0) {
        stackLo = DTCM_ARENA_LO - (u32) SDK_SYS_STACKSIZE;
    } else {
        stackLo = IRQ_STACK_END - (u32) SDK_IRQ_STACKSIZE - (u32) SDK_SYS_STACKSIZE;
    }
    OSi_LauncherThread.stackBottom = IRQ_STACK_END - (u32) SDK_IRQ_STACKSIZE;
    OSi_LauncherThread.stackTop = stackLo;
    OSi_LauncherThread.stackWarningOffset = 0;
    *(u32 *) (OSi_LauncherThread.stackBottom - sizeof(u32)) = OSi_STACK_CHECKNUM_BOTTOM;
    *(u32 *) OSi_LauncherThread.stackTop = OSi_STACK_CHECKNUM_TOP;
    OSi_LauncherThread.joinQueue.tail = NULL;
    OSi_LauncherThread.joinQueue.head = NULL;
    OSi_ThreadInfo.isNeedRescheduling = FALSE;
    OSi_ThreadInfo.irqDepth = 0;
    *(OSThreadInfo **) 0x027fffa0 = &OSi_ThreadInfo;
    OS_SetSwitchThreadCallback(NULL);
    OS_CreateThread(&OSi_IdleThread, OSi_IdleThreadProc, NULL,
                    OSi_IdleThreadStack + OS_IDLE_THREAD_STACK_SIZE / sizeof(u32), OS_IDLE_THREAD_STACK_SIZE,
                    OS_THREAD_PRIORITY_MAX);
    OSi_IdleThread.priority = OS_THREAD_PRIORITY_MAX + 1;
    OSi_IdleThread.state = OS_THREAD_STATE_READY;
}

void OS_CreateThread(OSThread *thread, OSThreadFunc func, void *arg, void *stack, u32 stackSize, u32 prio) {
    OSIntrMode enabled = OS_DisableInterrupts();
    s32 index = OSi_GetUnusedThreadId();
    thread->priority = prio;
    thread->id = (u32) index;
    thread->state = OS_THREAD_STATE_WAITING;
    thread->profiler = NULL;
    OSi_InsertThreadToList(thread);
    thread->stackBottom = (u32) stack;
    thread->stackTop = (u32) stack - stackSize;
    thread->stackWarningOffset = 0;
    *(u32 *) (thread->stackBottom - sizeof(u32)) = OSi_STACK_CHECKNUM_BOTTOM;
    *(u32 *) thread->stackTop = OSi_STACK_CHECKNUM_TOP;
    thread->joinQueue.head = thread->joinQueue.tail = NULL;
    OS_InitContext(&thread->context, (u32) func, (u32) stack - 4);
    thread->context.r[0] = (u32) arg;
    thread->context.lr = (u32) OS_ExitThread;
    MIi_CpuClear32(0, (void *) ((u32) stack - stackSize + sizeof(u32)), stackSize - sizeof(u32) * 2);
    thread->mutex = NULL;
    thread->mutexQueue.head = NULL;
    thread->mutexQueue.tail = NULL;
    OS_SetThreadDestructor(thread, NULL);
    thread->queue = NULL;
    thread->link.prev = thread->link.next = NULL;
    MIi_CpuClear32(0, thread->specific, sizeof(thread->specific));
    thread->alarmForSleep = NULL;
    OS_RestoreInterrupts(enabled);
}

static void OSi_ExitThread_ArgSpecified(OSThread *thread, void *arg);

void OS_ExitThread(void) {
    OS_DisableInterrupts();
    OSi_ExitThread_ArgSpecified(OSi_ThreadInfo.current, NULL);
}

static void OSi_ExitThread_ArgSpecified(OSThread *thread, void *arg) {
    if (OSi_StackForDestructor != NULL) {
        OS_InitContext(&thread->context, (u32) OSi_ExitThread, (u32) OSi_StackForDestructor);
        thread->context.r[0] = (u32) arg;
        thread->context.cpsr |= HW_PSR_IRQ_DISABLE;
        thread->state = OS_THREAD_STATE_READY;
        OS_LoadContext(&thread->context);
    } else {
        OSi_ExitThread(arg);
    }
}

static void OSi_ExitThread(void *arg) {
    OSThread *currentThread = *OSi_CurrentThreadPtr;
    OSThreadDestructor destructor = currentThread->destructor;
    if (destructor != NULL) {
        currentThread->destructor = NULL;
        destructor(arg);
        OS_DisableInterrupts();
    }
    OSi_ExitThread_Destroy();
}

static void OSi_ExitThread_Destroy(void) {
    OSThread *currentThread = *OSi_CurrentThreadPtr;
    OS_DisableScheduler();
    OSi_UnlockAllMutex(currentThread);
    if (currentThread->queue != NULL) {
        OSi_RemoveSpecifiedLinkFromQueue(currentThread->queue, currentThread);
    }
    OSi_RemoveThreadFromList(currentThread);
    currentThread->state = OS_THREAD_STATE_TERMINATED;
    OS_WakeupThread(&currentThread->joinQueue);
    OS_EnableScheduler();
    OS_RescheduleThread();
    OS_Terminate();
}

void OS_SleepThreadDirect(OSThread *thread, OSThreadQueue *queue) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (queue != NULL) {
        thread->queue = queue;
        OSi_InsertLinkToQueue(queue, thread);
    }
    thread->state = OS_THREAD_STATE_WAITING;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(enabled);
}

void OS_SleepThread(OSThreadQueue *queue) {
    OSIntrMode enabled = OS_DisableInterrupts();
    OSThread *currentThread = *OSi_CurrentThreadPtr;
    if (queue != NULL) {
        currentThread->queue = queue;
        OSi_InsertLinkToQueue(queue, currentThread);
    }
    currentThread->state = OS_THREAD_STATE_WAITING;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(enabled);
}

void OS_WakeupThread(OSThreadQueue *queue) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (queue->head != NULL) {
        while (queue->head != NULL) {
            OSThread *thread = OSi_RemoveLinkFromQueue(queue);
            thread->state = OS_THREAD_STATE_READY;
            thread->queue = NULL;
            thread->link.prev = thread->link.next = NULL;
        }
        OS_InitThreadQueue(queue);
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(enabled);
}

void OS_WakeupThreadDirect(OSThread *thread) {
    OSIntrMode enabled = OS_DisableInterrupts();
    thread->state = OS_THREAD_STATE_READY;
    OSi_RescheduleThread();
    OS_RestoreInterrupts(enabled);
}

OSThread *OS_SelectThread(void) {
    OSThread *t;
    for (t = OSi_ThreadInfo.list; t != NULL && t->state != OS_THREAD_STATE_READY; t = t->next) {
    }
    return t;
}

void OS_RescheduleThread(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    OSi_RescheduleThread();
    OS_RestoreInterrupts(enabled);
}

void OS_SetThreadStackWarningOffset(OSThread *thread, u32 offset) {
    thread->stackWarningOffset = offset;
    if (offset != 0) {
        *(u32 *) (thread->stackTop + offset) = OSi_STACK_CHECKNUM_WARN;
    }
}

BOOL OS_SetThreadPriority(OSThread *thread, u32 prio) {
    OSThread *t = OSi_ThreadInfo.list;
    OSThread *pre = NULL;
    OSIntrMode enabled = OS_DisableInterrupts();
    while (t != NULL && t != thread) {
        pre = t;
        t = t->next;
    }
    if (t == NULL || t == &OSi_IdleThread) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    if (t->priority != prio) {
        if (pre == NULL) {
            OSi_ThreadInfo.list = thread->next;
        } else {
            pre->next = thread->next;
        }
        thread->priority = prio;
        OSi_InsertThreadToList(thread);
        OSi_RescheduleThread();
    }
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

u32 OS_GetThreadPriority(const OSThread *thread) {
    return thread->priority;
}

OSSwitchThreadCallback OS_SetSwitchThreadCallback(OSSwitchThreadCallback callback) {
    OSIntrMode enabled = OS_DisableInterrupts();
    OSSwitchThreadCallback prev = OSi_ThreadInfo.switchCallback;
    OSi_ThreadInfo.switchCallback = callback;
    OS_RestoreInterrupts(enabled);
    return prev;
}

static void OSi_IdleThreadProc(void *arg) {
    OS_EnableInterrupts();
    for (;;) {
        OS_Halt();
    }
}

u32 OS_DisableScheduler(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    u32 count;
    if (OSi_RescheduleCount < (u32) -1) {
        count = OSi_RescheduleCount++;
    }
    OS_RestoreInterrupts(enabled);
    return count;
}

u32 OS_EnableScheduler(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    u32 count = 0;
    if (OSi_RescheduleCount > 0) {
        count = OSi_RescheduleCount--;
    }
    OS_RestoreInterrupts(enabled);
    return count;
}

void OS_SetThreadDestructor(OSThread *thread, OSThreadDestructor dtor) {
    thread->destructor = dtor;
}

// (Not linked into the game; it keeps the statics it uses.)
void OSi_ThreadUnusedStatics(void) {
    OSi_Unused10 = OSi_Unused14 = OSi_Unused18 = 0;
}
