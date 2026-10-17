// DinoBattleViewKasekiPoint, WaitKPCountTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_kaseki_point.h"
#include "game/wait_kp_count_task.h"

DinoBattleViewKasekiPoint::~DinoBattleViewKasekiPoint() {
}

WaitKPCountTask::~WaitKPCountTask() {
}

DinoBattleViewPlayer::~DinoBattleViewPlayer() {
}

void DinoBattleViewPlayer::Idle() {
    Dispatch();
}
