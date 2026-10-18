// A background task that makes an object (whose constructor loads files) on a worker thread.
#ifndef M_BG_NEW_TASK_H
#define M_BG_NEW_TASK_H

#include "m/bg_task.h"

template <class T, class A1>
class MBgNewTask1 : public MBgTask {
public:
    MBgNewTask1(T **result, A1 a1, int thread) : MBgTask(NULL, thread), mResult(result), mA1(a1) {}
    virtual ~MBgNewTask1() { Cancel(); }
    virtual void Run() { *mResult = new T(mA1); }

    T **mResult;    // 0x34: gets the object
    A1 mA1;         // 0x38
};

template <class T, class A1>
MTask *MCreateBgNewTask(T **result, A1 a1, int thread) {
    return new MBgNewTask1<T, A1>(result, a1, thread);
}

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

template <class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
class MBgCallTask7 : public MBgTask {
public:
    typedef void (T::*Func)(A1, A2, A3, A4, A5, A6, A7);

    MBgCallTask7(T *obj, Func func, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, int thread) : MBgTask(NULL, thread) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
        mA2 = a2;
        mA3 = a3;
        mA4 = a4;
        mA5 = a5;
        mA6 = a6;
        mA7 = a7;
    }
    virtual ~MBgCallTask7() { Cancel(); }
    virtual void Run() { (mObj->*mFunc)(mA1, mA2, mA3, mA4, mA5, mA6, mA7); }

    T *mObj;        // 0x34
    Func mFunc;     // 0x38
    A1 mA1;
    A2 mA2;
    A3 mA3;
    A4 mA4;
    A5 mA5;
    A6 mA6;
    A7 mA7;
};

template <class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
MTask *MCreateBgCallTask(T *obj, void (T::*func)(A1, A2, A3, A4, A5, A6, A7), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, int thread) {
    return new MBgCallTask7<T, A1, A2, A3, A4, A5, A6, A7>(obj, func, a1, a2, a3, a4, a5, a6, a7, thread);
}

template <class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class MBgCallTask8 : public MBgTask {
public:
    typedef void (T::*Func)(A1, A2, A3, A4, A5, A6, A7, A8);

    MBgCallTask8(T *obj, Func func, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, int thread) : MBgTask(NULL, thread) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
        mA2 = a2;
        mA3 = a3;
        mA4 = a4;
        mA5 = a5;
        mA6 = a6;
        mA7 = a7;
        mA8 = a8;
    }
    virtual ~MBgCallTask8() { Cancel(); }
    virtual void Run() { (mObj->*mFunc)(mA1, mA2, mA3, mA4, mA5, mA6, mA7, mA8); }

    T *mObj;        // 0x34
    Func mFunc;     // 0x38
    A1 mA1;
    A2 mA2;
    A3 mA3;
    A4 mA4;
    A5 mA5;
    A6 mA6;
    A7 mA7;
    A8 mA8;
};

template <class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
MTask *MCreateBgCallTask(T *obj, void (T::*func)(A1, A2, A3, A4, A5, A6, A7, A8), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, int thread) {
    return new MBgCallTask8<T, A1, A2, A3, A4, A5, A6, A7, A8>(obj, func, a1, a2, a3, a4, a5, a6, a7, a8, thread);
}

#endif
