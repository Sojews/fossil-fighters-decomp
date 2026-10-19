// DinoBattleViewEffectTelop.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/input.h"
#include "game/dino_battle_view_effect_telop.h"
#include "types.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoBattleViewEffectTelop::~DinoBattleViewEffectTelop() {
}

SetBGModeTask::~SetBGModeTask() {
}

SkipTelopTask::~SkipTelopTask() {
}

SkipWinTelopTask::~SkipWinTelopTask() {
}

SkipLoseTelopTask::~SkipLoseTelopTask() {
}

NopWaitTask::~NopWaitTask() {
}

void NopWaitTask::Execute() {
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x40];
    void (DinoBattleViewEffectTelop::*Start_40)();
} data_ov000_0219387c;

DinoBattleViewEffectTelop::DinoBattleViewEffectTelop(u32 unk7C, std::string text, u32 unk98, u32 unk8C, u32 unk90, u8 unk9C)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(text), mUnk8C(unk8C), mUnk90(unk90), mUnk98(unk98), mUnk9C(unk9C) {
    mState = data_ov000_0219387c.Start_40;
}

void DinoBattleViewEffectTelop::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBattleViewEffectTelop>;
template MTask *MCreateBgCallTask(DinoBattleViewEffectTelop *obj, void (DinoBattleViewEffectTelop::*func)(), int thread);

void SetBGModeTask::Execute() {
    mBg->SetMode(mMode);
    Exit();
}

void SkipTelopTask::Idle() {
    if (mTasks->IsEmpty()) {
        OnFinished();
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (mDelay != 0) {
        mDelay--;
        return;
    }
    if (!mNoSkip && (mInput->GetPressed() & 0x1003)) {
        OnSkipped();
        mSerial.AddTail(MTaskExitTransfer::Create(this));
    }
}
