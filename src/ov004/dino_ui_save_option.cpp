// DinoUiSaveOption, DinoUiSaveOptionAppendItemTask, DinoUiSaveOptionRemoveItemTask, DinoUiSaveOptionMenuSaveTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_save_option_menu.h"
#include "game/dino_ui_save_option.h"
#include "game/dino_ui_save_option_append_item_task.h"
#include "game/dino_ui_save_option_remove_item_task.h"
#include "game/dino_ui_save_option_menu_save_task.h"
#include "game/dino_ui_save_option_menu_item.h"
#include "game/dino_sound.h"

DinoUiSaveOption::~DinoUiSaveOption() {
}

DinoUiSaveOptionAppendItemTask::~DinoUiSaveOptionAppendItemTask() {
}

DinoUiSaveOptionRemoveItemTask::~DinoUiSaveOptionRemoveItemTask() {
}

DinoUiSaveOptionMenuSaveTask::~DinoUiSaveOptionMenuSaveTask() {
}

DinoUiSaveOptionMenuItem::~DinoUiSaveOptionMenuItem() {
}

DinoUiSaveOptionMenu::~DinoUiSaveOptionMenu() {
}

DinoUiSaveOptionSaveButton::~DinoUiSaveOptionSaveButton() {
}

DinoUiSaveOptionOptionButton::~DinoUiSaveOptionOptionButton() {
}

DinoUiSaveOptionBackButton::~DinoUiSaveOptionBackButton() {
}

MRect DinoUiSaveOptionMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiSaveOptionMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiSaveOptionMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiSaveOptionMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiSaveOptionMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiSaveOptionBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiSaveOptionRemoveItemTask::Execute() {
    mOwner->RemoveItem();
    Exit();
}

void DinoUiSaveOptionBackButton::OnDecide() {
    mMotion->Play(4, 1);
    DinoSoundPlaySe(0x38F, 0);
    mOwner->OnBackDecided();
}

void DinoUiSaveOptionMenu::OnFocus(MMenuItem *item) {
    if (mSilentFocus) {
        mSilentFocus = FALSE;
    } else {
        DinoSoundPlaySe(0x38E, 0);
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
}

void DinoUiSaveOptionOptionButton::OnDecide() {
    mMotion->Play(3, 1);
    mOwner->OnOptionDecided();
}

void DinoUiSaveOptionSaveButton::OnDecide() {
    mMotion->Play(3, 1);
    mOwner->OnSaveDecided();
}

MRect DinoUiSaveOptionOptionButton::GetFrame() {
    return MRect(mMotion->GetFrame().GetTopLeft(), mMotion->GetFrame().GetTopLeft(108, 76));
}

MRect DinoUiSaveOptionSaveButton::GetFrame() {
    return MRect(mMotion->GetFrame().GetTopLeft(), mMotion->GetFrame().GetTopLeft(108, 76));
}
