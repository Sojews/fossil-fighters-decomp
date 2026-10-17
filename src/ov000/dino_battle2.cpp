// DinoBattle2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle2.h"
#include "types.h"

extern "C" void func_ov000_0218b928(void *, u8);

DinoBattleViewFocusLCDFrame::~DinoBattleViewFocusLCDFrame() {
}

DBViewFocusLCDFrameTask::~DBViewFocusLCDFrameTask() {
}

DinoBattleTalkWindow::~DinoBattleTalkWindow() {
}

void DinoBattleViewFocusLCDFrame::Idle() {
}

void DBViewFocusLCDFrameTask::Execute() {
    func_ov000_0218b928((void *)mUnk10, mUnk14);
    Exit();
}
