// DinoCleaningDrill.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_drill.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoCleaningDrill::~DinoCleaningDrill() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoCleaningDrill>;
template MTask *MCreateBgCallTask(DinoCleaningDrill *obj, void (DinoCleaningDrill::*func)(), int thread);
