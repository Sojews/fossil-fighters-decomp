// DinoUiTopMenuLoop.
// Only the destructors are decompiled so far.

#include "game/dino_ui_top_menu_loop.h"
#include "game/dino_backup.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiTopMenuLoop::~DinoUiTopMenuLoop() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiTopMenu : public DinoHelpBase {};
template class MBgCallTask0<DinoBackup>;
template MTask *MCreateBgCallTask(DinoBackup *obj, void (DinoBackup::*func)(), int thread);
template class MCallTask0<DinoUiTopMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiTopMenu *obj, void (DinoHelpBase::*func)());
