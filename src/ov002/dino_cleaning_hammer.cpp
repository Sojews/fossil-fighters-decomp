// DinoCleaningHammer, DinoCleaningHammerEffect, DinoCleaningHammerTask, DinoCleaningHammerBaseHit.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_hammer.h"
#include "game/dino_cleaning_hammer_effect.h"
#include "game/dino_cleaning_hammer_task.h"
#include "game/dino_cleaning_hammer_base_hit.h"
#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/display.h"
#include "m/raster.h"
#include "m/window.h"

// Done: frees the window and resets the display's windows.
void DinoCleaningHammerEffect::StateEnd() {
    delete mWindow;
    gCurrentDisplay->mWin->Reset();
    Exit();
}

DinoCleaningHammer::DinoCleaningHammer(DinoCleaning *cleaning, u8 arg1) : DinoCleaningTool(cleaning) {
    mUnk80 = arg1;
}

void DinoCleaningHammer::Unk18() {
}

void DinoCleaningHammer::Unk1C() {
}

DinoCleaningHammer::~DinoCleaningHammer() {
}

DinoCleaningHammerEffect::~DinoCleaningHammerEffect() {
}

DinoCleaningHammerTask::~DinoCleaningHammerTask() {
}

DinoCleaningHammerBaseHit::~DinoCleaningHammerBaseHit() {
}

void DinoCleaningHammerBaseHit::Idle() {
    (this->*mState)();
}

void DinoCleaningHammerTask::Idle() {
    (this->*mState)();
}

void DinoCleaningHammerEffect::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoCleaningDrill>;
template MTask *MCreateBgCallTask(DinoCleaningDrill *obj, void (DinoCleaningDrill::*func)(), int thread);

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    MFx32 mShakeDecay;      // 0x04: how much the base hit's shake dies down by each frame
    MFx32 mRadius;          // 0x08: the ring's radius
    u8 unk0C[0x10 - 0xC];
    MFx32 mBigRadius;       // 0x10
    u8 unk14[0x30 - 0x14];
    void (DinoCleaningHammerEffect::*StateEnd_30)();
    u8 unk38[0x40 - 0x38];
    void (DinoCleaningHammerEffect::*State0215caf8_40)(); // 0x40
    u8 unk48[0x60 - 0x48];
    void (DinoCleaningHammerBaseHit::*StateStart_60)(); // 0x60
    void (DinoCleaningHammerBaseHit::*State0215d0c4_68)(); // 0x68
} data_ov002_02164fb8;

// A window ring, through which the hit shows, grows to the radius.
void DinoCleaningHammerEffect::StateStart() {
    mWindow = new MRasterWindow(-1, NULL, TRUE);
    mWindow->SetTarget(0);
    mWindow->SetObjTarget(0x3F);
    mWindow->SetBlendEnabled(TRUE);
    mWindow->SetObjBlendEnabled(TRUE);
    mWindow->Show();
    mFrame = 0;
    mFrames = MFx32ToInt((mBig ? data_ov002_02164fb8.mBigRadius : data_ov002_02164fb8.mRadius) / mSpeed) - 1;
    mState = data_ov002_02164fb8.State0215caf8_40;
}

void DinoCleaningHammerEffect::StateClear() {
    mWindow->Clear();
    mState = data_ov002_02164fb8.StateEnd_30;
}

void DinoCleaningHammerBaseHit::StateStart() {
    mRadius = mBig ? data_ov002_02164fb8.mBigRadius : data_ov002_02164fb8.mRadius;
    mState = data_ov002_02164fb8.State0215d0c4_68;
}

extern "C" void func_ov002_0215fdec(DinoCleaningKasekiRock *rock);

// Shakes the rock, less and less.
// NONMATCHING: the fixed-point temporaries sit in other stack slots.
void DinoCleaningHammerBaseHit::State0215d0c4() {
    mRadius = mRadius - data_ov002_02164fb8.mShakeDecay;
    func_ov002_0215fdec(mRock);
    MFx32 amp = std::max(MFx32(0), mRadius * MFx32(3));
    mRock->SetOffset(MPoint(MFx32ToInt(amp * mUnk80->NextFraction() - amp / MFx32(2)),
                            MFx32ToInt(amp * mUnk80->NextFraction() - amp / MFx32(2))));
    if (mRadius <= MFx32(0)) {
        Exit();
    }
}
