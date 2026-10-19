// WaitActionInfoBarTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/wait_action_info_bar_task.h"
#include "types.h"
#include "game/dino_resource.h"

WaitActionInfoBarTask::WaitActionInfoBarTask(DinoBattleViewActionInfoBar *bar) {
    mBar = bar;
}

WaitActionInfoBarTask::~WaitActionInfoBarTask() {
}

#pragma auto_inline off

extern "C" void func_ov000_0218b4dc(DinoBattleViewActionInfoBar *bar, const char *text);

// Lists the action's text in the bar (when the defs ask for it).
extern "C" void func_ov000_0218b4c0(DinoBattleViewActionInfoBar *bar, u32 textId) {
    func_ov000_0218b4dc(bar, gDinoResource->GetText(textId));
}

extern "C" void func_ov000_0218b4dc(DinoBattleViewActionInfoBar *bar, const char *text) {
    if (gDinoResource->GetBattleAdjustDefs()->actionLog) {
        bar->mLines.push_back(std::string(text));
    }
}

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
