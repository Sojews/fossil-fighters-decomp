// DinoUiMultiplayMenuItem, DinoUiMultiplayMenuItem2.
// Only the destructors are decompiled so far.

#include "game/dino_ui_multiplay_menu_item.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiMultiplayMenuItem::~DinoUiMultiplayMenuItem() {
}

DinoUiMultiplayMenuItem2::~DinoUiMultiplayMenuItem2() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiMultiplayMenu : public DinoHelpBase {};
template class MCallTask0<DinoUiMultiplayMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiMultiplayMenu *obj, void (DinoHelpBase::*func)());
