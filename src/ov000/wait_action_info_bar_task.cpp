// WaitActionInfoBarTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/wait_action_info_bar_task.h"
#include "types.h"

WaitActionInfoBarTask::WaitActionInfoBarTask(DinoBattleViewActionInfoBar *bar) {
    mBar = bar;
}

WaitActionInfoBarTask::~WaitActionInfoBarTask() {
}

#pragma auto_inline off

BOOL DinoBattleViewActionInfoBar::IsIdle() {
    if (mUnk80 == 0) {
        return TRUE;
    }
    return FALSE;
}

#pragma auto_inline on

void WaitActionInfoBarTask::Execute() {
    if (mBar->IsIdle() && mBar->mLines.empty()) {
        Exit();
    }
}
