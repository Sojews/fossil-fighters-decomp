// SetToturialFlagTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_toturial_flag_task.h"
#include "types.h"

SetToturialFlagTask::~SetToturialFlagTask() {
}

void SetToturialFlagTask::Execute() {
    mTutorial->mFlags |= mFlag;
    Exit();
}
