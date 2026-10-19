// DinoUiPartsTabIconMenuItem, DinoUiStatusMenuItem, DinoUiStatusChangeModeHelpTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_parts_tab_icon_menu_item_menu.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "game/dino_ui_status_menu_item.h"
#include "game/dino_ui_status_change_mode_help_task.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"

#pragma auto_inline off
DinoUiStatusMenu::DinoUiStatusMenu(int *choice, DinoTeam *team, DinoTeam *team2)
    : MMenu(gLcd->GetBottomDisplay(), NULL, 0), mResult(choice), mTeam((Team *)team), mUnk11C((u32)team2), mUnk120(0),
      mSelected(0), mMode(0), mUnk12D(0) {
}
#pragma auto_inline on

DinoUiPartsTabIconMenuItem::~DinoUiPartsTabIconMenuItem() {
}

DinoUiStatusMenuItem::~DinoUiStatusMenuItem() {
}

DinoUiStatusChangeModeHelpTask::~DinoUiStatusChangeModeHelpTask() {
}

DinoUiStatusMenu::~DinoUiStatusMenu() {
}

DinoUiStatusBackButton::~DinoUiStatusBackButton() {
}

DinoUiStatusOKButton::~DinoUiStatusOKButton() {
}

void DinoUiStatusChangeModeHelpTask::Idle() {
}

void DinoUiPartsTabIconMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiPartsTabIconMenuItem::OnDecide() {
}

void DinoUiPartsTabIconMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiStatusMenuItem::GetRect() {
    return GetFrame();
}

MRect DinoUiPartsTabIconMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiPartsTabIconMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiPartsTabIconMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiStatusMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiStatusOKButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnOKDecided();
    DinoSoundPlaySe(0x9A, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiStatusMenu, DinoUiStatusMenu>;
template MTask *MCreateCallTask(DinoUiStatusMenu *obj, void (DinoUiStatusMenu::*func)());
template class MCallTask0<DinoUiStatusChangeModeHelpTask, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiStatusChangeModeHelpTask *obj, void (DinoHelpBase::*func)());

void DinoUiStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

MRect DinoUiPartsTabIconMenuItem::GetFrame() {
    return mFrame;
}

void DinoUiStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (item == mMembers[i]) {
            if (mMode == 1) {
                DecideMember(i);
            } else if (mTeam->pickable[i]) {
                mSelected = i;
                SelectMember(i);
            }
            return;
        }
    }
}

#pragma auto_inline off

DinoUiStatusMenuItem::DinoUiStatusMenuItem(DinoUiStatusMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(2, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mMotion->GetUnit()->SetMode(1);
}
