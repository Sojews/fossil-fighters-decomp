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
