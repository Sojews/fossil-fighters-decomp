// BlackInTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/black_in_task.h"
#include "types.h"
#include "game/b3d_status_view_task.h"
#include "game/dino_battle_view_3d2.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

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
