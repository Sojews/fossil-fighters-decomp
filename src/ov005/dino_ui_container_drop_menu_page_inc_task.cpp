// DinoUiContainerDropMenuPageIncTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_container_drop_menu_page_inc_task_menu.h"
#include "game/dino_ui_container_drop_menu_page_inc_task.h"
#include "game/dino_ui_container_drop_menu_item.h"
#include "game/dino_sound.h"

DinoUiContainerDropMenuPageIncTask::~DinoUiContainerDropMenuPageIncTask() {
}

DinoUiContainerDropMenuItem::~DinoUiContainerDropMenuItem() {
}

DinoUiContainerDropMenu::~DinoUiContainerDropMenu() {
}

DinoUiContainerDropBackButton::~DinoUiContainerDropBackButton() {
}

DinoUiContainerDropPageLButton::~DinoUiContainerDropPageLButton() {
}

DinoUiContainerDropPageRButton::~DinoUiContainerDropPageRButton() {
}

MRect DinoUiContainerDropMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerDropMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerDropMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerDropMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerDropMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerDropBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerDropBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}
