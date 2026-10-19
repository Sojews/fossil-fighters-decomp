// DinoBattleView3d.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_sound.h"
#include "game/dino_battle_view_3d.h"
#include "game/dino_3d_load_csv.h"
#include "game/dino_battle_view_class.h"
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
    mSerial.Clear();
    if (mModel->GetUnk180()) {
        DinoSoundFunc_0201feb4(0);
    }
}

DinoBattleView3DAction::~DinoBattleView3DAction() {
    mSerial.Clear();
    if (mModel->GetUnk180()) {
        DinoSoundFunc_0201feb4(0);
    }
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

void DinoBattleView3DMask::Execute() {
    for (int i = 0; i < 3; i++) {
        if (mMask & (1 << i)) {
            mView->mModels[i]->Push7(0x1000);
        } else {
            mView->mModels[i]->Push7(0);
        }
    }
    Exit();
}

#pragma auto_inline off

void DinoBattleView3D::GetUnk19C(u32 *a, u32 *b) {
    *a = mUnk19C;
    *b = mUnk1A0;
}

void DinoBattleView3D::SetUnk19C(u32 value) {
    mUnk19C = value;
}

u32 DinoBattleView3D::GetUnkB0() {
    return mUnkB0;
}

DelDmgMotionTask::~DelDmgMotionTask() {
    delete mUnk10;
}

void DelDmgMotionTask::Execute() {
    Exit();
}

#pragma auto_inline off

void DinoBattleView3D::SetUnk180(u8 value) {
    mUnk180 = value;
    if (value && !mView->GetUnk138()) {
        DinoSoundFunc_0201feb4(0);
    }
}

u8 DinoBattleView3D::GetUnk180() {
    if (mView->GetUnk138()) {
        return FALSE;
    }
    return mUnk180;
}

// A 16-byte-element vector the game emits members of in this file.
#pragma auto_inline off
template class std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >;

// A 52-byte-element vector the game appends to in this file.
template void std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::append_realloc(size_t n, const DinoBattleView3DRecord &x);

// A vector of string lists (its elements are destroyed with vector<string>'s out-of-line destructor).
// NONMATCHING (both erase_at_end, callcheck only): our vector<string> destructor is inlined into a
// __vec_deleter<string> D2 call, so the bytes match but the call target differs (see the vector<string> note).
template class std::__vec_deleter<std::vector<std::string>, std::allocator<std::vector<std::string> > >;

#pragma auto_inline off

// The 3D processes' states (called through pointers to members).
extern "C" void func_ov000_02184cf0(DinoBattleView3DGroupFade *fade, int i) {
    fade->mTarget->mModels[i]->LoadByName(fade->mNames[i], TRUE, 0x0FFFFFFF);
}

extern "C" void func_ov000_02184d14(DinoBattleView3DAction *action) {
    action->mCsv = new Dino3DLoadCsv(action->mUnk8C + action->mUnk88 * 10, action->mUnk78, action->mUnk98, (void *)5,
                                     action->mUnk84);
    action->mParallel.AddTail(action->mCsv);
}

extern "C" void func_ov000_02184d64(DinoBattleView3DDamage *damage, int i) {
    damage->mCsvs[i]->LoadByName(damage->mNames[i], TRUE, 0x0FFFFFFF);
    if (damage->mFiles[i] != NULL) {
        damage->mCsvs[i]->LoadFile(damage->mFiles[i], NULL, 0);
    }
}

extern "C" void func_ov000_02184da0(DinoBattleView3DDamage *damage, int i) {
    damage->mCsvs[i]->Load(damage->mDefs->mPaths[i], (u32)damage->mNames[i]);
    if (damage->mFiles[i] != NULL) {
        damage->mCsvs[i]->LoadFile(damage->mFiles[i], NULL, 0);
    }
}

// A model free in the slot (the current one for -1): the last with nothing on it.
extern "C" Dino3DLoadCsv *func_ov000_02187e68(DinoBattleView3D *view, int slot) {
    Dino3DLoadCsv *model = NULL;
    if (slot < 0) {
        slot = view->mSlot;
    }
    for (int i = 0; i < 3; i++) {
        if (view->mSlotModels[slot * 3 + i] == NULL) {
            model = view->mModels[i];
        }
    }
    return model;
}

// Zeroes three words (nothing in the game calls it).
extern "C" void func_ov000_021858ac(u32 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
}
