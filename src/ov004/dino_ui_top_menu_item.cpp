// DinoUiTopMenuItem, DinoUiTopMenuItem2.
// Only the destructors (and the thunks the compiler makes with them) are decompiled so far.

// The buttons' destructors have their base classes' destructor bodies inlined (the game probably defines these
// destructors inline in a header); auto-inlining the ones defined above reproduces that.
#pragma auto_inline on

#include "game/dino_ui_top_menu_item.h"
#include "game/dino_ui_top_menu_item2.h"
#include "game/dino_ui_save_slot_select.h"
#include "game/help.h"
#include "game/dino_ui_top_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiTopMenu::~DinoUiTopMenu() {
}

DinoUiTopMenuItem::~DinoUiTopMenuItem() {
}

DinoUiTopMenuBackButton::~DinoUiTopMenuBackButton() {
}

DinoUiTopMenuSingleButton::~DinoUiTopMenuSingleButton() {
}

DinoUiTopMenuMultiButton::~DinoUiTopMenuMultiButton() {
}

DinoUiTopMenuMultiDarkButton::~DinoUiTopMenuMultiDarkButton() {
}

DinoUiTopMenuItem2::~DinoUiTopMenuItem2() {
}

DinoUiTopMenuPassButton::~DinoUiTopMenuPassButton() {
}

DinoUiTopMenuPassDarkButton::~DinoUiTopMenuPassDarkButton() {
}

DinoUiTopMenuOptionButton::~DinoUiTopMenuOptionButton() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiTopMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiTopMenu *obj, void (DinoHelpBase::*func)());
template class MBgCallTask0<DinoUiSaveSlotSelect>;
template MTask *MCreateBgCallTask(DinoUiSaveSlotSelect *obj, void (DinoUiSaveSlotSelect::*func)(), int thread);
