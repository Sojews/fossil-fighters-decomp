// DinoUiSaveSlotSelect, DinoUiSaveSlotConfirm, DinoUiSaveSlotSelectMenuConfirmDeleteTask.
// Only the destructors are decompiled so far.

#include "game/dino_ui_save_slot_select.h"
#include "game/dino_ui_save_slot_confirm.h"
#include "game/dino_ui_save_slot_select_menu_confirm_delete_task.h"
#include "game/dino_ui_save_slot_select_menu_item.h"
#include "game/dino_ui_save_slot_select_menu_delete_item.h"
#include "game/dino_ui_save_slot_select_menu_cancel_item.h"
#include "game/dino_ui_save_slot_select_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiSaveSlotSelect::~DinoUiSaveSlotSelect() {
}

DinoUiSaveSlotConfirm::~DinoUiSaveSlotConfirm() {
}

DinoUiSaveSlotSelectMenuConfirmDeleteTask::~DinoUiSaveSlotSelectMenuConfirmDeleteTask() {
}

DinoUiSaveSlotSelectMenuItem::~DinoUiSaveSlotSelectMenuItem() {
}

DinoUiSaveSlotSelectMenuDeleteItem::~DinoUiSaveSlotSelectMenuDeleteItem() {
}

DinoUiSaveSlotSelectMenuCancelItem::~DinoUiSaveSlotSelectMenuCancelItem() {
}

DinoUiSaveSlotSelectMenu::~DinoUiSaveSlotSelectMenu() {
}

DinoUiSaveSlotConfirmMenu::~DinoUiSaveSlotConfirmMenu() {
}

MRect DinoUiSaveSlotSelectMenuCancelItem::GetRect() {
    return GetFrame();
}

MRect DinoUiSaveSlotSelectMenuDeleteItem::GetRect() {
    return GetFrame();
}

MRect DinoUiSaveSlotSelectMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiSaveSlotSelectMenuCancelItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiSaveSlotSelectMenuDeleteItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiSaveSlotSelectMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiSaveSlotSelectMenuCancelItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiSaveSlotSelectMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiSaveSlotSelectMenuDeleteItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiSaveSlotSelectMenuCancelItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiOpening {};
template class MCallTask1<DinoUiSaveSlotSelectMenu, DinoUiSaveSlotSelectMenu, u32>;
template MTask *MCreateCallTask(DinoUiSaveSlotSelectMenu *obj, void (DinoUiSaveSlotSelectMenu::*func)(u32), u32 a0);
template class MCallTask0<DinoUiSaveSlotSelectMenu, DinoUiSaveSlotSelectMenu>;
template MTask *MCreateCallTask(DinoUiSaveSlotSelectMenu *obj, void (DinoUiSaveSlotSelectMenu::*func)());
template class MBgCallTask0<DinoUiSaveSlotSelect>;
template MTask *MCreateBgCallTask(DinoUiSaveSlotSelect *obj, void (DinoUiSaveSlotSelect::*func)(), int thread);
template class MBgCallTask0<DinoUiOpening>;
template MTask *MCreateBgCallTask(DinoUiOpening *obj, void (DinoUiOpening::*func)(), int thread);
