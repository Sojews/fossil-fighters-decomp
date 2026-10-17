// Tasks that call a member function once, with arguments saved when the task is made, then exit.
#ifndef M_CALL_TASK_H
#define M_CALL_TASK_H

#include "m/task.h"

template <class T, class U>
class MCallTask0 : public MProcess {
public:
    typedef void (U::*Func)();

    MCallTask0(T *obj, Func func) : MProcess(NULL) {
        mObj = obj;
        mFunc = func;
    }
    virtual void Idle() {
        (mObj->*mFunc)();
        Exit();
    }

    u32 mUnk74;     // 0x74
    T *mObj;        // 0x78
    Func mFunc;     // 0x7C
};

template <class T, class U, class A1>
class MCallTask1 : public MProcess {
public:
    typedef void (U::*Func)(A1);

    MCallTask1(T *obj, Func func, A1 a1) : MProcess(NULL) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
    }
    virtual void Idle() {
        (mObj->*mFunc)(mA1);
        Exit();
    }

    u32 mUnk74;     // 0x74
    T *mObj;        // 0x78
    Func mFunc;     // 0x7C
    A1 mA1;         // 0x84
};

template <class T, class U, class A1, class A2>
class MCallTask2 : public MProcess {
public:
    typedef void (U::*Func)(A1, A2);

    MCallTask2(T *obj, Func func, A1 a1, A2 a2) : MProcess(NULL) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
        mA2 = a2;
    }
    virtual void Idle() {
        (mObj->*mFunc)(mA1, mA2);
        Exit();
    }

    u32 mUnk74;     // 0x74
    T *mObj;        // 0x78
    Func mFunc;     // 0x7C
    A1 mA1;         // 0x84
    A2 mA2;
};

template <class T, class U, class A1, class A2, class A3>
class MCallTask3 : public MProcess {
public:
    typedef void (U::*Func)(A1, A2, A3);

    MCallTask3(T *obj, Func func, A1 a1, A2 a2, A3 a3) : MProcess(NULL) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
        mA2 = a2;
        mA3 = a3;
    }
    virtual void Idle() {
        (mObj->*mFunc)(mA1, mA2, mA3);
        Exit();
    }

    u32 mUnk74;     // 0x74
    T *mObj;        // 0x78
    Func mFunc;     // 0x7C
    A1 mA1;         // 0x84
    A2 mA2;
    A3 mA3;
};

template <class T, class U, class A1, class A2, class A3, class A4>
class MCallTask4 : public MProcess {
public:
    typedef void (U::*Func)(A1, A2, A3, A4);

    MCallTask4(T *obj, Func func, A1 a1, A2 a2, A3 a3, A4 a4) : MProcess(NULL) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
        mA2 = a2;
        mA3 = a3;
        mA4 = a4;
    }
    virtual void Idle() {
        (mObj->*mFunc)(mA1, mA2, mA3, mA4);
        Exit();
    }

    u32 mUnk74;     // 0x74
    T *mObj;        // 0x78
    Func mFunc;     // 0x7C
    A1 mA1;         // 0x84
    A2 mA2;
    A3 mA3;
    A4 mA4;
};

template <class T, class U, class A1, class A2, class A3, class A4, class A5>
class MCallTask5 : public MProcess {
public:
    typedef void (U::*Func)(A1, A2, A3, A4, A5);

    MCallTask5(T *obj, Func func, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) : MProcess(NULL) {
        mObj = obj;
        mFunc = func;
        mA1 = a1;
        mA2 = a2;
        mA3 = a3;
        mA4 = a4;
        mA5 = a5;
    }
    virtual void Idle() {
        (mObj->*mFunc)(mA1, mA2, mA3, mA4, mA5);
        Exit();
    }

    u32 mUnk74;     // 0x74
    T *mObj;        // 0x78
    Func mFunc;     // 0x7C
    A1 mA1;         // 0x84
    A2 mA2;
    A3 mA3;
    A4 mA4;
    A5 mA5;
};

template <class T, class U>
MTask *MCreateCallTask(T *obj, void (U::*func)()) {
    return new MCallTask0<T, U>(obj, func);
}

template <class T, class U, class A1>
MTask *MCreateCallTask(T *obj, void (U::*func)(A1), A1 a1) {
    return new MCallTask1<T, U, A1>(obj, func, a1);
}

template <class T, class U, class A1, class A2>
MTask *MCreateCallTask(T *obj, void (U::*func)(A1, A2), A1 a1, A2 a2) {
    return new MCallTask2<T, U, A1, A2>(obj, func, a1, a2);
}

template <class T, class U, class A1, class A2, class A3>
MTask *MCreateCallTask(T *obj, void (U::*func)(A1, A2, A3), A1 a1, A2 a2, A3 a3) {
    return new MCallTask3<T, U, A1, A2, A3>(obj, func, a1, a2, a3);
}

template <class T, class U, class A1, class A2, class A3, class A4>
MTask *MCreateCallTask(T *obj, void (U::*func)(A1, A2, A3, A4), A1 a1, A2 a2, A3 a3, A4 a4) {
    return new MCallTask4<T, U, A1, A2, A3, A4>(obj, func, a1, a2, a3, a4);
}

template <class T, class U, class A1, class A2, class A3, class A4, class A5>
MTask *MCreateCallTask(T *obj, void (U::*func)(A1, A2, A3, A4, A5), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
    return new MCallTask5<T, U, A1, A2, A3, A4, A5>(obj, func, a1, a2, a3, a4, a5);
}

#endif
