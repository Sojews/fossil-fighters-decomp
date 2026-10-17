// DinoUiMultiplayLoop.
// Only the destructors are decompiled so far.

#include "game/dino_ui_multiplay_loop.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "game/dino_ui_multiplay_menu_item.h"

DinoUiMultiplayLoop::~DinoUiMultiplayLoop() {
}

MRect DinoUiMultiplayMenuItem2::GetRect() {
    return GetFrame();
}

MRect DinoUiMultiplayMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMultiplayMenuItem2::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiMultiplayMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiMultiplayMenu : public DinoHelpBase {};
template class MCallTask0<DinoUiMultiplayMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiMultiplayMenu *obj, void (DinoHelpBase::*func)());
