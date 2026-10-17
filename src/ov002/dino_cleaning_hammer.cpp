// DinoCleaningHammer, DinoCleaningHammerEffect, DinoCleaningHammerTask, DinoCleaningHammerBaseHit.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_hammer.h"
#include "game/dino_cleaning_hammer_effect.h"
#include "game/dino_cleaning_hammer_task.h"
#include "game/dino_cleaning_hammer_base_hit.h"
#include "game/dino_cleaning_drill.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

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
