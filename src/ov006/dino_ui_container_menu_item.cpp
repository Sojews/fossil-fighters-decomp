// DinoUiContainerMenuItem.
// Only the destructors are decompiled so far.

#include "game/dino_ui_container_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiContainerMenuItem::~DinoUiContainerMenuItem() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiContainerMenu {};
template class MCallTask0<DinoUiContainerMenu, DinoUiContainerMenu>;
template MTask *MCreateCallTask(DinoUiContainerMenu *obj, void (DinoUiContainerMenu::*func)());
