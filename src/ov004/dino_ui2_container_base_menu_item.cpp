// DinoUi2ContainerBaseMenuItem.
// Only the destructors are decompiled so far.

#include "game/dino_ui2_container_base_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUi2ContainerBaseMenuItem::~DinoUi2ContainerBaseMenuItem() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUi2CannonShootMenu {};
template class MCallTask0<DinoUi2CannonShootMenu, DinoUi2CannonShootMenu>;
template MTask *MCreateCallTask(DinoUi2CannonShootMenu *obj, void (DinoUi2CannonShootMenu::*func)());
