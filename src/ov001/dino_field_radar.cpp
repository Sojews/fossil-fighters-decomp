// DinoFieldRadar.
// Only the template task instances it emits are here so far.

#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask1<DinoFieldRadar, DinoFieldRadar, bool>;
template MTask *MCreateCallTask(DinoFieldRadar *obj, void (DinoFieldRadar::*func)(bool), bool a0);
