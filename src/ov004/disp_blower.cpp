// DispBlower, OptionKeyCheckTask.
// Only the destructors are decompiled so far.

#include "game/disp_blower.h"
#include "game/option_key_check_task.h"
#include "game/dino_ui_option_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DispBlower::~DispBlower() {
}

OptionKeyCheckTask::~OptionKeyCheckTask() {
}

DinoUiOptionMenuItem::~DinoUiOptionMenuItem() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiOptionMenu : public DinoHelpBase {};
template class MCallTask0<DinoUiOptionMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiOptionMenu *obj, void (DinoHelpBase::*func)());
