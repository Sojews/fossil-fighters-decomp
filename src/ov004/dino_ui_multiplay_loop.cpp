// DinoUiMultiplayLoop.
// Only the destructors are decompiled so far.

#include "game/dino_ui_multiplay_loop.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "game/dino_ui_multiplay_menu_item.h"
#include "game/dino_ui_multiplay_menu_item_menu.h"

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

MRect DinoUiMultiplayMenuItem2::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMultiplayMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMultiplayMenuBackButton::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMultiplayWirelessButton::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiMultiplayMenuItem2::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiMultiplayMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMultiplayLoop::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMultiplayMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiMultiplayMenu *obj, void (DinoHelpBase::*func)());
