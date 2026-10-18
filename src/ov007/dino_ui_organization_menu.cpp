// DinoUiOrganizationMenu (team organization).
// NONMATCHING: RemoveMember (the search loop); the rest (OnStart, OnFocus, OnDecide, slot creation...) is to do.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "game/dino_sound.h"
#include "game/dino_ui_organization_menu.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "m/display.h"
#include "m/blend.h"
#include "m/call_task.h"
#include "m/bg_new_task.h"
#include "m/picture.h"
#include "game/dino_ui_rv_information.h"
#include "game/dino_team.h"
#include "game/dino_ui_parts_team_tech.h"
#include "game/dino_ui_parts_reviver_medal_button.h"
#include "game/common_window_message.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/dino_common_window_message_box.h"
#include "m/lcd.h"
#include "m/bright.h"

DinoUiOrganizationMenu::~DinoUiOrganizationMenu() {
    gCurrentDisplay->mBlend->Reset();
}

DinoUiOrganizationMenuItem::~DinoUiOrganizationMenuItem() {
}

DinoUiOrganizationOKButton::~DinoUiOrganizationOKButton() {
}

DinoUiOrganizationBackButton::~DinoUiOrganizationBackButton() {
}

DinoUiOrganizationPageLButton::~DinoUiOrganizationPageLButton() {
}

DinoUiOrganizationPageRButton::~DinoUiOrganizationPageRButton() {
}

DinoUiOrganizationSlotCreateTask::~DinoUiOrganizationSlotCreateTask() {
}

DinoUiOrganizationMenuBlendResetTask::~DinoUiOrganizationMenuBlendResetTask() {
}

MRect DinoUiOrganizationMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOrganizationMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOrganizationMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiOrganizationMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiOrganizationOKButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrganizationBackButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrganizationMenuBlendResetTask::Execute() {
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->Reset();
    blend->SetMode(3);
    blend->SetBrightness(8);
    blend->SetAlpha(16, 0);
    blend->SetTarget1(0x10);
    blend->SetTarget2(0x3F);
    Exit();
}


void DinoUiOrganizationMenu::OnIdle() {
    mUnk23E = TRUE;
    if (GetInput()->GetPressed() & 0x800) {
        TabLeft();
        return;
    }
    if (GetInput()->GetPressed() & 0x400) {
        TabRight();
    }
}

extern "C" void func_ov007_0217d548(void *owner);

void DinoUiOrganizationOKButton::OnDecide() {
    mMotion->Play(4, 1);
    MTask *play = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    func_ov007_0217d548(mOwner);
    DinoSoundPlaySe(0x9A, 0);
}

void DinoUiOrganizationBackButton::OnDecide() {
    mMotion->Play(4, 1);
    MTask *play = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    mOwner->Back();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiOrganizationPageLButton::OnDecide() {
    mOwner->PageLeft();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiOrganizationPageRButton::OnDecide() {
    mOwner->PageRight();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

extern "C" const int data_02089a0c;

#pragma auto_inline off

extern "C" void func_ov007_0217fb94(DinoUiOrganizationMenu *menu, int a1);
// Where the team's medals go.
extern MPoint data_ov007_0218ef0c[5];

void DinoUiOrganizationMenu::ShowTabInfo(u8 tab) {
    mInfo->SetPage(tab);
    MTask *enable = MCreateCallTask(this, &DinoHelpBase::Enable);
    MTask *wait = MTaskSetWaitEmptyTask::Create(&mInfo->mSerial);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(enable);
    mUnk230->Clear();
    Disable();
}

void DinoUiOrganizationMenu::ClearSlots() {
    RemoveSlotLinks();
    for (int i = 0; i < 15; i++) {
        RemoveItem(mSlots[i]);
        delete mSlots[i];
    }
}

void DinoUiOrganizationMenu::FinishPageRight() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

void DinoUiOrganizationMenu::FinishPageLeft() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

void DinoUiOrganizationMenu::PageRight() {
    if (mPage == mLastPage) {
        mPage = 0;
    } else {
        mPage++;
    }
    mSerial.AddTail(MCreateBgCallTask(this, &DinoUiOrganizationMenu::LoadPage, 1));
    mSerial.AddTail(MCreateCallTask(this, &DinoUiOrganizationMenu::FinishPageRight));
    Disable();
}

void DinoUiOrganizationMenu::PageLeft() {
    if (mPage == 0) {
        mPage = mLastPage;
    } else {
        mPage--;
    }
    mSerial.AddTail(MCreateBgCallTask(this, &DinoUiOrganizationMenu::LoadPage, 1));
    mSerial.AddTail(MCreateCallTask(this, &DinoUiOrganizationMenu::FinishPageLeft));
    Disable();
}

// The pictures of the tabs' titles.
extern std::string data_ov007_0218ef34[4];

void DinoUiOrganizationMenu::SetTab(u8 tab) {
    if (tab > 3) {
        return;
    }
    BOOL first = FALSE;
    if (mTab == 0xFF) {
        first = TRUE;
    }
    if (mTab == tab) {
        return;
    }
    mTab = tab;
    if (mTabPicture != NULL) {
        mParallel.Remove(mTabPicture);
        delete mTabPicture;
    }
    mParallel.AddTail(mTabPicture = new MTextPicture(data_ov007_0218ef34[tab], TRUE, false));
    mTabPicture->Show();
    mTabPicture->SetPriority(3);
    for (int i = 0; i < 3; i++) {
        RemoveLink(mButtons[i], 2);
        AddLink(mButtons[i], 2, mTabs[tab]);
    }
    Disable();
    UpdateDetails(0xFF);
    if (first) {
        ShowTabInfo(tab);
        return;
    }
    DinoSoundPlaySe(0x94, 0);
    MTask *show = MCreateCallTask(this, &DinoUiOrganizationMenu::ShowTabInfo, tab);
    MTask *timer = new MTimer(1);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(show);
}

void DinoUiOrganizationMenu::TabLeft() {
    SetTab((mTab + 3) % 4);
    if (mFocusTabs) {
        FocusItem(mTabs[mTab]);
    }
}

void DinoUiOrganizationMenu::TabRight() {
    SetTab((mTab + 1) % 4);
    if (mFocusTabs) {
        FocusItem(mTabs[mTab]);
    }
}

BOOL DinoUiOrganizationMenu::InTeam(u32 id) {
    for (int i = 0; i < 5; i++) {
        if (id == mTeam.slots[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

void DinoUiOrganizationMenu::SetSort(u8 sort) {
    if (sort < 5 && mSort != sort) {
        mSort = sort;
        DinoSoundPlaySe(0x98, 0);
        mSortCursor->SetPosition(0, sort * -0xC0);
        RemoveSlotLinks();
        ClearSlots();
        Sort(sort);
        Disable();
        MTask *enable = MCreateCallTask(this, &DinoHelpBase::Enable);
        MTask *create = new DinoUiOrganizationSlotCreateTask(this);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(create);
        serial->AddTail(enable);
    }
}

// Everything back on, and the links back to how they start.
void DinoUiOrganizationMenu::EnableAll() {
    if (mUnkC0 != NULL) {
        mUnkC0->Enable();
        mUnkC4->Enable();
    }
    int i;
    for (i = 0; i < 5; i++) {
        mListItems[i]->Enable();
    }
    for (i = 0; i < 15; i++) {
        if (i < mSlotCount) {
            if (InTeam(mCreatures[mPage * 15 + i])) {
                mSlots[i]->mMotion->GetUnit()->SetMode(0);
            } else {
                mSlots[i]->mMotion->GetUnit()->SetMode(1);
            }
        }
        mSlots[i]->Enable();
    }
    for (i = 0; i < 5; i++) {
        mButtons[i]->Enable();
    }
    for (i = 0; i < 4; i++) {
        mTabs[i]->Enable();
    }
    mUnkBC->MMenuItem::Enable();
    for (i = 0; i < 4; i++) {
        RemoveLink(mTabs[i], 4);
        AddLink(mTabs[i], 4, mButtons[0]);
    }
    RemoveLink(mUnkB8, 4);
    AddLink(mUnkB8, 4, mButtons[4]);
    RemoveLink(mButtons[3], 2);
    AddLink(mButtons[3], 2, mUnkBC);
    RemoveLink(mTabs[3], 1);
    AddLink(mTabs[3], 1, mUnkBC);
    RemoveLink(mUnkB8, 3);
    AddLink(mUnkB8, 3, mUnkBC);
    for (i = 0; i < 4; i++) {
        RemoveLink(mTabs[i], 2);
        AddLink(mTabs[i], 2, mListItems[0]);
    }
    RemoveLink(mUnkB8, 2);
    AddLink(mUnkB8, 2, mListItems[0]);
}

void DinoUiOrganizationMenu::SelectMember(int index) {
    if (index < 5) {
        mListItems[index]->Disable();
        LinkList();
        for (int i = 0; i < mSlotCount; i++) {
            u32 id = mCreatures[mPage * 15 + i];
            if (InTeam(id)) {
                mSlots[i]->Enable();
            } else {
                DinoCreatureState creature;
                gDinoBackup->GetCreature(id, &creature);
                mSlots[i]->Enable();
            }
        }
        if (index == 0) {
            for (int i = 0; i < 4; i++) {
                RemoveLink(mTabs[i], 2);
                AddLink(mTabs[i], 2, mListItems[1]);
            }
            RemoveLink(mUnkB8, 2);
            AddLink(mUnkB8, 2, mListItems[1]);
        }
    } else {
        if (mUnkC0 != NULL) {
            mUnkC0->Disable();
            mUnkC4->Disable();
        }
        for (int i = 0; i < 15; i++) {
            mSlots[i]->Disable();
        }
        for (int i = 0; i < 5; i++) {
            mButtons[i]->Disable();
        }
        for (int i = 0; i < 4; i++) {
            RemoveLink(mTabs[i], 4);
            AddLink(mTabs[i], 4, mListItems[0]);
        }
        RemoveLink(mUnkB8, 4);
        AddLink(mUnkB8, 4, mListItems[4]);
    }
    mUnkBC->MMenuItem::Disable();
    RemoveLink(mButtons[3], 2);
    AddLink(mButtons[3], 2, mUnkB8);
    RemoveLink(mTabs[3], 1);
    AddLink(mTabs[3], 1, mUnkB8);
    RemoveLink(mUnkB8, 3);
    AddLink(mUnkB8, 3, mTabs[3]);
}

extern const u32 data_0208aa54;
extern const MPoint data_020b5818;

void DinoUiOrganizationMenu::HandleResult() {
    if (mResult == 11) {
        mUnk23C = TRUE;
        func_ov007_0217fb94(this, 1);
        mMessage = new DinoCommonWindowMessage(data_020b5818, gDinoResource->GetText(data_0208aa54), 4, 0);
        mParallel.AddTail(mMessage);
        SelectMember(mSelected);
        mListItems[mSelected]->PlayDecided();
        if (mSelected < 3) {
            mTeamTech->Hide();
        }
        FocusNearestItem(GetCursor());
    } else if (mResult == 12) {
        RemoveMember();
    } else if (mResult == -2) {
        mListItems[mSelected]->PlayFocused();
    }
}

extern const u32 data_0208aa50;

void DinoUiOrganizationMenu::HandleMemberResult() {
    if (mResult == 11) {
        mUnk23C = TRUE;
        func_ov007_0217fb94(this, 2);
        mMessage = new DinoCommonWindowMessage(data_020b5818, gDinoResource->GetText(data_0208aa50), 4, 0);
        mParallel.AddTail(mMessage);
        SelectMember(mSelected);
        mListItems[mSelected]->PlayDecided();
        for (int i = 0; i < mSlotCount; i++) {
            u32 id = mCreatures[mPage * 15 + i];
            if (!InTeam(id)) {
                DinoCreatureState creature;
                gDinoBackup->GetCreature(id, &creature);
                if (creature.parts[0].level != 0) {
                    FocusItem(mSlots[i]);
                    return;
                }
            }
        }
    } else if (mResult == -2) {
        mListItems[mSelected]->PlayFocused();
    }
}

extern DinoUiOrganizationLayout2 data_ov007_0218ec6c;
extern const u32 data_0208aa58, data_0208aa5c, data_0208aa60, data_0208aa64;
extern const u32 data_0208a694, data_0208a698, data_0208a69c, data_0208a6a0;

// The menu for a team member: change and remove, or just add for an empty place.
void DinoUiOrganizationMenu::OpenMemberMenu(int member) {
    mResult = 0;
    mMenuBack->Hide();
    MPoint pos = data_ov007_0218ef0c[member] + data_ov007_0218ec6c.memberMenuOffset;
    if (member == 4) {
        pos.x -= 16;
    }
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(&mResult, pos, NULL, 0);
    MTask *show = mMenuBack->CreateShowTransfer();
    MTask *blend = new DinoUiOrganizationMenuBlendResetTask;
    MTask *task = menu;
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(blend);
    serial->AddTail(show);
    if (mTeam.slots[member] != 0) {
        menu->AddChoice(11, gDinoResource->GetText(data_0208aa60), gDinoResource->GetText(data_0208a69c));
        menu->AddChoice(12, gDinoResource->GetText(data_0208aa5c), gDinoResource->GetText(data_0208a698));
        mSerial.AddTail(MCreateCallTask(this, &DinoUiOrganizationMenu::HandleResult));
    } else {
        menu->AddChoice(11, gDinoResource->GetText(data_0208aa64), gDinoResource->GetText(data_0208a6a0));
        mSerial.AddTail(MCreateCallTask(this, &DinoUiOrganizationMenu::HandleMemberResult));
    }
    menu->AddChoice(-2, gDinoResource->GetText(data_0208aa58), gDinoResource->GetText(data_0208a694));
}

void DinoUiOrganizationMenu::RemoveListLinks() {
    if (mListLinked) {
        RemoveLink(mListItems[0], 3);
        RemoveLink(mListItems[4], 1);
        RemoveLink(mListItems[3], 1);
        RemoveLink(mListItems[1], 3);
        mListLinked = FALSE;
    }
}

void DinoUiOrganizationMenu::RemoveSlotLinks() {
    if (mSlotsLinked) {
        RemoveLink(mSlots[0], 3);
        RemoveLink(mSlots[5], 3);
        RemoveLink(mSlots[10], 3);
        RemoveLink(mSlots[4], 1);
        RemoveLink(mSlots[9], 1);
        RemoveLink(mSlots[14], 1);
        mSlotsLinked = FALSE;
    }
}


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


void DinoUiOrganizationMenu::CreateList() {
    for (int i = 0; i < 5; i++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(mTeam.slots[i], &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        mListItems[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov007_0218ef0c[i], mTeam.slots[i], maxed, 0);
        AddItem(mListItems[i]);
        AddLink(mListItems[i], 4, mUnkB8);
    }
    AddLink(mTabs[0], 2, mListItems[0]);
    AddLink(mTabs[1], 2, mListItems[0]);
    AddLink(mTabs[2], 2, mListItems[0]);
    AddLink(mTabs[3], 2, mListItems[0]);
    AddLink(mUnkBC, 2, mListItems[0]);
    AddLink(mUnkB8, 2, mListItems[0]);
}

void DinoUiOrganizationMenu::ClearList() {
    RemoveLink(mTabs[0], 2);
    RemoveLink(mTabs[1], 2);
    RemoveLink(mTabs[2], 2);
    RemoveLink(mTabs[3], 2);
    RemoveLink(mUnkBC, 2);
    RemoveLink(mUnkB8, 2);
    for (int i = 0; i < 5; i++) {
        RemoveItem(mListItems[i]);
        delete mListItems[i];
    }
}

void DinoUiOrganizationMenu::RestoreFocus() {
    func_ov007_0217fb94(this, 0);
    EnableAll();
    if (mSelected < 5) {
        mListItems[mSelected]->PlayNormal();
        FocusItem(mListItems[mSelected]);
    } else {
        mSlots[mSelected - 5]->PlayNormal();
        FocusItem(mSlots[mSelected - 5]);
    }
    UpdateTeamTech();
}

extern DinoUiOrganizationLayout data_ov007_0218ebec;

void DinoUiOrganizationMenu::PlaceSlots() {
    if (mPage == mLastPage) {
        mSlotCount = mCreatures.size() - mPage * 15;
    } else {
        mSlotCount = 15;
    }
    for (int i = 0; i < mSlotCount; i++) {
        u32 id = mCreatures[mPage * 15 + i];
        int row = i / 5;
        int column = i % 5;
        mSlots[i]->mMotion->SetPosition(MPoint(
            data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * column + data_ov007_0218ebec.slotRowX * row,
            data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * column + data_ov007_0218ebec.slotRowY * row));
        if (InTeam(id)) {
            mSlots[i]->mMotion->GetUnit()->SetMode(0);
        } else {
            mSlots[i]->mMotion->GetUnit()->SetMode(1);
        }
        mSlots[i]->Enable();
    }
}

// Where a medal flies to when its creature is on an earlier or a later page.
extern const MPoint data_ov007_0218ecbc;
extern const MPoint data_ov007_0218ecc4;

// Sends the selected member back to its place in the list.
// NONMATCHING: the game reloads the list's size each time round the search loop.
void DinoUiOrganizationMenu::RemoveMember() {
    int i = FindCreature(mTeam.slots[mSelected]);
    mTeam.slots[mSelected] = 0;
    if (i / 15 < mPage) {
        mSerial.AddTail(mListItems[mSelected]->CreateFlyInTask(data_ov007_0218ecbc));
    } else if (i / 15 > mPage) {
        mSerial.AddTail(mListItems[mSelected]->CreateFlyInTask(data_ov007_0218ecc4));
    } else {
        int index = i % 15;
        int row = index / 5;
        int column = index % 5;
        MPoint pos;
        pos.x = data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * column + data_ov007_0218ebec.slotRowX * row;
        pos.y = data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * column + data_ov007_0218ebec.slotRowY * row;
        mSerial.AddTail(mListItems[mSelected]->CreateFlyInTask(pos));
    }
    mSerial.AddTail(MCreateCallTask(this, &DinoUiOrganizationMenu::Refresh));
    mUnk230->Clear();
    Disable();
}

void DinoUiOrganizationMenu::LinkList() {
    if (mListLinked) {
        RemoveListLinks();
    }
    if (!mListItems[0]->IsEnabled()) {
        AddLink(mListItems[1], 3, mListItems[4]);
        AddLink(mListItems[4], 1, mListItems[1]);
        AddLink(mListItems[0], 3, mListItems[4]);
        AddLink(mListItems[3], 1, mListItems[4]);
    } else if (!mListItems[4]->IsEnabled()) {
        AddLink(mListItems[0], 3, mListItems[3]);
        AddLink(mListItems[3], 1, mListItems[0]);
        AddLink(mListItems[1], 3, mListItems[0]);
        AddLink(mListItems[4], 1, mListItems[0]);
    } else {
        AddLink(mListItems[0], 3, mListItems[4]);
        AddLink(mListItems[4], 1, mListItems[0]);
        AddLink(mListItems[3], 1, mListItems[4]);
        AddLink(mListItems[1], 3, mListItems[0]);
    }
    mListLinked = TRUE;
}

void DinoUiOrganizationMenu::LinkSlots() {
    if (mSlotsLinked) {
        RemoveSlotLinks();
    }
    AddLink(mSlots[0], 3, mSlots[4]);
    AddLink(mSlots[5], 3, mSlots[9]);
    AddLink(mSlots[10], 3, mSlots[14]);
    AddLink(mSlots[4], 1, mSlots[0]);
    AddLink(mSlots[9], 1, mSlots[5]);
    AddLink(mSlots[14], 1, mSlots[10]);
    mSlotsLinked = TRUE;
}

void DinoUiOrganizationMenu::UpdateTeamTech() {
    if (mTeam.slots[0] != 0) {
        mUnkBC->mMotion->GetUnit()->SetMode(1);
    } else {
        mUnkBC->mMotion->GetUnit()->SetMode(0);
    }
    if (DinoTeamHasTech(&mTeam)) {
        mTeamTech->Show();
    } else {
        mTeamTech->Hide();
    }
}

void DinoUiOrganizationMenu::Refresh() {
    RemoveListLinks();
    RemoveSlotLinks();
    ClearList();
    CreateList();
    PlaceSlots();
    EnableAll();
    LinkList();
    LinkSlots();
    Enable();
    UpdateTeamTech();
    FocusNearestItem(GetCursor());
}

extern "C" MTask *func_ov007_0217d1c0(DinoUiOrganizationMenu *menu, DinoTeam *saved);
extern const u32 data_0208aa38, data_0208aa3c;

// Puts a moving member back, or leaves (saving the team if it changed).
void DinoUiOrganizationMenu::Back() {
    MTask *exit, *mainFade, *subFade, *wait;
    if (mUnk23C == TRUE) {
        mUnk23C = FALSE;
        mMessage->Exit();
        RemoveListLinks();
        RemoveSlotLinks();
        LinkList();
        LinkSlots();
        MTask *restore = MCreateCallTask(this, &DinoUiOrganizationMenu::RestoreFocus);
        MTask *wait = MTaskSetWaitEmptyTask::Create(&mUnkB8->mSerial);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(restore);
        return;
    }
    Disable();
    mMenuBack->Hide();
    if (mUnk23D) {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208aa38), 4));
    } else {
        DinoTeam saved;
        gDinoBackup->GetTeam(mTeamIndex, &saved);
        if (DinoTeamDiffers(&mTeam, &saved)) {
            mSerial.AddTail(
                new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208aa3c), 4));
            mSerial.AddTail(func_ov007_0217d1c0(this, &saved));
        }
    }
    exit = CreateExitTransfer();
    wait = mInfo->CreateWaitModelTask();
    subFade = gLcd->mSub->mBright->CreateFadeOutToBlack(30, 0);
    mainFade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(mainFade);
    fades->AddTail(subFade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void DinoUiOrganizationMenu::CreateSlots() {
    for (int i = 0; i < 15; i++) {
        CreateSlot(i);
    }
}

DinoUiOrganizationSlotCreateTask::DinoUiOrganizationSlotCreateTask(DinoUiOrganizationMenu *menu) : MProcess(NULL) {
    mMenu = menu;
    mIndex = 0;
}

#pragma auto_inline on

void DinoUiOrganizationSlotCreateTask::Idle() {
    if (mIndex < 15) {
        mMenu->CreateSlot(mIndex);
        if (mIndex < mMenu->mSlotCount - 1) {
            mSerial.AddTail(new MTimer(data_02089a0c));
        }
    } else {
        Exit();
    }
    mIndex++;
}

// DinoUiOrganizationMenu::OnFocus (not decompiled yet) re-enables the help through a call task.
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)());
template MTask *MCreateBgCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(), int thread);
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(u8), u8 a1);
