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
    mSerial.Clear();
    mTarget->mUnk13C = mSaved;
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

void MValChange::Execute() {
    *mDst = mValue;
    Exit();
}

extern "C" MTask *func_ov000_0218ae50(u32 arg);
extern "C" MTask *func_ov000_0218addc(u32 arg);

void BlackInTask::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = func_ov000_0218ae50(mUnk74);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

void BlackOutTask::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = func_ov000_0218addc(mUnk74);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

void DinoBattleView3D::Idle() {
    MMessageObserver::Dispatch();
    (this->*mState)();
}

extern "C" void func_0202b350(void *model, int alpha);

void DinoBattleView3DMask::Execute() {
    for (int i = 0; i < 3; i++) {
        if (mMask & (1 << i)) {
            func_0202b350(mView->mModels[i], 0x1000);
        } else {
            func_0202b350(mView->mModels[i], 0);
        }
    }
    Exit();
}
