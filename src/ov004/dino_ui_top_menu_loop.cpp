// DinoUiTopMenuLoop.
// Only the destructors are decompiled so far.

#include "game/dino_ui_top_menu_loop.h"
#include "game/dino_backup.h"
#include "game/help.h"
#include "game/dino_ui_top_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_top_menu_item2.h"
#include "game/dino_ui_top_menu_item.h"

DinoUiTopMenuLoop::~DinoUiTopMenuLoop() {
}

MRect DinoUiTopMenuItem2::GetRect() {
    return GetFrame();
}

MRect DinoUiTopMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiTopMenuItem2::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiTopMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiTopMenuBackButton::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiTopMenuItem2::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiTopMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiTopMenuLoop::Idle() {
    (this->*mState)();
}

void DinoUiTopMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiTopMenuBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBackup>;
template MTask *MCreateBgCallTask(DinoBackup *obj, void (DinoBackup::*func)(), int thread);
template class MCallTask0<DinoUiTopMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiTopMenu *obj, void (DinoHelpBase::*func)());
