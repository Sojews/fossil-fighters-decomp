// DinoCleaningHammer, DinoCleaningHammerEffect, DinoCleaningHammerTask, DinoCleaningHammerBaseHit.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_hammer.h"
#include "game/dino_cleaning_hammer_effect.h"
#include "game/dino_cleaning_hammer_task.h"
#include "game/dino_cleaning_hammer_base_hit.h"
#include "game/dino_cleaning_drill.h"
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
    u8 unk00[0x8];
    MFx32 mRadius;          // 0x08: the ring's radius
    u8 unk0C[0x10 - 0xC];
    MFx32 mBigRadius;       // 0x10
    u8 unk14[0x30 - 0x14];
    void (DinoCleaningHammerEffect::*StateEnd_30)();
    u8 unk38[0x40 - 0x38];
    void (DinoCleaningHammerEffect::*State0215caf8_40)(); // 0x40
} data_ov002_02164fb8;

// A window ring, through which the hit shows, grows to the radius.
// NONMATCHING: the fixed-point temporaries sit in other stack slots
void DinoCleaningHammerEffect::StateStart() {
    mWindow = new MRasterWindow(-1, NULL, TRUE);
    mWindow->SetTarget(0);
    mWindow->SetObjTarget(0x3F);
    mWindow->SetBlendEnabled(TRUE);
    mWindow->SetObjBlendEnabled(TRUE);
    mWindow->Show();
    mFrame = 0;
    MFx32 speed = mSpeed;
    MFx32 radius;
    if (mBig) {
        radius = data_ov002_02164fb8.mBigRadius;
    } else {
        radius = data_ov002_02164fb8.mRadius;
    }
    mFrames = MFx32ToInt(radius / speed) - 1;
    mState = data_ov002_02164fb8.State0215caf8_40;
}

void DinoCleaningHammerEffect::StateClear() {
    mWindow->Clear();
    mState = data_ov002_02164fb8.StateEnd_30;
}
