// DinoCommonWindowMenuItemFlushTask.
// Only the destructors are decompiled so far.

#include "game/dino_common_window_menu_item_flush_task.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoCommonWindowMenuItemFlushTask::~DinoCommonWindowMenuItemFlushTask() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoCommonWindowMenu : public DinoHelpBase {};
template class MCallTask0<DinoCommonWindowMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoCommonWindowMenu *obj, void (DinoHelpBase::*func)());
