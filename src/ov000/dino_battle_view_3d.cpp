// DinoBattleView3d.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_3d.h"
#include "types.h"
#include "game/b3d_status_view_task.h"
#include "game/dino_battle_view_3d2.h"
#include "game/black_in_task.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_battle_view_3d_move.h"

DinoBattleView3D::~DinoBattleView3D() {
}

B3DStatusViewTask::~B3DStatusViewTask() {
    mSerial.Clear();
}

LoadWinnerLoad::~LoadWinnerLoad() {
    mSerial.Clear();
}

void B3DStatusViewTask::Idle() {
    (this->*mState)();
}

DinoBattleView3DDamage::~DinoBattleView3DDamage() {
}

DinoBattleView3DAction::~DinoBattleView3DAction() {
}

DinoBattleView3DMask::~DinoBattleView3DMask() {
}

DinoBattleView3DGroupFade::~DinoBattleView3DGroupFade() {
}

BlackInTask::~BlackInTask() {
}

BlackOutTask::~BlackOutTask() {
}

MValChange::~MValChange() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBattleView3DAction>;
template MTask *MCreateBgCallTask(DinoBattleView3DAction *obj, void (DinoBattleView3DAction::*func)(), int thread);
template class MBgCallTask1<DinoBattleView3DGroupFade, int>;
template MTask *MCreateBgCallTask(DinoBattleView3DGroupFade *obj, void (DinoBattleView3DGroupFade::*func)(int), int a0, int thread);
template class MBgCallTask1<DinoBattleView3DDamage, int>;
template MTask *MCreateBgCallTask(DinoBattleView3DDamage *obj, void (DinoBattleView3DDamage::*func)(int), int a0, int thread);
template class MBgCallTask1<LoadWinnerLoad, int>;
template MTask *MCreateBgCallTask(LoadWinnerLoad *obj, void (LoadWinnerLoad::*func)(int), int a0, int thread);

DinoBattleView3DMove::~DinoBattleView3DMove() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
