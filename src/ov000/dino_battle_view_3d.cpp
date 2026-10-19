// DinoBattleView3d.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_sound.h"
#include <printf.h>
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
#include "game/camera_load_task.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "m/3d_scene.h"

DinoBattleView3D::~DinoBattleView3D() {
    CancelQueue();
    mUnkC0->Clear();
    if (mUnkE4 != NULL) {
        mParallel.Remove(mUnkE4);
        delete mUnkE4;
        mUnkE4 = NULL;
    }
    if (mUnkE8 != NULL) {
        mParallel.Remove(mUnkE8);
        delete mUnkE8;
        mUnkE8 = NULL;
    }
    mUnkCC->Clear();
}

B3DStatusViewTask::~B3DStatusViewTask() {
    mSerial.Clear();
}

LoadWinnerLoad::~LoadWinnerLoad() {
    mSerial.Clear();
}

extern "C" const char data_ov000_021965c0[];   // "din%03d_%02d"
extern "C" const VecFx32 data_ov000_02190334, data_ov000_0219034c;

// Loads the slot's i-th creature's winner model into the view's i-th model.
void LoadWinnerLoad::LoadModel(int i) {
    for (int j = 0; j < 3; j++) {
        mView->mModels[j]->Reset();
    }
    char name[64];
    sprintf(name, data_ov000_021965c0, mView->mCreatureIds[mSlot * 3 + i], 4);
    mView->mModels[i]->LoadByName(name, TRUE, 0x0FFFFFFF);
    mView->mModels[i]->PushVec3(&data_ov000_02190334);
    mView->mModels[i]->PushVec2(&data_ov000_0219034c);
    if (mView->mUnk14A[mSlot * 3 + i] == 0) {
        mView->mModels[i]->Push11();
    } else {
        mView->mModels[i]->Push12();
    }
    mView->mModels[i]->Push7(0x1000);
}

void B3DStatusViewTask::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (set up at startup).
extern "C" struct {
    u8 unk00[0x4C];
    void (B3DStatusViewTask::*State_4C)();
    u8 unk54[0x5C - 0x54];
    void (B3DStatusViewTask::*State_5C)();
    u8 unk64[0x6C - 0x64];
    void (B3DStatusViewTask::*State_6C)();
    void (B3DStatusViewTask::*State_74)();
    u8 unk7C[0xD4 - 0x7C];
    void (B3DStatusViewTask::*State_D4)();
    u8 unkDC[0xEC - 0xDC];
    void (B3DStatusViewTask::*State_EC)();
} data_ov000_021962e8;

extern "C" MTask *func_ov000_0218abd0(void *camera, int light);
extern "C" MTask *func_ov000_0218ac04(void *camera, void *arg1, const VecFx32 *arg2, BOOL arg3);
extern "C" MTask *func_ov000_0218ac40(void *camera, u8 pause);
extern "C" MTask *func_ov000_0218ac74(void *camera, Dino3DLoadCsv *parent);

// Points the camera at the status's creature and shows it.
void B3DStatusViewTask::StateShow() {
    u32 next;
    mView->GetUnk19C(&mUnk7C, &next);
    if (next == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (next != 0 && (next >> 16) == (mUnk80 >> 16)) {
        mUnk80 = next;
    }
    mUnk84 = mView->mView->GetUnkE4(mUnk80 >> 16)->GetUnkAC();
    int i = (u16)mUnk80 - 1;
    MTask *unmask, *hide, *pause, *change, *parent, *light, *mask, *show;
    show = ((M3DScene *)mView->GetUnkB0())->CreateEnableTask(TRUE);
    mask = new DinoBattleView3DMask((DinoBattleView3DMask::View *)mView, 1 << i);
    light = func_ov000_0218abd0(mView->mUnkE8, -1);
    parent = func_ov000_0218ac74(mView->mUnkE8, mView->mModels[i]);
    change = func_ov000_0218ac04(mView->mUnkE8, (u8 *)mView + 0x1E0, &data_ov000_02190334, mUnk84 == 0);
    pause = func_ov000_0218ac40(mView->mUnkE8, 0);
    hide = ((M3DScene *)mView->GetUnkB0())->CreateEnableTask(FALSE);
    unmask = new DinoBattleView3DMask((DinoBattleView3DMask::View *)mView, 0);
    MSerialTaskSet *serial = GetSerial();
    serial->AddTail(unmask);
    serial->AddTail(hide);
    serial->AddTail(pause);
    serial->AddTail(change);
    serial->AddTail(parent);
    serial->AddTail(light);
    serial->AddTail(mask);
    serial->AddTail(show);
    mState = data_ov000_021962e8.State_4C;
}

// Fades the status's creature group in.
void B3DStatusViewTask::StateFade() {
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    mUnk84 = mView->mView->GetUnkE4(mUnk80 >> 16)->GetUnkAC();
    int i = (u16)mUnk80 - 1;
    GetSerial()->AddTail(new DinoBattleView3DMask((DinoBattleView3DMask::View *)mView, 1 << i),
                         func_ov000_0218ac04(mView->mUnkE8, (u8 *)mView + 0x1E0, &data_ov000_02190334, mUnk84 == 0),
                         new DinoBattleView3DGroupFade(mView->mUnkE8, (DinoBattleView3DGroupFade::Target *)mView,
                                                       mUnk84, 0, i));
    mState = data_ov000_021962e8.State_5C;
}

// Goes on to the next status: within the same group the cameras just resume, else the group fades.
// NONMATCHING: 98%, the fade's duration is loaded later than in the game (scheduling).
void B3DStatusViewTask::StateNext() {
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if ((mUnk80 >> 16) == (mUnk7C >> 16)) {
        GetSerial()->AddTail(func_ov000_0218ac40(mView->mUnkE8, 1), func_ov000_0218ac40(mView->mUnkE4, 1));
        mState = data_ov000_021962e8.State_EC;
        return;
    }
    mUnk84 = mView->mView->GetUnkE4(mUnk80 >> 16)->GetUnkAC();
    GetSerial()->AddTail(new DinoBattleView3DGroupFade(mView->mUnkE4, (DinoBattleView3DGroupFade::Target *)mView,
                                                       mUnk84 == 0, 1, -1),
                         func_ov000_0218ac40(mView->mUnkE4, 1), func_ov000_0218ac40(mView->mUnkE8, 0));
    mState = data_ov000_021962e8.State_4C;
}

// Shows the view's current status, then goes on to the next (fading in between different ones).
void B3DStatusViewTask::StateAdvance() {
    mView->SetUnk19C(mUnk80);
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (mUnk7C != mUnk80) {
        mSerial.AddTail(new BlackInTask());
        mState = data_ov000_021962e8.State_6C;
        return;
    }
    Exit();
}

void B3DStatusViewTask::StateCheck() {
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (mUnk7C == mUnk80) {
        mState = data_ov000_021962e8.State_D4;
        return;
    }
    mSerial.AddTail(new BlackInTask());
    mState = data_ov000_021962e8.State_74;
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

// Deletes the tasks waiting to run.
void DinoBattleView3D::CancelQueue() {
    while (!mQueue.empty()) {
        MTask *task = mQueue.front();
        mQueue.pop_front();
        delete task;
    }
    mUnk19C = -1;
    mUnk1A0 = -1;
}

extern "C" u32 data_0208a0c0;
extern "C" u8 data_ov000_02196cb4[];
extern "C" Dino3DLoadCsv *func_ov000_02187e68(DinoBattleView3D *view, int slot);

// Shows something on the slot's models: the camera follows a free model, and (on the current side) one of the
// models moves.
MTask *DinoBattleView3D::CreateMove(int unkA, u32 shown, int slot) {
    mSlotModels[slot] = (void *)shown;
    Dino3DLoadCsv *model = func_ov000_02187e68(this, -1);
    CameraSetDefParentTask::Camera *camera = (CameraSetDefParentTask::Camera *)mUnkE4;
    camera->mParent = model;
    camera->mUnk7C->SetParent(camera->mParent);
    if (slot / 3 == mSlot) {
        MSerialTaskSet *set = new MSerialTaskSet();
        DinoBattleView3DMoveParams params;
        params.unk0 = mCreatureIds[slot];
        params.unk4 = 8;
        params.unk10 = 0x7FFF;
        params.unk8 = 0;
        params.unkA = unkA;
        params.unkC = shown;
        params.unkE = 20;
        set->mExitWhenEmpty = TRUE;
        MTask *move = new DinoBattleView3DMove(&params, mUnkB0, mModels[slot % 3], NULL, data_ov000_02196cb4, 0);
        set->AddTail(func_ov000_0218addc(data_0208a0c0));
        set->AddTail(move);
        return set;
    }
    return new MTimer(1);
}

// Starts the next waiting task once the task set is idle.
void DinoBattleView3D::RunNextQueued() {
    if (!mQueue.empty() && mUnkC0->IsEmpty()) {
        MTask *task = mQueue.front();
        mQueue.pop_front();
        mUnkC0->AddTail(task);
    }
}

// Hands the tasks waiting to run to the task set.
void DinoBattleView3D::FlushQueue() {
    while (!mQueue.empty()) {
        MTask *task = mQueue.front();
        mQueue.pop_front();
        mUnkC0->AddTail(task);
    }
}

// The queue's members the game emits in this file.
template std::__deque_deleter<MTask *, std::allocator<MTask *> >::~__deque_deleter();
template std::__deque_deleter<MTask *, std::allocator<MTask *> >::iterator std::__deque_deleter<MTask *, std::allocator<MTask *> >::make_iter(size_t n);
template void std::__deque_deleter<MTask *, std::allocator<MTask *> >::push_back(MTask *const &x, Metrowerks::int2type<true>);
template void std::__deque_deleter<MTask *, std::allocator<MTask *> >::erase_front(size_t n, Metrowerks::int2type<true>);
