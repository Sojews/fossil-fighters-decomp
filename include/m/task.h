// Tasks: the game's cooperative update framework. Almost every game object is an MTask that is run once a frame
// from a task set. An MProcess owns two sets: a serial one (one task at a time, in order) and a parallel one
// (every task, every frame).
#ifndef M_TASK_H
#define M_TASK_H

#include "types.h"
#include "nitro/os.h"
#include "nitro/section.h"

class MDisplay;

// Locks a mutex for the rest of the scope, if asked to.
class MScopedLock {
public:
    MScopedLock(OSMutex *mutex, u8 lock) {
        mMutex = mutex;
        mLocked = lock;
        if (mLocked) {
            OS_LockMutex(mMutex);
        }
    }
    ~MScopedLock() {
        if (mLocked) {
            OS_UnlockMutex(mMutex);
        }
    }

    OSMutex *mMutex;
    u8 mLocked;
};

class MTask {
public:
    enum State {
        STATE_RUN = 0,
        STATE_SUSPEND = 1,
        STATE_EXIT = 2,     // the owning set deletes the task
    };

    MTask() {
        mState = STATE_RUN;
        mInSet = FALSE;
    }
    virtual ~MTask() {}
    virtual void Execute() = 0;

    void Exit() { mState = STATE_EXIT; }
    void Suspend() { mState = STATE_SUSPEND; }
    void Run() { mState = STATE_RUN; }
    // A transfer that exits this task.
    MTask *CreateExitTransfer();

    MTask *mNext;           // 0x04: next task in the set
    s32 mState : 8;         // 0x08
    u8 mUnkC : 8;           // 0x0C: (MTaskSet)
    u8 mExitWhenEmpty : 8;  // 0x0D: (MTaskSet) exit once the set runs out of tasks
    u8 mUseMutex : 8;       // 0x0E: (MTaskSet) lock the set while it's changed
    u8 mInSet : 8;          // 0x0F
};

class MTaskSet : public MTask {
public:
    MTaskSet() {
        OS_InitMutex(&mMutex);
        mUnkC = FALSE;
        mExitWhenEmpty = FALSE;
        mUseMutex = FALSE;
        mHead = NULL;
        mTail = &mHead;
    }
    virtual ~MTaskSet() = 0;
    virtual void Execute();

    void AddHead(MTask *task);
    void AddTail(MTask *task);
    // Several at once (the arguments are evaluated last to first).
    void AddTail(MTask *a, MTask *b) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
    }
    void AddTail(MTask *a, MTask *b, MTask *c) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
        self->AddTail(f);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
        self->AddTail(f);
        self->AddTail(g);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g, MTask *h) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
        self->AddTail(f);
        self->AddTail(g);
        self->AddTail(h);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g, MTask *h, MTask *i) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
        self->AddTail(f);
        self->AddTail(g);
        self->AddTail(h);
        self->AddTail(i);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g, MTask *h, MTask *i,
                 MTask *j) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
        self->AddTail(f);
        self->AddTail(g);
        self->AddTail(h);
        self->AddTail(i);
        self->AddTail(j);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g, MTask *h, MTask *i,
                 MTask *j, MTask *k) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
        self->AddTail(f);
        self->AddTail(g);
        self->AddTail(h);
        self->AddTail(i);
        self->AddTail(j);
        self->AddTail(k);
    }
    void AddTail(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g, MTask *h, MTask *i,
                 MTask *j, MTask *k, MTask *l, MTask *m) {
        MTaskSet *self = this;
        self->AddTail(a);
        self->AddTail(b);
        self->AddTail(c);
        self->AddTail(d);
        self->AddTail(e);
        self->AddTail(f);
        self->AddTail(g);
        self->AddTail(h);
        self->AddTail(i);
        self->AddTail(j);
        self->AddTail(k);
        self->AddTail(l);
        self->AddTail(m);
    }
    void Remove(MTask *task);
    void Remove(MTask *a, MTask *b) {
        Remove(a);
        Remove(b);
    }
    void Clear();

    void SetUseMutex(BOOL use) {
        // (Through an MTask pointer: the game addresses the flag from the set.)
        MTask *self = this;
        self->mUseMutex = use;
    }
    BOOL IsEmpty() {
        MScopedLock lock(&mMutex, mUseMutex);
        return mHead == NULL;
    }

    MTask *mHead;       // 0x10
    MTask **mTail;      // 0x14: the last task's mNext, or mHead
    OSMutex mMutex;     // 0x18
};

// Runs the first task until it stops running, then the next.
class MSerialTaskSet : public MTaskSet {
public:
    MSerialTaskSet(u8 exitWhenEmpty = FALSE) { mExitWhenEmpty = exitWhenEmpty; }
    virtual void Execute();
    // AddTail, for chaining.
    MSerialTaskSet *Add(MTask *task) {
        AddTail(task);
        return this;
    }
};

// Runs every task each frame.
class MParallelTaskSet : public MTaskSet {
public:
    MParallelTaskSet(u8 exitWhenEmpty = FALSE) { mExitWhenEmpty = exitWhenEmpty; }
    virtual void Execute();
    // AddTail, for chaining.
    MParallelTaskSet *Add(MTask *task) {
        AddTail(task);
        return this;
    }
};

// Exits after mFrames + 1 frames.
class MTimer : public MTask {
public:
    MTimer(int frames) { mFrames = frames; }
    virtual ~MTimer() {}
    virtual void Execute();

    int mFrames;    // 0x10
};

// A task that does its work once, then exits.
class MTransfer : public MTask {
public:
    virtual void Execute();
    virtual void Transfer() = 0;
};

class MTaskRunTransfer : public MTransfer {
public:
    static MTask *Create(MTask *task);

    MTaskRunTransfer(MTask *task) { mTask = task; }
    virtual ~MTaskRunTransfer() {}
    virtual void Transfer();

    MTask *mTask;   // 0x10
};

class MTaskSuspendTransfer : public MTransfer {
public:
    static MTask *Create(MTask *task);

    MTaskSuspendTransfer(MTask *task) { mTask = task; }
    virtual ~MTaskSuspendTransfer() {}
    virtual void Transfer();

    MTask *mTask;   // 0x10
};

class MTaskExitTransfer : public MTransfer {
public:
    static MTask *Create(MTask *task);

    MTaskExitTransfer(MTask *task) { mTask = task; }
    virtual ~MTaskExitTransfer() {}
    virtual void Transfer();

    MTask *mTask;   // 0x10
};

inline MTask *MTask::CreateExitTransfer() {
    return MTaskExitTransfer::Create(this);
}

class MTaskSetMakeEmptyTask : public MTransfer {
public:
    static MTask *Create(MTaskSet *set);

    MTaskSetMakeEmptyTask(MTaskSet *set) { mSet = set; }
    virtual ~MTaskSetMakeEmptyTask() {}
    virtual void Transfer();

    MTaskSet *mSet; // 0x10
};

// Exits once the set is empty.
class MTaskSetWaitEmptyTask : public MTask {
public:
    static MTask *Create(MTaskSet *set);

    MTaskSetWaitEmptyTask(MTaskSet *set) { mSet = set; }
    virtual ~MTaskSetWaitEmptyTask() {}
    virtual void Execute();

    MTaskSet *mSet; // 0x10
};

// A task with its own serial and parallel task sets. When the serial set is empty, Idle() is called to refill it.
// Each process belongs to a display (screen), which is made current while it runs.
class MProcess : public MTask {
public:
    MProcess(MDisplay *display);
    virtual void Execute();
    virtual void Idle() = 0;

    void SetDisplay(MDisplay *display);
    MDisplay *GetDisplay();

    MSerialTaskSet *GetSerial() { return &mSerial; }
    MParallelTaskSet *GetParallel() { return &mParallel; }

    MSerialTaskSet mSerial;     // 0x10
    MParallelTaskSet mParallel; // 0x40
    MDisplay *mDisplay;         // 0x70: made current while the process runs; new processes default to the current one
};

extern MDisplay *gCurrentDisplay;

// Makes a display current for the rest of the scope.
class MDisplayScope {
public:
    MDisplayScope(MDisplay *display) {
        mSaved = gCurrentDisplay;
        gCurrentDisplay = display;
    }
    ~MDisplayScope() { gCurrentDisplay = mSaved; }

    MDisplay *mSaved;
};

#endif
