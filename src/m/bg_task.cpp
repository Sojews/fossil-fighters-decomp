// MBgTask and MBgTaskManager.

#include <algorithm>
#include "m/bg_task.h"
#include "m/display.h"
#include "m/heap.h"

void func_0204bbfc(OSThread *thread);

MBgTaskManager::MBgTaskManager(u32 priority) : MLooperPostLoopHook(FALSE, TRUE) {
    OS_InitMutex(&mMutex);
    OS_CreateThread(&mThread, ThreadMain, this, mStack + sizeof(mStack), sizeof(mStack), priority);
    OS_SetThreadStackWarningOffset(&mThread, 0x200);
    OS_InitThreadQueue(&mQueue);
    OS_SleepThreadDirect(&mThread, &mQueue);
    func_0204bbfc(&mThread);
}

void MBgTaskManager::ThreadMain(void *arg) {
    ((MBgTaskManager *) arg)->Run();
}

void MBgTaskManager::Add(MBgTask *task) {
    MScopedLock lock(&mMutex, TRUE);
    mTasks.push_back(task);
}

void MBgTaskManager::Remove(MBgTask *task) {
    OSMutex *mutex = &mMutex;
    OS_LockMutex(mutex);
    std::list<MBgTask *>::iterator it = std::find(mTasks.begin(), mTasks.end(), task);
    if (it != mTasks.end()) {
        mTasks.erase(it);
    }
    OS_UnlockMutex(mutex);
}

void MBgTaskManager::OnPostLoop() {
    MScopedLock lock(&mMutex, TRUE);
    if (mTasks.size() != 0) {
        OS_WakeupThread(&mQueue);
    }
}

void MBgTaskManager::Run() {
    for (;;) {
        OS_LockMutex(&mMutex);
        if (mTasks.size() == 0) {
            OS_UnlockMutex(&mMutex);
            OS_SleepThread(&mQueue);
            continue;
        }
        MBgTask *task = mTasks.front();
        mTasks.erase(mTasks.begin());
        OS_LockMutex(&task->mMutex);
        OS_UnlockMutex(&mMutex);
        task->RunOnThread();
        OS_UnlockMutex(&task->mMutex);
    }
}

MBgTaskManager *gBgTaskManagers[3];

void MInitBgTaskManagers() {
    gBgTaskManagers[0] = new MBgTaskManager(22);
    gBgTaskManagers[1] = new MBgTaskManager(23);
    gBgTaskManagers[2] = new MBgTaskManager(24);
}

MBgTask::MBgTask(MDisplay *display, int thread) {
    if (display == NULL) {
        display = gCurrentDisplay;
    }
    mDisplay = display;
    OS_InitMutex(&mMutex);
    mThread = thread;
    mQueued = FALSE;
    mCanceled = FALSE;
}

MBgTask::~MBgTask() {
    // Deleting a task that's queued or running would leave the worker with a dangling pointer.
    if (mQueued && !mCanceled) {
        OS_Terminate();
    }
}

void MBgTask::Cancel() {
    gBgTaskManagers[mThread]->Remove(this);
    MMutexLock lock(&mMutex);
    mCanceled = TRUE;
}

void MBgTask::Execute() {
    if (!mQueued) {
        gBgTaskManagers[mThread]->Add(this);
        mQueued = TRUE;
    }
}

void MBgTask::RunOnThread() {
    gCurrentDisplay = mDisplay;
    Run();
    Exit();
}

MBgTaskManager::~MBgTaskManager() {
}
