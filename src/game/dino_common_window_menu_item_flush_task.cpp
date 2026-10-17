// DinoCommonWindowMenuItemFlushTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/dino_common_window_menu_item_flush_task.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoCommonWindowMenuItemFlushTask::~DinoCommonWindowMenuItemFlushTask() {
}

DinoCommonWindowMenu::~DinoCommonWindowMenu() {
}

DinoCommonWindowMenuItem::~DinoCommonWindowMenuItem() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoCommonWindowMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoCommonWindowMenu *obj, void (DinoHelpBase::*func)());
