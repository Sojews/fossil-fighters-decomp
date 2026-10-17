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

// A background task that calls a member function on a worker thread.
template <class T>
class MBgCallTask0 : public MBgTask {
public:
    typedef void (T::*Func)();

    MBgCallTask0(T *obj, Func func, int thread) : MBgTask(NULL, thread) {
        mObj = obj;
        mFunc = func;
    }
    virtual ~MBgCallTask0() { Cancel(); }
    virtual void Run() { (mObj->*mFunc)(); }

    T *mObj;        // 0x34
    Func mFunc;     // 0x38
};

template <class T, class A1>
class MBgCallTask1 : public MBgTask {
public:
    typedef void (T::*Func)(A1);

    MBgCallTask1(T *obj, Func func, A1 a1, int thread) : MBgTask(NULL, thread) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
    }
    virtual ~MBgCallTask1() { Cancel(); }
    virtual void Run() { (mObj->*mFunc)(mA1); }

    T *mObj;        // 0x34
    Func mFunc;     // 0x38
    A1 mA1;         // 0x40
};

template <class T>
MTask *MCreateBgCallTask(T *obj, void (T::*func)(), int thread) {
    return new MBgCallTask0<T>(obj, func, thread);
}

template <class T, class A1>
MTask *MCreateBgCallTask(T *obj, void (T::*func)(A1), A1 a1, int thread) {
    return new MBgCallTask1<T, A1>(obj, func, a1, thread);
}

#endif
