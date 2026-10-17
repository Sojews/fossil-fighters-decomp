// EnableRootSelectTask.
// Only the destructors are decompiled so far.

#include "game/enable_root_select_task.h"

extern "C" void func_ov000_0215bbc4(void *);

EnableRootSelectTask::~EnableRootSelectTask() {
}

void EnableRootSelectTask::Execute() {
    func_ov000_0215bbc4((void *)mUnk10);
    Exit();
}
