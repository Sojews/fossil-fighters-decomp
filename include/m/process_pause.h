// MProcessPause: keeps a process's task sets suspended while it changes them.
#ifndef M_PROCESS_PAUSE_H
#define M_PROCESS_PAUSE_H

#include "m/task.h"
#include "m/heap.h"

// Keeps a process's task sets suspended for the rest of the scope (nested scopes count).
class MProcessPause {
public:
    MProcessPause(MProcess *process, OSMutex *mutex, int *count) {
        mProcess = process;
        mMutex = mutex;
        mCount = count;
        MMutexLock lock(mMutex);
        if (++*mCount == 1) {
            mProcess->mParallel.Suspend();
            mProcess->mSerial.Suspend();
        }
    }
    ~MProcessPause() {
        MMutexLock lock(mMutex);
        if (--*mCount == 0) {
            mProcess->mParallel.Run();
            mProcess->mSerial.Run();
        }
    }

    MProcess *mProcess;
    OSMutex *mMutex;
    int *mCount;
};

#endif
