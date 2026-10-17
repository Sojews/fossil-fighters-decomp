// Ov00002156380.
// Only the destructors are decompiled so far.

#include "game/set_live_event_check_task.h"
#include "game/dino_battle_controller.h"
#include "types.h"

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
