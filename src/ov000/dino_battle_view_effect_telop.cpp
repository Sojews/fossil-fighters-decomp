// DinoBattleViewEffectTelop.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/input.h"
#include "game/dino_battle_view_effect_telop.h"
#include "types.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_static_init.h"
#include "game/dino_sound.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/lcd.h"

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
    u8 unk00[0x10];
    void (DinoBattleViewEffectTelop::*Load_10)();
    void (DinoBattleViewEffectTelop::*State_18)();
    u8 unk20[0x40 - 0x20];
    void (DinoBattleViewEffectTelop::*Start_40)();
    u8 unk48[0x58 - 0x48];
    void (DinoBattleViewEffectTelop::*State_58)();
    u8 unk60[0x70 - 0x60];
    void (DinoBattleViewEffectTelop::*State_70)();
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
#pragma auto_inline off
template class MBgCallTask0<DinoBattleViewEffectTelop>;
template MTask *MCreateBgCallTask(DinoBattleViewEffectTelop *obj, void (DinoBattleViewEffectTelop::*func)(), int thread);
#pragma auto_inline reset

#pragma auto_inline off

// Starts the telop: loads it in the background, then goes on by its kind (with a sound for most).
void DinoBattleViewEffectTelop::StateStart() {
    MTaskSet *parallel = &mParallel;
    parallel->mUseMutex = TRUE;
    MTaskSet *serial = &mSerial;
    serial->mUseMutex = TRUE;
    gLcd->mSub->mBg->SetMode(3);
    mSerial.AddTail(MCreateBgCallTask(this, data_ov000_0219387c.Load_10, 1));
    switch (mUnk98) {
    case 4:
        mState = data_ov000_0219387c.State_58;
        return;
    case 5:
        mState = data_ov000_0219387c.State_18;
        return;
    }
    mSerial.AddTail(DinoSoundCreatePlaySeTask(0x52, 0));
    mState = data_ov000_0219387c.State_70;
}

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

// The file's static initialization (see dino_static_init.h).
static struct DinoBattleViewEffectTelopStaticInit {
    DinoBattleViewEffectTelopStaticInit() {
        DINO_POOL_WORD(data_ov000_0219387c, 0x38) = 128;
        DINO_POOL_WORD(data_ov000_0219387c, 0x3c) = 176;
        DINO_POOL_WORD(data_ov000_0219387c, 0x48) = 8;
        DINO_POOL_WORD(data_ov000_0219387c, 0x4c) = 150;
        DINO_POOL_WORD(data_ov000_0219387c, 0x68) = 64;
        DINO_POOL_WORD(data_ov000_0219387c, 0x6c) = 112;
    }
} sDinoBattleViewEffectTelopStaticInit;
