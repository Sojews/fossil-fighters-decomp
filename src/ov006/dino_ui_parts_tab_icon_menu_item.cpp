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
#include "game/dino_ui_parts_reviver_name.h"
#include "game/dino_common_window_message_box.h"
#include "game/common_window_message.h"
#include "m/view.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "m/bright.h"
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
#pragma auto_inline off
template class MCallTask0<DinoUiStatusMenu, DinoUiStatusMenu>;
template MTask *MCreateCallTask(DinoUiStatusMenu *obj, void (DinoUiStatusMenu::*func)());
template class MCallTask0<DinoUiStatusChangeModeHelpTask, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiStatusChangeModeHelpTask *obj, void (DinoHelpBase::*func)());
#pragma auto_inline reset

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
    mHelp = NULL;
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

extern const int data_02089a20; // fade frames
extern const int data_02089a88;

void DinoUiStatusMenu::SelectTab(int tab) {
    if (tab < 0 || tab > 2) {
        return;
    }
    switch (tab) {
    case 0:
        break;
    case 1: {
        *mResult = 2;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130), fade, exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    case 2: {
        *mResult = 3;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130), fade, exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    }
}

// Where things go on the screen (filled in at start).
struct DinoUiStatusLayout {
    u8 unk00[0x14];
    MPoint memberMenuOffset;    // 0x14: from the member's medal
};
extern DinoUiStatusLayout data_ov006_02190e9c;
extern const u32 data_0208aad4, data_0208aad8, data_0208aadc;
extern const u32 data_0208a878, data_0208a87c, data_0208a880;

// The menu for a team member.
void DinoUiStatusMenu::SelectMember(int index) {
    mUnk120 = 0;
    mMenuBack->Hide();
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(
        &mUnk120, data_ov006_02191000[index] + data_ov006_02190e9c.memberMenuOffset, NULL, 0);
    menu->AddChoice(1, gDinoResource->GetText(data_0208aadc), gDinoResource->GetText(data_0208a880));
    menu->AddChoice(2, gDinoResource->GetText(data_0208aad8), gDinoResource->GetText(data_0208a87c));
    menu->AddChoice(-2, gDinoResource->GetText(data_0208aad4), gDinoResource->GetText(data_0208a878));
    GetSerial()->AddTail(menu, mMenuBack->CreateShowTransfer(),
                         MCreateCallTask(this, &DinoUiStatusMenu::HandleMemberResult));
}

extern const u32 data_0208aad0;

void DinoUiStatusMenu::HandleMemberResult() {
    if (mUnk120 == 1) {
        mMode = 1;
        mMessage = new DinoCommonWindowMessage(MPoint(128, 96), std::string(gDinoResource->GetText(data_0208aad0)), 4, 0);
        mParallel.AddTail(mMessage);
        PickReplacement(mSelected);
        mMembers[mSelected]->PlayDecided();
        FocusNearestItem(GetCursor());
        if (mSelected < 3) {
            mTech->Hide();
        }
    } else if (mUnk120 == 2) {
        *mResult = mSelected + 4;
        GetSerial()->AddTail(gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0),
                             MTaskExitTransfer::Create(&(MTask &)*this));
    } else if (mUnk120 == -2) {
        mMembers[mSelected]->PlayFocused();
    }
    mUnk120 = 0;
}

void DinoUiStatusMenu::PickReplacement(int index) {
    int i;
    for (i = 0; i < 5; i++) {
        mMembers[i]->Enable();
    }
    if (!mTeam->slots[index]) {
        for (int j = 0; j < 5; j++) {
            if (!mTeam->slots[j]) {
                mMembers[j]->Disable();
            }
        }
    }
    mMembers[index]->Disable();
    for (i = 0; i < 3; i++) {
        mTabItems[i]->Disable();
    }
    mOKButton->MMenuItem::Disable();
    LinkMembers();
    for (i = 0; i < 5; i++) {
        RemoveLink(mMembers[i], 2);
        AddLink(mMembers[i], 2, mUnkB8);
    }
    mHelp = new DinoUiStatusChangeModeHelpTask(mUnkB8->GetFrame());
    mUnk108->AddTail(mHelp);
}

extern const u32 data_0208aa4c, data_0208aae0;
extern const int data_02089a2c, data_02089a30, data_02089a34;

void DinoUiStatusMenu::OnOKDecided() {
    if (mMode) {
        return;
    }
    *mResult = 0;
    if (mTeam->slots[0]) {
        if (DinoTeamDiffers(mTeam, (DinoTeam *)mUnk11C)) {
            mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96),
                                                           std::string(gDinoResource->GetText(data_0208aae0)), 4));
        } else {
            mSerial.AddTail(new MTimer(data_02089a34));
        }
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130),
                             gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a30, data_02089a2c),
                             MTaskExitTransfer::Create(&(MTask &)*this));
    } else {
        mSerial.AddTail(
            new DinoCommonWindowMessageBox(MPoint(128, 96), std::string(gDinoResource->GetText(data_0208aa4c)), 4));
    }
}

// Swaps the picked member with the one decided on.
void DinoUiStatusMenu::DecideMember(int index) {
    mMode = 0;
    mMessage->mUnk98 = 15;
    mName->Hide();
    MParallelTaskSet *anims = new MParallelTaskSet(TRUE);
    if (mTeam->slots[index]) {
        mMembers[index]->PlayDecided();
        anims->AddTail(mMembers[index]->CreateFlyInTask(data_ov006_02191000[mSelected]));
    } else {
        mMembers[index]->PlayNormal();
    }
    anims->AddTail(mMembers[mSelected]->CreateFlyInTask(data_ov006_02191000[index]));
    GetSerial()->AddTail(new MTimer(5), anims, MTaskExitTransfer::Create(mMessage),
                         MCreateCallTask(this, &DinoUiStatusMenu::FinishPage));
    u8 slot = mTeam->slots[mSelected];
    mTeam->slots[mSelected] = mTeam->slots[index];
    mTeam->slots[index] = slot;
    if (mHelp != NULL) {
        mHelp->mSerial.Clear();
        mHelp->Disable();
    }
}

extern const u32 data_0208aa3c;

void DinoUiStatusMenu::OnBackDecided() {
    if (mMode == 1) {
        mMode = 0;
        mMessage->Exit();
        RefreshItems();
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mUnkB8->mSerial),
                             MCreateCallTask(this, &DinoUiStatusMenu::FinishTab));
        return;
    }
    Disable();
    *mResult = 9;
    mSerial.AddTail(new MTimer(data_02089a34));
    if (DinoTeamDiffers(mTeam, (DinoTeam *)mUnk11C)) {
        MParallelTaskSet *anims = new MParallelTaskSet(TRUE);
        for (int i = 0; i < 5; i++) {
            if (mTeam->slots[i] != 0 && mTeam->slots[i] != ((DinoTeam *)mUnk11C)->slots[i]) {
                int from = 5;
                for (int j = 0; j < 5; j++) {
                    if (mTeam->slots[i] == ((DinoTeam *)mUnk11C)->slots[j]) {
                        from = j;
                    }
                }
                if (from != 5) {
                    anims->AddTail(mMembers[i]->CreateFlyInTask(data_ov006_02191000[from]));
                }
            }
        }
        GetSerial()->AddTail(
            new DinoCommonWindowMessageBox(MPoint(128, 96), std::string(gDinoResource->GetText(data_0208aa3c)), 4),
            anims);
    }
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130),
                         gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a30, data_02089a2c),
                         MTaskExitTransfer::Create(&(MTask &)*this));
}
