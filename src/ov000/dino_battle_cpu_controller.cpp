// DinoBattleCpuController.

#include "game/set_live_event_check_task.h"
#include "game/dino_battle_controller.h"
#include "types.h"
#include "game/dino_battle_cpu_controller.h"

#pragma auto_inline off

BOOL DinoBattleController::Unk10() {
    return FALSE;
}

BOOL DinoBattleController::Unk14() {
    return FALSE;
}

extern "C" void func_020210f4(u32 event);
extern "C" void func_02021098(u32 event, u8 check);

void SetLiveEventCheckTask::Execute() {
    if (mEvent != 0) {
        if (!mCheck) {
            func_020210f4(mEvent);
        }
        func_02021098(mEvent, mCheck);
    }
    Exit();
}

#pragma auto_inline on

DinoBattleCpuController::~DinoBattleCpuController() {
}

SetLiveEventCheckTask::~SetLiveEventCheckTask() {
}

#pragma auto_inline off


