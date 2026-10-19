// DinoUiPartsTabIconMenuItem, DinoUiStatusMenuItem, DinoUiStatusChangeModeHelpTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "game/dino_ui_parts_reviver_medal_button.h"
#include "game/dino_ui_parts_tab_icon_menu_item_menu.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "game/dino_ui_status_menu_item.h"
#include "game/dino_ui_status_change_mode_help_task.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/smart_oam.h"
#include "m/motion.h"
#include "game/dino_backup.h"
#include "game/dino_team.h"
#include "game/dino_ui_parts_team_tech.h"

#pragma auto_inline off
DinoUiStatusMenu::DinoUiStatusMenu(int *choice, DinoTeam *team, DinoTeam *team2)
    : MMenu(gLcd->GetBottomDisplay(), NULL, 0), mResult(choice), mTeam(team), mUnk11C((u32)team2), mUnk120(0),
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
            } else if (mTeam->slots[i]) {
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

void DinoUiStatusMenu::FinishPage() {
    ClearItems();
    CreateItems();
    RefreshItems();
    RefreshOKButton();
    FocusNearestItem(GetCursor());
}

void DinoUiStatusMenu::FinishTab() {
    mMembers[mSelected]->PlayNormal();
    FocusItem(mMembers[mSelected]);
    RefreshOKButton();
}

void DinoUiStatusMenu::ClearItems() {
    RemoveLink(mUnkB8, 4);
    for (int i = 0; i < 5; i++) {
        RemoveItem(mMembers[i]);
        delete mMembers[i];
    }
    mUnk12D = 0;
}

// The OK button lights up when the first member can be picked; the technique shows when the team has one.
void DinoUiStatusMenu::RefreshOKButton() {
    if (mTeam->slots[0]) {
        mOKButton->mMotion->GetUnit()->SetMode(1);
    } else {
        mOKButton->mMotion->GetUnit()->SetMode(0);
    }
    if (!gDinoBackup->GetSystemFlag(2)) {
        if (DinoTeamHasTech(mTeam)) {
            mTech->Show();
        } else {
            mTech->Hide();
        }
    }
}

void DinoUiStatusMenu::RefreshItems() {
    for (int i = 0; i < 5; i++) {
        mMembers[i]->Enable();
    }
    for (int j = 0; j < 3; j++) {
        mTabItems[j]->Enable();
    }
    mOKButton->MMenuItem::Enable();
    LinkMembers();
    for (int i = 0; i < 5; i++) {
        RemoveLink(mMembers[i], 2);
        AddLink(mMembers[i], 2, mTabItems[0]);
    }
    mUnk108->Clear();
    mUnk10C = 0;
}

// (Data the item positions come from.)
extern "C" const MPoint data_ov006_02191000[5];
extern const u32 kCreatureLevelLimits[2];

// A creature's experience, up to what the top level needs.
static inline u32 GetCreatureTotal(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    u32 sum = creature.unk0_8 + (creature.parts[0].level + creature.parts[1].level + creature.parts[2].level +
                                 creature.parts[3].level);
    return std::min(kCreatureLevelLimits[0] * 11 + 50, sum);
}

static inline u32 GetCreatureLevel(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    return (GetCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

void DinoUiStatusMenu::CreateItems() {
    for (int i = 0; i < 5; i++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(mTeam->slots[i], &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        mMembers[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_02191000[i], mTeam->slots[i], maxed, 0);
        AddItem(mMembers[i]);
        mMembers[i]->SetMode(2);
        AddLink(mMembers[i], 2, mTabItems[0]);
        AddLink(mMembers[i], 4, mUnkB8);
    }
    AddLink(mTabItems[0], 2, mMembers[0]);
    AddLink(mTabItems[1], 2, mMembers[0]);
    AddLink(mTabItems[2], 2, mMembers[0]);
    AddLink(mOKButton, 2, mMembers[0]);
    AddLink(mUnkB8, 2, mMembers[0]);
}

// Links the members left and right to the nearest enabled ones (round the row), and the tabs and buttons down to the
// first enabled member.
void DinoUiStatusMenu::LinkMembers() {
    MMenuItem *left[5];
    MMenuItem *right[5];
    int i;
    for (i = 0; i < 5; i++) {
        left[i] = mMembers[(i + 4) % 5];
        right[i] = mMembers[(i + 1) % 5];
    }
    int first = 0;
    for (i = 0; i < 5; i++) {
        if (mMembers[i]->IsEnabled()) {
            first = i;
            break;
        }
    }
    for (i = 0; i < 5; i++) {
        int k = first + i;
        if (!mMembers[k % 5]->IsEnabled()) {
            left[(k + 1) % 5] = left[k % 5];
        }
    }
    int start = (first + 1) % 5;
    for (i = 4; i >= 0; i--) {
        int k = start + i;
        if (!mMembers[k % 5]->IsEnabled()) {
            right[(k + 4) % 5] = right[k % 5];
        }
    }
    if (mUnk12D) {
        RemoveLink(mUnkB8, 4);
        for (i = 0; i < 5; i++) {
            RemoveLink(mMembers[i], 3);
            RemoveLink(mMembers[i], 1);
        }
    }
    if (mMembers[4]->IsEnabled()) {
        AddLink(mUnkB8, 4, mMembers[4]);
    } else if (mMembers[3]->IsEnabled()) {
        AddLink(mUnkB8, 4, mMembers[3]);
    } else {
        AddLink(mUnkB8, 4, mMembers[2]);
    }
    for (i = 0; i < 5; i++) {
        AddLink(mMembers[i], 3, left[i]);
        AddLink(mMembers[i], 1, right[i]);
    }
    if (mMembers[0]->IsEnabled()) {
        RemoveLink(mTabItems[0], 2);
        RemoveLink(mTabItems[1], 2);
        RemoveLink(mTabItems[2], 2);
        RemoveLink(mOKButton, 2);
        RemoveLink(mUnkB8, 2);
        AddLink(mTabItems[0], 2, mMembers[0]);
        AddLink(mTabItems[1], 2, mMembers[0]);
        AddLink(mTabItems[2], 2, mMembers[0]);
        AddLink(mOKButton, 2, mMembers[0]);
        AddLink(mUnkB8, 2, mMembers[0]);
    } else {
        RemoveLink(mTabItems[0], 2);
        RemoveLink(mTabItems[1], 2);
        RemoveLink(mTabItems[2], 2);
        RemoveLink(mOKButton, 2);
        RemoveLink(mUnkB8, 2);
        AddLink(mTabItems[0], 2, mMembers[1]);
        AddLink(mTabItems[1], 2, mMembers[1]);
        AddLink(mTabItems[2], 2, mMembers[1]);
        AddLink(mOKButton, 2, mMembers[1]);
        AddLink(mUnkB8, 2, mMembers[1]);
    }
    mUnk12D = 1;
}
