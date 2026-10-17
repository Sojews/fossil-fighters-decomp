// NitroSDK OS: message queues (a ring of pointer-sized messages, with threads waiting to send or receive).

#include "nitro/os.h"

BOOL OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
BOOL OS_JamMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);

void OS_InitMessageQueue(OSMessageQueue *mq, OSMessage *msgArray, s32 msgCount) {
    OS_InitThreadQueue(&mq->queueSend);
    OS_InitThreadQueue(&mq->queueReceive);
    mq->msgArray = msgArray;
    mq->msgCount = msgCount;
    mq->firstIndex = 0;
    mq->usedCount = 0;
}

BOOL OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags) {
    OSIntrMode enabled = OS_DisableInterrupts();
    s32 lastIndex;
    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OS_RestoreInterrupts(enabled);
            return FALSE;
        }
        OS_SleepThread(&mq->queueSend);
    }
    lastIndex = (mq->firstIndex + mq->usedCount) % mq->msgCount;
    mq->msgArray[lastIndex] = msg;
    mq->usedCount++;
    OS_WakeupThread(&mq->queueReceive);
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

BOOL OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags) {
    OSIntrMode enabled = OS_DisableInterrupts();
    while (mq->usedCount == 0) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OS_RestoreInterrupts(enabled);
            return FALSE;
        }
        OS_SleepThread(&mq->queueReceive);
    }
    if (msg != NULL) {
        *msg = mq->msgArray[mq->firstIndex];
    }
    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;
    OS_WakeupThread(&mq->queueSend);
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

BOOL OS_JamMessage(OSMessageQueue *mq, OSMessage msg, s32 flags) {
    OSIntrMode enabled = OS_DisableInterrupts();
    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OS_RestoreInterrupts(enabled);
            return FALSE;
        }
        OS_SleepThread(&mq->queueSend);
    }
    mq->firstIndex = (mq->firstIndex + mq->msgCount - 1) % mq->msgCount;
    mq->msgArray[mq->firstIndex] = msg;
    mq->usedCount++;
    OS_WakeupThread(&mq->queueReceive);
    OS_RestoreInterrupts(enabled);
    return TRUE;
}
