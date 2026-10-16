// The main loop and its hooks.

#include "m/looper.h"

#define REG_VCOUNT (*(volatile u16 *)0x04000006)

extern "C" {
void func_020508fc(int a, MTask *task);
void func_02050934(int a, MTask *task);
void func_02075054(void);
}

MLooper *gLooper;
OSThread *gLooperThread;

static inline BOOL IsIrqDisabled() {
    return REG_IME == 0;
}

// Hooks may only be changed from the looper's thread, or with interrupts off.
static inline void CheckLooperContext() {
    if (OS_GetThreadPriority(OS_GetCurrentThread()) > OS_GetThreadPriority(gLooperThread)) {
        if (!IsIrqDisabled()) {
            OS_Terminate();
        }
    }
}

MLooperVSyncHookClock::MLooperVSyncHookClock(MLooperVSyncHook *hook) {
    mHook = hook;
}

void MLooperVSyncHookClock::Execute() {
    if (mHook->mEnabled) {
        mHook->OnVSync();
    }
}

MLooperVSyncHook::MLooperVSyncHook(BOOL first, u8 enabled) {
    mEnabled = enabled;
    if (mEnabled) {
        CheckLooperContext();
    }
    mClock = new MLooperVSyncHookClock(this);
    if (!first) {
        gLooper->mVSyncTasks.AddTail(mClock);
    } else {
        gLooper->mVSyncTasks.AddHead(mClock);
    }
}

void MLooperVSyncHook::Enable() {
    MIrqLock lock;
    mEnabled = TRUE;
}

void MLooperVSyncHook::Disable() {
    MIrqLock lock;
    mEnabled = FALSE;
}

MLooperVSyncHook::~MLooperVSyncHook() {
    if (mEnabled) {
        CheckLooperContext();
    }
    gLooper->mVSyncTasks.Remove(mClock);
    delete mClock;
}

void MLooperVSyncTask::Execute() {
}

MLooperPreLoopHookClock::MLooperPreLoopHookClock(MLooperPreLoopHook *hook) {
    mHook = hook;
}

void MLooperPreLoopHookClock::Execute() {
    if (mHook->mEnabled) {
        mHook->OnPreLoop();
    }
}

MLooperPreLoopHook::MLooperPreLoopHook(BOOL first, u8 enabled) {
    mEnabled = enabled;
    if (mEnabled) {
        CheckLooperContext();
    }
    mClock = new MLooperPreLoopHookClock(this);
    if (!first) {
        gLooper->mPreLoopTasks.AddTail(mClock);
    } else {
        gLooper->mPreLoopTasks.AddHead(mClock);
    }
}

void MLooperPreLoopHook::Enable() {
    MIrqLock lock;
    mEnabled = TRUE;
}

MLooperPreLoopHook::~MLooperPreLoopHook() {
    if (mEnabled) {
        CheckLooperContext();
    }
    gLooper->mPreLoopTasks.Remove(mClock);
    delete mClock;
}

MLooperPostLoopHookClock::MLooperPostLoopHookClock(MLooperPostLoopHook *hook) {
    mHook = hook;
}

void MLooperPostLoopHookClock::Execute() {
    if (mHook->mEnabled) {
        mHook->OnPostLoop();
    }
}

MLooperPostLoopHook::MLooperPostLoopHook(BOOL first, u8 enabled) {
    mEnabled = enabled;
    if (mEnabled) {
        CheckLooperContext();
    }
    mClock = new MLooperPostLoopHookClock(this);
    if (!first) {
        gLooper->mPostLoopTasks.AddTail(mClock);
    } else {
        gLooper->mPostLoopTasks.AddHead(mClock);
    }
}

void MLooperPostLoopHook::Enable() {
    MIrqLock lock;
    mEnabled = TRUE;
}

MLooperPostLoopHook::~MLooperPostLoopHook() {
    if (mEnabled) {
        CheckLooperContext();
    }
    gLooper->mPostLoopTasks.Remove(mClock);
    delete mClock;
}

MLooperClock::MLooperClock(MLooper *looper) {
    mLooper = looper;
}

void MLooperClock::Execute() {
    mLooper->mFramesLeft--;
}

void InitLooper() {
    gLooperThread = OS_GetCurrentThread();
    gLooper = new MLooper(1);
}

MLooper::MLooper(int framesPerLoop)
    : mFramesPerLoop(framesPerLoop), mFramesLeft(framesPerLoop), mVCountMargin(0), mSkipFrame(FALSE),
      mWaiting(FALSE), mBoostPriority(TRUE) {
    OS_InitMutex(&mMutex);
    gLooper = this;
    mVSyncTasks.mUnkC = TRUE;
    mPreLoopTasks.mUnkC = TRUE;
    mPostLoopTasks.mUnkC = TRUE;
    mVSyncTasks.mUseMutex = TRUE;
    mPreLoopTasks.mUseMutex = TRUE;
    mPostLoopTasks.mUseMutex = TRUE;
    mClock = new MLooperClock(this);
    mThread = OS_GetCurrentThread();
    mPriority = OS_GetThreadPriority(mThread);
}

MLooper::~MLooper() {
}

int MLooper::GetVCountMargin() {
    return mVCountMargin;
}

void MLooper::RunUnkD4Tasks() {
    MScopedLock lock(&mMutex, TRUE);
    if (mUnkD4Tasks.mState == MTask::STATE_RUN) {
        mUnkD4Tasks.Execute();
    }
}

void MLooper::AddTransferTask(MTask *task) {
    MScopedLock lock(&mMutex, TRUE);
    mTransferTasks.AddTail(task);
}

void MLooper::RunTransferTasks() {
    MScopedLock lock(&mMutex, TRUE);
    if (mTransferTasks.mState == MTask::STATE_RUN) {
        mTransferTasks.Execute();
    }
}

OSThread *MLooper::GetThread() {
    return mThread;
}

void MLooper::Run(MTask *root) {
    u32 prio;

    mFramesLeft = mFramesPerLoop;
    func_020508fc(0, mClock);
    do {
        if (!mSkipFrame) {
            if (mPreLoopTasks.mState == MTask::STATE_RUN) {
                mPreLoopTasks.Execute();
            }
            if (root->mState == MTask::STATE_RUN) {
                root->Execute();
            }
            if (mPostLoopTasks.mState == MTask::STATE_RUN) {
                mPostLoopTasks.Execute();
            }
        } else {
            mSkipFrame = FALSE;
        }
        if (root->mState == MTask::STATE_EXIT) {
            delete root;
            root = NULL;
        }
        if (mFramesLeft <= 0) {
            mVCountMargin = 0x106;
        } else {
            int vcount = REG_VCOUNT;
            if (vcount > 0xC0) {
                mVCountMargin = vcount - 0xC0;
            } else {
                mVCountMargin = vcount + 0x46;
            }
        }
        mWaiting = TRUE;
        prio = OS_GetThreadPriority(mThread);
        if (mBoostPriority) {
            OS_SetThreadPriority(mThread, 3);
        }
        do {
            func_02075054();
        } while (mFramesLeft > 0);
        mWaiting = FALSE;
        mFramesLeft = mFramesPerLoop;
        if (mVSyncTasks.mState == MTask::STATE_RUN) {
            mVSyncTasks.Execute();
        }
        RunUnkD4Tasks();
        RunTransferTasks();
        if (mBoostPriority) {
            OS_SetThreadPriority(mThread, prio);
        }
    } while (root != NULL && root->mState == MTask::STATE_RUN);
    func_02050934(0, mClock);
}
