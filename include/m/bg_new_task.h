// A background task that makes an object (whose constructor loads files) on a worker thread.
#ifndef M_BG_NEW_TASK_H
#define M_BG_NEW_TASK_H

#include "m/bg_task.h"

template <class T, class A1, class A2>
class MBgNewTask2 : public MBgTask {
public:
    MBgNewTask2(T **result, A1 a1, A2 a2, int thread) : MBgTask(NULL, thread), mResult(result), mA1(a1), mA2(a2) {}
    virtual ~MBgNewTask2() { Cancel(); }
    virtual void Run() { *mResult = new T(mA1, mA2); }

    T **mResult;    // 0x34: gets the object
    A1 mA1;         // 0x38
    A2 mA2;         // 0x3C
};

template <class T, class A1, class A2>
MTask *MCreateBgNewTask(T **result, A1 a1, A2 a2, int thread) {
    return new MBgNewTask2<T, A1, A2>(result, a1, a2, thread);
}

#endif
