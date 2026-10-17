// DinoUiTopMenuItem, DinoUiTopMenuItem2.
// Only the destructors are decompiled so far.

#include "game/dino_ui_top_menu_item.h"
#include "game/dino_ui_top_menu_item2.h"
#include "game/dino_ui_save_slot_select.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiTopMenuItem::~DinoUiTopMenuItem() {
}

DinoUiTopMenuItem2::~DinoUiTopMenuItem2() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiTopMenu : public DinoHelpBase {};
template class MCallTask0<DinoUiTopMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiTopMenu *obj, void (DinoHelpBase::*func)());
template class MBgCallTask0<DinoUiSaveSlotSelect>;
template MTask *MCreateBgCallTask(DinoUiSaveSlotSelect *obj, void (DinoUiSaveSlotSelect::*func)(), int thread);
