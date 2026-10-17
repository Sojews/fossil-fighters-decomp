// DinoCommonWindowConsoleFilter.
// Only the destructors are decompiled so far.

#include "game/dino_common_window_console_filter.h"
#include "game/dino_help_disabler_task.h"
#include "game/common_window_message.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoCommonWindowConsoleFilter::~DinoCommonWindowConsoleFilter() {
}

DinoHelpDisablerTask::~DinoHelpDisablerTask() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoCommonWindowMessage>;
template MTask *MCreateBgCallTask(DinoCommonWindowMessage *obj, void (DinoCommonWindowMessage::*func)(), int thread);
template class MCallTask0<DinoCommonWindowMessage, DinoCommonWindowMessage>;
template MTask *MCreateCallTask(DinoCommonWindowMessage *obj, void (DinoCommonWindowMessage::*func)());
