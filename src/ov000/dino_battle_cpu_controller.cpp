// DinoBattleCpuController.

#include "game/set_live_event_check_task.h"
#include "game/dino_battle_controller.h"
#include "types.h"
#include "game/dino_battle_cpu_controller.h"
#include "game/dino_common_live.h"

#pragma auto_inline off

BOOL DinoBattleController::Unk10() {
    return FALSE;
}

BOOL DinoBattleController::Unk14() {
    return FALSE;
}


void SetLiveEventCheckTask::Execute() {
    if (mLive != NULL) {
        if (!mCheck) {
            mLive->CloseIfOpen();
        }
        mLive->SetUnkD2(mCheck);
    }
    Exit();
}

#pragma auto_inline on

DinoBattleCpuController::~DinoBattleCpuController() {
}

SetLiveEventCheckTask::~SetLiveEventCheckTask() {
}

#pragma auto_inline off



// The list's vector members the game emits in this file.
#pragma auto_inline off
template class std::__vec_deleter<DinoBattleCpuEntry, std::allocator<DinoBattleCpuEntry> >;
