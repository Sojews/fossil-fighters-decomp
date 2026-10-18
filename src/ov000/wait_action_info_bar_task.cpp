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

extern "C" BOOL func_ov000_0218b7f0(DinoBattleViewActionInfoBar *bar);

void WaitActionInfoBarTask::Execute() {
    if (func_ov000_0218b7f0(mBar) && mBar->mLines.empty()) {
        Exit();
    }
}
