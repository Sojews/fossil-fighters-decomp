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
#include "game/dino_sound.h"

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

void DinoUiMaskEquipBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiMaskEquipBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMaskEquipPageRButton::OnDecide() {
    if (mOwner->PageRight()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiMaskEquipPageLButton::OnDecide() {
    if (mOwner->PageLeft()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMaskEquipMenu, DinoUiMaskEquipMenu>;
template MTask *MCreateCallTask(DinoUiMaskEquipMenu *obj, void (DinoUiMaskEquipMenu::*func)());
