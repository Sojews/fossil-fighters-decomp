// The main loop: runs the root task once per frame, with task sets before and after it and at each VBlank.
#ifndef M_LOOPER_H
#define M_LOOPER_H

#include "m/task.h"
#include "m/irq.h"

class MLooper {
public:
    MLooper(int framesPerLoop);
    virtual ~MLooper();

    int GetVCountMargin();
    void RunUnkD4Tasks();
    void AddTransferTask(MTask *task);
    void RunTransferTasks();
    OSThread *GetThread();
    void Run(MTask *root);

    int mFramesPerLoop;             // 0x04
    int mFramesLeft;                // 0x08: counted down at each VBlank
    int mVCountMargin;              // 0x0C
    u8 mSkipFrame;                  // 0x10
    u8 mWaiting;                    // 0x11: waiting for VBlank
    u8 mBoostPriority;              // 0x12: raise the thread's priority while waiting for VBlank
    MParallelTaskSet mVSyncTasks;   // 0x14
    MParallelTaskSet mPreLoopTasks; // 0x44
    MParallelTaskSet mPostLoopTasks;// 0x74
    MParallelTaskSet mTransferTasks;// 0xA4
    MParallelTaskSet mUnkD4Tasks;   // 0xD4
    MTask *mClock;                  // 0x104: counts down mFramesLeft at each VBlank
    OSMutex mMutex;                 // 0x108
    OSThread *mThread;              // 0x120
    u32 mPriority;                  // 0x124
};

extern MLooper *gLooper;
extern OSThread *gLooperThread;

// Something to do at each VBlank. While enabled, OnVSync() is called from the looper's VBlank task set.
class MLooperVSyncHook {
public:
    MLooperVSyncHook(BOOL first, u8 enabled);
    virtual ~MLooperVSyncHook();
    virtual void OnVSync() = 0;

    void Enable();
    void Disable();

    MTask *mClock;  // 0x04
    u8 mEnabled;    // 0x08
};

class MLooperVSyncHookClock : public MTask {
public:
    MLooperVSyncHookClock(MLooperVSyncHook *hook);
    virtual ~MLooperVSyncHookClock() {}
    virtual void Execute();

    MLooperVSyncHook *mHook;    // 0x10
};

// Something to do before the root task each frame.
class MLooperPreLoopHook {
public:
    MLooperPreLoopHook(BOOL first, u8 enabled);
    virtual ~MLooperPreLoopHook();
    virtual void OnPreLoop() = 0;

    void Enable();
    void Disable();

    MTask *mClock;  // 0x04
    u8 mEnabled;    // 0x08
};

class MLooperPreLoopHookClock : public MTask {
public:
    MLooperPreLoopHookClock(MLooperPreLoopHook *hook);
    virtual ~MLooperPreLoopHookClock() {}
    virtual void Execute();

    MLooperPreLoopHook *mHook;  // 0x10
};

// Something to do after the root task each frame.
class MLooperPostLoopHook {
public:
    MLooperPostLoopHook(BOOL first, u8 enabled);
    virtual ~MLooperPostLoopHook();
    virtual void OnPostLoop() = 0;

    void Enable();
    void Disable();

    MTask *mClock;  // 0x04
    u8 mEnabled;    // 0x08
};

class MLooperPostLoopHookClock : public MTask {
public:
    MLooperPostLoopHookClock(MLooperPostLoopHook *hook);
    virtual ~MLooperPostLoopHookClock() {}
    virtual void Execute();

    MLooperPostLoopHook *mHook; // 0x10
};

// Counts down the looper's frames at each VBlank.
class MLooperClock : public MTask {
public:
    MLooperClock(MLooper *looper);
    virtual ~MLooperClock() {}
    virtual void Execute();

    MLooper *mLooper;   // 0x10
};

// A task that's also run at each VBlank.
class MLooperVSyncTask : public MTask, public MLooperVSyncHook {
public:
    MLooperVSyncTask(BOOL first, u8 enabled) : MLooperVSyncHook(first, enabled) {}
    virtual ~MLooperVSyncTask() {}
    virtual void Execute();
};

void InitLooper();

#endif
