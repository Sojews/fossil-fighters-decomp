// NitroSDK OS: recursive mutexes. Each thread keeps a list of the mutexes it holds so they can be released if
// it exits.

#include "nitro/os.h"

void OSi_EnqueueTail(OSThread *thread, OSMutex *mutex);
void OSi_DequeueItem(OSThread *thread, OSMutex *mutex);
OSMutex *OSi_RemoveMutexLinkFromQueue(OSMutexQueue *queue);

void OS_InitMutex(OSMutex *mutex) {
    OS_InitThreadQueue(&mutex->queue);
    mutex->thread = NULL;
    mutex->count = 0;
}

void OS_LockMutex(OSMutex *mutex) {
    OSIntrMode enabled = OS_DisableInterrupts();
    OSThread *currentThread = OS_GetCurrentThread();
    for (;;) {
        OSThread *ownerThread = ((volatile OSMutex *) mutex)->thread;
        if (ownerThread == NULL) {
            mutex->thread = currentThread;
            mutex->count++;
            OSi_EnqueueTail(currentThread, mutex);
            break;
        } else if (ownerThread == currentThread) {
            mutex->count++;
            break;
        } else {
            currentThread->mutex = mutex;
            OS_SleepThread(&mutex->queue);
            currentThread->mutex = NULL;
        }
    }
    OS_RestoreInterrupts(enabled);
}

void OS_UnlockMutex(OSMutex *mutex) {
    OSIntrMode enabled = OS_DisableInterrupts();
    OSThread *currentThread = OS_GetCurrentThread();
    if (mutex->thread == currentThread) {
        if (--mutex->count == 0) {
            OSi_DequeueItem(currentThread, mutex);
            mutex->thread = NULL;
            OS_WakeupThread(&mutex->queue);
        }
    }
    OS_RestoreInterrupts(enabled);
}

void OSi_UnlockAllMutex(OSThread *thread) {
    while (thread->mutexQueue.head != NULL) {
        OSMutex *mutex = OSi_RemoveMutexLinkFromQueue(&thread->mutexQueue);
        mutex->count = 0;
        mutex->thread = NULL;
        OS_WakeupThread(&mutex->queue);
    }
}

BOOL OS_TryLockMutex(OSMutex *mutex) {
    OSIntrMode enabled = OS_DisableInterrupts();
    OSThread *currentThread = OS_GetCurrentThread();
    BOOL locked;
    if (mutex->thread == NULL) {
        mutex->thread = currentThread;
        mutex->count++;
        OSi_EnqueueTail(currentThread, mutex);
        locked = TRUE;
    } else if (mutex->thread == currentThread) {
        mutex->count++;
        locked = TRUE;
    } else {
        locked = FALSE;
    }
    OS_RestoreInterrupts(enabled);
    return locked;
}

void OSi_EnqueueTail(OSThread *thread, OSMutex *mutex) {
    OSMutex *prev = thread->mutexQueue.tail;
    if (prev == NULL) {
        thread->mutexQueue.head = mutex;
    } else {
        prev->link.next = mutex;
    }
    mutex->link.prev = prev;
    mutex->link.next = NULL;
    thread->mutexQueue.tail = mutex;
}

void OSi_DequeueItem(OSThread *thread, OSMutex *mutex) {
    OSMutex *next = mutex->link.next;
    OSMutex *prev = mutex->link.prev;
    if (next == NULL) {
        thread->mutexQueue.tail = prev;
    } else {
        next->link.prev = prev;
    }
    if (prev == NULL) {
        thread->mutexQueue.head = next;
    } else {
        prev->link.next = next;
    }
}
