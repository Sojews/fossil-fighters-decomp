// Blocking interrupts for a short while.
#ifndef M_IRQ_H
#define M_IRQ_H

#include "types.h"

#define REG_IME (*(volatile u16 *)0x04000208)

// Disables interrupts for the rest of the scope.
class MIrqLock {
public:
    MIrqLock() {
        u16 prev = REG_IME;
        REG_IME = 0;
        mEnabled = prev;
    }
    ~MIrqLock() {
        u16 prev = REG_IME;
        REG_IME = mEnabled;
    }

    bool mEnabled;
};

class MTask;

// Adds a task to (or removes it from) the tasks run in one interrupt.
void MAddIntrTask(int intr, MTask *task);
void MRemoveIntrTask(int intr, MTask *task);

#endif
