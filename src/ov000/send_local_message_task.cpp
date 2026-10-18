// SendLocalMessageTask, SetLiveOneShotFlagTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/send_local_message_task_menu.h"
#include "game/send_local_message_task.h"
#include "game/dino_battle_model.h"
#include "game/set_live_one_shot_flag_task.h"

extern "C" void func_ov000_0215c174(void *, u32);

SendLocalMessageTask::~SendLocalMessageTask() {
}

SetLiveOneShotFlagTask::~SetLiveOneShotFlagTask() {
}

DinoBattleModelField::~DinoBattleModelField() {
}

void DinoBattleModelField::Idle() {
    Dispatch();
}

void SetLiveOneShotFlagTask::Execute() {
    func_ov000_0215c174((void *)mUnk10, mUnk14);
    Exit();
}

void SendLocalMessageTask::Execute() {
    mModel->SendLocalMessage(mMessage);
    Exit();
}
