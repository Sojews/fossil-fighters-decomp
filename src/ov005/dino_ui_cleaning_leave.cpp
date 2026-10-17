// DinoUiCleaningLeave.
// Only the destructors are decompiled so far.

#include "game/dino_ui_cleaning_leave.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiCleaningLeave::~DinoUiCleaningLeave() {
}

void DinoUiCleaningLeave::Idle() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiCleaningLeaveMenu {};
template class MCallTask0<DinoUiCleaningLeaveMenu, DinoUiCleaningLeaveMenu>;
template MTask *MCreateCallTask(DinoUiCleaningLeaveMenu *obj, void (DinoUiCleaningLeaveMenu::*func)());
