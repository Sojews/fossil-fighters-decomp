// SetVarTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_var_task.h"
#include "types.h"
#include "game/dino_battle2.h"

SetVarTask::~SetVarTask() {
}

DinoBattleLiveConsoleFilter::~DinoBattleLiveConsoleFilter() {
}

void SetVarTask::Execute() {
    *mDst = mValue;
    Exit();
}

#pragma auto_inline off

void DinoBattleTalkWindow::SetUnk84(u32 value) {
    mUnk84 = value;
}
