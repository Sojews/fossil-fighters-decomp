// Task sets, processes and the small tasks that control other tasks.

#include "m/task.h"

void *gCurrentContext;

MTask *MTaskRunTransfer::Create(MTask *task) {
    return new MTaskRunTransfer(task);
}

MTask *MTaskSuspendTransfer::Create(MTask *task) {
    return new MTaskSuspendTransfer(task);
}

MTask *MTaskExitTransfer::Create(MTask *task) {
    return new MTaskExitTransfer(task);
}

MTask *MTaskSetWaitEmptyTask::Create(MTaskSet *set) {
    return new MTaskSetWaitEmptyTask(set);
}

MTask *MTaskSetMakeEmptyTask::Create(MTaskSet *set) {
    return new MTaskSetMakeEmptyTask(set);
}

MTaskSet::~MTaskSet() {
    Clear();
}

void MTaskSet::AddHead(MTask *task) {
    MScopedLock lock(&mMutex, mUseMutex);

    task->mInSet = TRUE;
    if (mHead == NULL) {
        mTail = &task->mNext;
    }
    task->mNext = mHead;
    mHead = task;
}

void MTaskSet::AddTail(MTask *task) {
    MScopedLock lock(&mMutex, mUseMutex);

    task->mInSet = TRUE;
    *mTail = task;
    mTail = &task->mNext;
    task->mNext = NULL;
}

void MTaskSet::Remove(MTask *task) {
    MScopedLock lock(&mMutex, mUseMutex);
    MTask **link;
    MTask *cur;

    task->mInSet = FALSE;
    link = &mHead;
    for (cur = mHead; cur != NULL; cur = cur->mNext) {
        if (cur == task) {
            *link = cur->mNext;
            if (&cur->mNext == mTail) {
                mTail = link;
            }
            return;
        }
        link = &cur->mNext;
    }
}

void MTaskSet::Clear() {
    MScopedLock lock(&mMutex, mUseMutex);
    MTask *task;
    MTask *next;

    for (task = mHead; task != NULL; task = next) {
        next = task->mNext;
        task->mInSet = FALSE;
        delete task;
    }
    mHead = NULL;
    mTail = &mHead;
}

void MTaskSet::Execute() {
}

MProcess::MProcess(void *context) {
    mSerial.mUnkC = TRUE;
    mParallel.mUnkC = TRUE;
    if (context == NULL) {
        context = gCurrentContext;
    }
    mContext = context;
}

// The per-frame task loops run from ITCM, as ARM code.
#pragma section ITCM begin
#pragma thumb off

void MSerialTaskSet::Execute() {
    MScopedLock lock(&mMutex, mUseMutex);
    MTask **link = &mHead;
    MTask *task;

    for (task = mHead; task != NULL; task = *link) {
        if (task->mState == STATE_RUN) {
            task->Execute();
        }
        if (task->mState == STATE_RUN) {
            break;
        }
        *link = task->mNext;
        task->mInSet = FALSE;
        if (task->mState == STATE_EXIT) {
            delete task;
        }
    }
    if (task == NULL) {
        mTail = link;
    }
    if (mExitWhenEmpty && IsEmpty()) {
        Exit();
    }
}

void MParallelTaskSet::Execute() {
    MScopedLock lock(&mMutex, mUseMutex);
    MTask **link = &mHead;
    MTask *task = mHead;

    while (task != NULL) {
        if (task->mState == STATE_RUN) {
            task->Execute();
        }
        if (task->mState == STATE_EXIT) {
            *link = task->mNext;
            task->mInSet = FALSE;
            delete task;
            task = *link;
        } else {
            link = &task->mNext;
            task = task->mNext;
        }
    }
    mTail = link;
    if (mExitWhenEmpty && IsEmpty()) {
        Exit();
    }
}

void MProcess::Execute() {
    MContextScope scope(mContext);

    if (mSerial.IsEmpty()) {
        Idle();
    }
    if (mSerial.mState == STATE_RUN) {
        mSerial.Execute();
    }
    if (mParallel.mState == STATE_RUN) {
        mParallel.Execute();
    }
}

#pragma thumb on
#pragma section ITCM end

void MProcess::SetContext(void *context) {
    mContext = context;
    gCurrentContext = context;
}

void *MProcess::GetContext() {
    return mContext;
}

void MTaskSetMakeEmptyTask::Transfer() {
    mSet->Clear();
}

void MTaskSetWaitEmptyTask::Execute() {
    if (mSet->IsEmpty()) {
        Exit();
    }
}

void MTaskExitTransfer::Transfer() {
    mTask->Exit();
}

void MTaskSuspendTransfer::Transfer() {
    mTask->Suspend();
}

void MTaskRunTransfer::Transfer() {
    mTask->Run();
}

void MTimer::Execute() {
    if (--mFrames < 0) {
        Exit();
    }
}
