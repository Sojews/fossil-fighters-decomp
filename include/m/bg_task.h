// Background tasks: work run on one of three worker threads (by priority) instead of the main loop.
#ifndef M_BG_TASK_H
#define M_BG_TASK_H

#include <list>
#include "m/task.h"
#include "m/looper.h"

class MDisplay;

// A task whose work runs on a worker thread. Running it as a task queues it once; it exits after the work is done.
class MBgTask : public MTask {
public:
    MBgTask(MDisplay *display, int thread);
    virtual ~MBgTask();
    virtual void Execute();
    // The work, run on the worker thread.
    virtual void Run() = 0;

    void Cancel();
    void RunOnThread();

    MDisplay *mDisplay; // 0x10: made current for the work
    OSMutex mMutex;     // 0x14: held while the work runs
    int mThread;        // 0x2C: which worker
    u8 mQueued;         // 0x30
    u8 mCanceled;       // 0x31
};

// A worker thread and its queue of background tasks. It's woken after each main loop pass while it has work.
class MBgTaskManager : public MLooperPostLoopHook {
public:
    MBgTaskManager(u32 priority);
    virtual ~MBgTaskManager();
    virtual void OnPostLoop();

    static void ThreadMain(void *arg);
    void Add(MBgTask *task);
    void Remove(MBgTask *task);
    void Run();

    std::list<MBgTask *> mTasks;    // 0x0C
    OSThread mThread;               // 0x18
    OSThreadQueue mQueue;           // 0xD8: the thread sleeps here while there's nothing to do
    u8 mStack[0x1000];              // 0xE0
    OSMutex mMutex;                 // 0x10E0
};

void MInitBgTaskManagers();

extern MBgTaskManager *gBgTaskManagers[3];

#endif
