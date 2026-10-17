// DinoUiMaskEquip, DinoUiMaskEquipTest, DinoUiMaskEquipMaskButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_mask_equip_menu.h"
#include "game/dino_ui_mask_equip.h"
#include "game/dino_ui_mask_equip_test.h"
#include "game/dino_ui_mask_equip_mask_button.h"
#include "game/dino_ui_mask_equip_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiMaskEquip::~DinoUiMaskEquip() {
}

DinoUiMaskEquipTest::~DinoUiMaskEquipTest() {
}

DinoUiMaskEquipMaskButton::~DinoUiMaskEquipMaskButton() {
}

DinoUiMaskEquipMenuItem::~DinoUiMaskEquipMenuItem() {
}

DinoUiMaskEquipMenu::~DinoUiMaskEquipMenu() {
}

DinoUiMaskEquipBackButton::~DinoUiMaskEquipBackButton() {
}

DinoUiMaskEquipPageLButton::~DinoUiMaskEquipPageLButton() {
}

DinoUiMaskEquipPageRButton::~DinoUiMaskEquipPageRButton() {
}

void DinoUiMaskEquipMenuItem::OnSelectChanged(BOOL value) {
}

MRect DinoUiMaskEquipMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMaskEquipMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMaskEquipMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMaskEquipMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiMaskEquipMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMaskEquipMenu, DinoUiMaskEquipMenu>;
template MTask *MCreateCallTask(DinoUiMaskEquipMenu *obj, void (DinoUiMaskEquipMenu::*func)());
