// CreatureMotion.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/creature_motion.h"
#include "types.h"
#include "game/dino_battle_view_creature.h"
#include "game/view_status_effect_icon.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/geometry.h"
#include "m/motion.h"
#include "game/dino_battle_view_lifebar.h"
#include "game/dino_battle_view_effect_game_begin.h"

CreatureMotion::~CreatureMotion() {
}

NopTask::~NopTask() {
}

CreateCreatureMotionTask::~CreateCreatureMotionTask() {
}

EffectMotionTask::~EffectMotionTask() {
}

SetCretureIconTask::~SetCretureIconTask() {
}

SetCretureIconPriorityTask::~SetCretureIconPriorityTask() {
}

UpdateLifeBarTask::~UpdateLifeBarTask() {
}

UpdateLifePointTask::~UpdateLifePointTask() {
}

SetColorMaskTask::~SetColorMaskTask() {
}

ShuffleMoveTask::~ShuffleMoveTask() {
}

void EffectMotionTask::Idle() {
}

void NopTask::Execute() {
}

DinoBattleViewCreature::SetCoordTask::~SetCoordTask() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoBattleViewCreature, DinoBattleViewCreature>;
template MTask *MCreateCallTask(DinoBattleViewCreature *obj, void (DinoBattleViewCreature::*func)());
template class MCallTask1<DinoBattleViewStatusEffectIcon, DinoBattleViewStatusEffectIcon, MPoint>;
template MTask *MCreateCallTask(DinoBattleViewStatusEffectIcon *obj, void (DinoBattleViewStatusEffectIcon::*func)(MPoint), MPoint a0);

void UpdateLifeBarTask::Execute() {
    mLifebar->SetLife(mLife, mMaxLife);
    Exit();
}

DinoBattleViewEffectGameBeginPlayerNamePanel::~DinoBattleViewEffectGameBeginPlayerNamePanel() {
    mFont->Release();
    delete mUnk80;
    delete mUnk84;
}

DinoBattleViewEffectGameBegin::~DinoBattleViewEffectGameBegin() {
}

void DinoBattleViewEffectGameBegin::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.

MRect DinoBattleViewEffectGameBeginPlayerNamePanel::GetFrame() {
    MRect r = MMotion::GetFrame();
    r.right = r.left + 0x88;
    return r;
}

extern "C" void func_ov000_021728d4(DinoBattleViewCreature *view, int arg);
extern "C" void func_ov000_0216fd74(DinoBattleViewCreature *view);

void DinoBattleViewCreature::Idle() {
    MMessageObserver::Dispatch();
    func_ov000_021728d4(this, 0);
    func_ov000_0216fd74(this);
}

MPoint DinoBattleViewEffectGameBeginPlayerNamePanel::GetPosition() {
    return MMotion::GetPosition().Offset(12, 0);
}

void SetCretureIconPriorityTask::Execute() {
    (*mMotion)->SetPriority(mPriority);
    (*mMotion)->SetAnchor(mAnchor);
    Exit();
}

void SetColorMaskTask::Execute() {
    mTarget->mColorMask = mColorMask;
    Exit();
}

void CreateCreatureMotionTask::Execute() {
    (*mSlot)->Exit();
    *mSlot = new CreatureMotion(mPath.c_str(), mOam, mArg);
    func_ov000_021728d4(mView, 1);
    Exit();
}

#pragma auto_inline off

u32 DinoBattleViewCreature::GetUnkAC() {
    return mUnkAC;
}

u32 DinoBattleViewCreature::GetUnkB0() {
    return mUnkB0;
}

u32 DinoBattleViewCreature::GetUnkB4() {
    return mUnkB4;
}

u32 DinoBattleViewCreature::GetUnkA8() {
    return mUnkA8;
}

u32 DinoBattleViewCreature::GetUnkF4() {
    return mUnkF4;
}

u32 DinoBattleViewCreature::GetUnkF8() {
    return mUnkF8;
}

u32 DinoBattleViewCreature::GetUnk10C() {
    return mUnk10C;
}

u32 DinoBattleViewCreature::GetUnkFC() {
    return mUnkFC;
}

u32 DinoBattleViewCreature::GetUnk100() {
    return mUnk100;
}

u32 DinoBattleViewCreature::GetUnk104() {
    return mUnk104;
}

u32 DinoBattleViewCreature::GetUnk108() {
    return mUnk108;
}

u8 DinoBattleViewCreature::GetDefUnk0D() {
    return mDef->mUnk0D;
}

u8 DinoBattleViewCreature::GetDefUnk61() {
    return mDef->mUnk61;
}

u32 DinoBattleViewCreature::GetUnkE0() {
    return mUnkE0;
}

u32 DinoBattleViewCreature::GetUnkDC() {
    return mUnkDC;
}

u32 DinoBattleViewCreature::GetUnkD8() {
    return mUnkD8;
}

MMotion *DinoBattleViewCreature::GetMotion() {
    return mMotion;
}

u32 DinoBattleViewCreature::GetUnk118() {
    return mUnk118;
}

void DinoBattleViewCreature::SetUnk118(u32 value) {
    mUnk118 = value;
}

void DinoBattleViewCreature::SetMotionPriority(int priority) {
    mMotion->SetPriority(priority);
}

void DinoBattleViewCreature::SetMotionAnchor(int anchor) {
    mMotion->SetAnchor(anchor);
}

void DinoBattleViewCreature::PlayMotion3() {
    mMotion->Play(3, 0);
}

void DinoBattleViewCreature::SetFlag144(int bit) {
    mFlags144 |= 1 << bit;
}

void DinoBattleViewCreature::ClearFlag144(int bit) {
    mFlags144 &= ~(1 << bit);
}

#pragma auto_inline on
