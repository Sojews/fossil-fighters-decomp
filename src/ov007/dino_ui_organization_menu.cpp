// DinoUiOrganizationMenu (team organization).
// NONMATCHING: RemoveMember and SetMember (the search loop); the rest (OnStart, OnFocus, SetMember, UpdateDetails...) is to do.

#pragma ipa file

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "game/dino_sound.h"
#include "game/dino_ui_organization_menu.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "game/dino_ui_field_menu_cursor.h"
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
#include "game/dino_ui_medal_menu.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "game/dino_creature_sort.h"
#include "m/indicator.h"
#include <printf.h>
#include "game/dino_ui_parts_reviver_name.h"

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


void DinoUiOrganizationOKButton::OnDecide() {
    mMotion->Play(4, 1);
    MTask *play = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    mOwner->Confirm();
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

DinoUiOrganizationMenuItem::DinoUiOrganizationMenuItem(DinoUiOrganizationMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mMotion->GetUnit()->SetMode(1);
}

// The call-task makers, instantiated ahead of their users so none directly follows a function that calls it.
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)());
template MTask *MCreateBgCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(), int thread);
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(u8), u8 a1);

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

extern DinoUiOrganizationLayout data_ov007_0218ebec;
extern DinoUiOrganizationLayout2 data_ov007_0218ec6c;
// Where the cursor sits on each tab.
extern MPoint data_020b5890[4];
// (The pooled method pointers this function's call tasks read.)
extern "C" struct {
    void (DinoHelpBase::*Enable)();
} data_ov007_0218ecd4, data_ov007_0218ed4c;

// Moves the cursor to the item. A team member or a page's slot also shows its creature's name, information and
// details (and raises the team tech's member); an empty one hides them.
void DinoUiOrganizationMenu::OnFocus(MMenuItem *item) {
    int i;
    mUnk23E = FALSE;
    mFocusTabs = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (i = 0; i < 4; i++) {
        if (item == mTabs[i]) {
            frame.MoveTo(data_020b5890[i]);
            mCursor->SetMode(2);
            mFocusTabs = TRUE;
        }
    }
    for (i = 0; i < 5; i++) {
        if (item == mListItems[i]) {
            frame.Offset(data_ov007_0218ec6c.cursorOffset);
        }
    }
    for (i = 0; i < 15; i++) {
        if (item == mSlots[i]) {
            frame.Offset(data_ov007_0218ec6c.cursorOffset);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
    mTeamTech->Lower();
    for (i = 0; i < 5; i++) {
        if (item == mListItems[i]) {
            if (mTeam.slots[i] != 0) {
                mName->SetCreature(mTeam.slots[i]);
                mName->SetPosition(data_ov007_0218ef0c[i] + data_ov007_0218ec6c.nameOffset);
                DinoCreatureState creature;
                gDinoBackup->GetCreature(mTeam.slots[i], &creature);
                mInfo->Unk02156ee8(*(DinoCreatureInfo *)&creature);
                MTask *enable = MCreateCallTask(this, data_ov007_0218ecd4.Enable);
                mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial), enable);
                Disable();
                UpdateDetails(mTeam.slots[i]);
                if (i < 3) {
                    mTeamTech->Raise((u8)i);
                }
            } else {
                mName->Hide();
                mInfo->Unk021571bc();
                UpdateDetails(1);
            }
            return;
        }
    }
    for (i = 0; i < 15; i++) {
        if (item == mSlots[i]) {
            if (i < mSlotCount) {
                mName->SetCreature(mCreatures[mPage * 15 + i]);
                mName->SetPosition(MPoint(data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * (i % 5) +
                                              data_ov007_0218ebec.slotRowX * (i / 5),
                                          data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * (i % 5) +
                                              data_ov007_0218ebec.slotRowY * (i / 5)) +
                                   data_ov007_0218ec6c.nameOffset);
                DinoCreatureState creature;
                gDinoBackup->GetCreature(mCreatures[mPage * 15 + i], &creature);
                mInfo->Unk02156ee8(*(DinoCreatureInfo *)&creature);
                MTask *enable = MCreateCallTask(this, data_ov007_0218ed4c.Enable);
                mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial), enable);
                Disable();
                UpdateDetails(mCreatures[mPage * 15 + i]);
            } else {
                mName->Hide();
                mInfo->Unk021571bc();
                UpdateDetails(1);
            }
            return;
        }
    }
    mName->Hide();
}

// Everything back on, and the links back to how they start.
void DinoUiOrganizationMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 4; i++) {
        if (item == mTabs[i]) {
            SetTab(i);
        }
    }
    for (int i = 0; i < 5; i++) {
        if (item == mButtons[i]) {
            SetSort(i);
        }
    }
    if (mUnk23C == TRUE) {
        if (mSelected < 5) {
            for (int i = 0; i < 5; i++) {
                if (i != mSelected && item == mListItems[i]) {
                    if (mTeam.slots[mSelected] != 0 || mTeam.slots[i] != 0) {
                        SwapMember(i);
                    }
                    return;
                }
            }
            for (int i = 0; i < mSlotCount; i++) {
                if (item == mSlots[i] && mUnk23E && !InTeam(mCreatures[mPage * 15 + i])) {
                    PlaceSlot(i);
                    return;
                }
            }
        } else {
            for (int i = 0; i < 5; i++) {
                if (item == mListItems[i]) {
                    PlaceMember(i);
                    return;
                }
            }
        }
    } else {
        for (int i = 0; i < 5; i++) {
            if (item == mListItems[i]) {
                if (mUnk23E) {
                    mSelected = i;
                    OpenMemberMenu(i);
                }
                return;
            }
        }
        for (int i = 0; i < mSlotCount; i++) {
            if (item == mSlots[i]) {
                if (mUnk23E && !InTeam(mCreatures[mPage * 15 + i])) {
                    PickSlot(i);
                }
                return;
            }
        }
    }
}

void DinoUiOrganizationMenu::SwapMember(int member) {
    mUnk23C = FALSE;
    func_ov007_0217fb94(this, 0);
    mMessage->mUnk98 = 0xF;
    mName->Hide();
    MParallelTaskSet *moves = new MParallelTaskSet(TRUE);
    if (mTeam.slots[member] != 0) {
        mListItems[member]->PlayDecided();
        moves->AddTail(mListItems[member]->CreateFlyInTask(data_ov007_0218ef0c[mSelected]));
    } else {
        mListItems[member]->PlayNormal();
    }
    if (mTeam.slots[mSelected] != 0) {
        moves->AddTail(mListItems[mSelected]->CreateFlyInTask(data_ov007_0218ef0c[member]));
    } else {
        mListItems[mSelected]->PlayNormal();
    }
    MTask *exit;
    MTask *refresh = MCreateCallTask(this, &DinoUiOrganizationMenu::Refresh);
    exit = MTaskExitTransfer::Create(mMessage);
    MTask *timer = new MTimer(5);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(moves);
    serial->AddTail(exit);
    serial->AddTail(refresh);
    mUnk230->Clear();
    Disable();
    std::swap(mTeam.slots[mSelected], mTeam.slots[member]);
}

void DinoUiOrganizationMenu::PlaceSlot(int slot) {
    DinoCreatureState creature;
    gDinoBackup->GetCreature(mCreatures[mPage * 15 + slot], &creature);
    if (creature.parts[0].level != 0) {
        mMessage->mUnk98 = 0xF;
        mSlots[slot]->PlayDecided();
        SetMember(slot + mPage * 15, mSelected);
        mName->Hide();
    }
}

void DinoUiOrganizationMenu::PlaceMember(int member) {
    mMessage->mUnk98 = 0xF;
    mListItems[member]->PlayDecided();
    SetMember(mSelected + mPage * 15 - 5, member);
    mName->Hide();
}

// Loads the motions of the page's slots.
void DinoUiOrganizationMenu::LoadPage() {
    int count;
    if (mPage == mLastPage) {
        count = mCreatures.size() - mPage * 15;
    } else {
        count = 15;
    }
    mMotions.clear();
    for (int i = 0; i < count; i++) {
        char path[0x100];
        OS_SPrintf(path, "/motion/ui_reviver_medal/reviver_%03d.bin", mCreatures[mPage * 15 + i]);
        mMotions.push_back(MMotionFile(std::string(path), 7));
    }
}

void DinoUiOrganizationMenu::LoadCreatures() {
    mCreatures.clear();
    for (u32 id = 1; id <= gDinoResource->GetCreatureCount() - 1; id++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(id, &creature);
        if (creature.parts[0].level != 0 ||
            creature.parts[1].level + creature.parts[2].level + creature.parts[3].level != 0) {
            u32 index = creature.index;
            mCreatures.push_back(index);
        }
    }
    DinoCreatureByVoice byVoice;
    std::stable_sort(mCreatures.begin(), mCreatures.end(), byVoice);
    mByVoice = mCreatures;
    mByUnk88 = mCreatures;
    DinoCreatureByUnk88 byUnk88;
    std::stable_sort(mByUnk88.begin(), mByUnk88.end(), byUnk88);
    mByLevel = mCreatures;
    DinoCreatureByLevel byLevel;
    std::stable_sort(mByLevel.begin(), mByLevel.end(), byLevel);
    mByUnk02031104 = mCreatures;
    DinoCreatureByUnk02031104 byUnk02031104;
    std::stable_sort(mByUnk02031104.begin(), mByUnk02031104.end(), byUnk02031104);
    mByUnk0D = mCreatures;
    DinoCreatureByUnk0D byUnk0D;
    std::stable_sort(mByUnk0D.begin(), mByUnk0D.end(), byUnk0D);
    if (mCreatures.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mCreatures.size() - 1) / 15;
    }
    mFraction->SetDenominator(mLastPage + 1);
}

DinoUiOrganizationMenu::DinoUiOrganizationMenu(u32 team)
    : MMenu(gLcd->mSub, NULL, 0), mUnkC0(NULL), mTabPicture(NULL), mTeamIndex(team), mResult(0), mPage(0),
      mLastPage(0), mTab(0xFF), mSort(0), mFocusTabs(FALSE), mSelected(0), mUnk23C(FALSE), mUnk23D(0), mUnk23E(0),
      mUnk254(0xFF), mUnk255(0), mListLinked(FALSE), mSlotsLinked(FALSE) {
    mMotions.clear();
}

// Lists the creatures in the sort's order.
void DinoUiOrganizationMenu::Sort(u8 sort) {
    if (sort > 4) {
        return;
    }
    switch (sort) {
    case 0:
        mCreatures = mByVoice;
        break;
    case 1:
        mCreatures = mByUnk88;
        break;
    case 2:
        mCreatures = mByLevel;
        break;
    case 3:
        mCreatures = mByUnk02031104;
        break;
    case 4:
        mCreatures = mByUnk0D;
        break;
    }
}

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

void DinoUiOrganizationMenu::PickSlot(int slot) {
    DinoCreatureState creature;
    gDinoBackup->GetCreature(mCreatures[mPage * 15 + slot], &creature);
    if (creature.parts[0].level != 0) {
        mSelected = slot + 5;
        mUnk23C = TRUE;
        func_ov007_0217fb94(this, 1);
        mMessage = new DinoCommonWindowMessage(data_020b5818, gDinoResource->GetText(data_0208aa54), 4, 0);
        mParallel.AddTail(mMessage);
        SelectMember(mSelected);
        FocusItem(mListItems[0]);
        mSlots[slot]->PlayDecided();
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
    mCursor->Hide();
    MPoint pos = data_ov007_0218ef0c[member] + data_ov007_0218ec6c.memberMenuOffset;
    if (member == 4) {
        pos.x -= 16;
    }
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(&mResult, pos, NULL, 0);
    MTask *show = mCursor->CreateShowTransfer();
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

// Makes the page's slot (the first clears the numbers and counts the slots; the last links them).
void DinoUiOrganizationMenu::CreateSlot(int slot) {
    if (slot == 0) {
        if (mPage == mLastPage) {
            mSlotCount = mCreatures.size() - mPage * 15;
        } else {
            mSlotCount = 15;
        }
        for (int i = 0; i < 15; i++) {
            mNumbers[i]->SetText(std::string("   "));
        }
    }
    if (slot < mSlotCount) {
        u32 id = mCreatures[mPage * 15 + slot];
        DinoCreatureState creature;
        gDinoBackup->GetCreature(id, &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        mSlots[slot] = new DinoUiPartsReviverMedalButton(
            mOam,
            MPoint(data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * (slot % 5) +
                       data_ov007_0218ebec.slotRowX * (slot / 5),
                   data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * (slot % 5) +
                       data_ov007_0218ebec.slotRowY * (slot / 5)),
            id, maxed, creature.parts[0].level == 0);
        AddItem(mSlots[slot]);
        if (InTeam(id)) {
            mSlots[slot]->mMotion->GetUnit()->SetMode(0);
        } else {
            mSlots[slot]->mMotion->GetUnit()->SetMode(1);
        }
        mSlots[slot]->Enable();
        char number[4];
        sprintf(number, "%03d", gDinoResource->GetCreatureDef(id)->voice);
        mNumbers[slot]->SetText(std::string(number));
    } else {
        mSlots[slot] = new DinoUiPartsReviverMedalButton(
            mOam,
            MPoint(data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * (slot % 5) +
                       data_ov007_0218ebec.slotRowX * (slot / 5),
                   data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * (slot % 5) +
                       data_ov007_0218ebec.slotRowY * (slot / 5)),
            0xFFFF, TRUE, FALSE);
        AddItem(mSlots[slot]);
    }
    if (slot == 14) {
        LinkSlots();
    }
}

// Where a medal flies to when its creature is on an earlier or a later page.
extern const MPoint data_ov007_0218ecbc;
extern const MPoint data_ov007_0218ecc4;

// Sends the selected member back to its place in the list.
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

void DinoUiOrganizationMenu::SetMember(u8 index, u8 member) {
    mUnk23C = FALSE;
    func_ov007_0217fb94(this, 0);
    u8 old = mTeam.slots[member];
    int i = FindCreature(old);
    mTeam.slots[member] = mCreatures[index];
    MParallelTaskSet *moves = new MParallelTaskSet(TRUE);
    moves->AddTail(mSlots[index % 15]->CreateFlyInTask(data_ov007_0218ef0c[member]));
    if (old != 0) {
        if (i / 15 < mPage) {
            moves->AddTail(mListItems[member]->CreateFlyInTask(data_ov007_0218ecbc));
        } else if (i / 15 > mPage) {
            moves->AddTail(mListItems[member]->CreateFlyInTask(data_ov007_0218ecc4));
        } else {
            int slot = i % 15;
            int row = slot / 5;
            int column = slot % 5;
            MPoint pos;
            pos.x = data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * column + data_ov007_0218ebec.slotRowX * row;
            pos.y = data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * column + data_ov007_0218ebec.slotRowY * row;
            moves->AddTail(mListItems[member]->CreateFlyInTask(pos));
        }
    } else {
        mListItems[member]->PlayNormal();
    }
    MTask *exit = MTaskExitTransfer::Create(mMessage);
    MTask *timer = new MTimer(5);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(moves);
    serial->AddTail(exit);
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

extern const u32 data_0208a6a4, data_0208a6a8, data_0208a6ac, data_0208a6b0, data_0208a6b4, data_0208a6b8, data_0208a6bc, data_0208a6c0, data_0208a6c4, data_0208a6c8, data_0208a6cc, data_0208a6d0, data_0208a6d4, data_0208a6d8, data_0208a6dc, data_0208a6e0, data_0208a6e4, data_0208a6e8, data_0208a6ec, data_0208a6f0, data_0208a6f4, data_0208a6f8, data_0208a6fc, data_0208a700, data_0208a704, data_0208a74c, data_0208a750, data_0208a754, data_0208a758, data_0208a75c, data_0208a760, data_0208a764, data_0208a768, data_0208a76c;
extern MRect data_020b5950, data_020b5960, data_020b5970, data_020b5980, data_020b5990, data_020b59a0, data_020b59b0, data_020b59c0, data_020b59d0, data_020b59e0, data_020b59f0, data_020b5a00, data_020b5a10, data_020b5a20, data_020b5a30, data_020b5a40, data_020b5a50, data_020b5a60, data_020b5a70, data_020b5a80;

// The help areas of the top screen's details panel, by what it shows (as DinoUiOrderSelectMenu::ShowPage).
void DinoUiOrganizationMenu::UpdateDetails(u8 page) {
    u8 mode;
    if (page == 0) {
        return;
    }
    if (page != 0xFF) {
        mUnk255 = page;
    }
    mode = 0;
    switch (mTab) {
    case 0:
        mode = 0;
        break;
    case 1:
    case 2:
        mode = 1;
        break;
    case 3:
        if (mUnk255 != 0) {
            int index = (mUnk255 - 1) * 4 + 1;
            u8 uncleanable = mKasekiDefs.Get()->GetDef(index)->IsUncleanable();
            u32 part = mKasekiDefs.Get()->GetDef(index)->GetUnk38();
            if (uncleanable) {
                mode = part + 2;
            } else {
                mode = 2;
            }
        } else {
            mode = 2;
        }
        break;
    }
    if (mUnk254 == mode) {
        return;
    }
    mUnk254 = mode;
    mUnk24C->Clear();
    switch (mUnk254) {
    case 0:
        mUnk24C->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5950, std::string(gDinoResource->GetText(data_0208a76c))));
        mUnk24C->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5960, std::string(gDinoResource->GetText(data_0208a768))));
        mUnk24C->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5970, std::string(gDinoResource->GetText(data_0208a764))));
        mUnk24C->AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5980, std::string(gDinoResource->GetText(data_0208a760))));
        mUnk24C->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5990, std::string(gDinoResource->GetText(data_0208a75c))));
        mUnk24C->AddTail(mPageHelps[5] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59a0, std::string(gDinoResource->GetText(data_0208a758))));
        mUnk24C->AddTail(mPageHelps[6] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59b0, std::string(gDinoResource->GetText(data_0208a754))));
        mUnk24C->AddTail(mPageHelps[7] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59c0, std::string(gDinoResource->GetText(data_0208a750))));
        mUnk24C->AddTail(mPageHelps[8] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59d0, std::string(gDinoResource->GetText(data_0208a74c))));
        AddEntry((u32)mHelp26C, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mHelp270, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[0], 2, (u32)mPageHelps[1]);
        AddEntry((u32)mPageHelps[0], 3, 0);
        AddEntry((u32)mPageHelps[0], 1, 0);
        AddEntry((u32)mPageHelps[1], 4, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[1], 1, (u32)mPageHelps[2]);
        AddEntry((u32)mPageHelps[2], 1, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        AddEntry((u32)mPageHelps[5], 2, (u32)mPageHelps[7]);
        AddEntry((u32)mPageHelps[6], 1, 0);
        AddEntry((u32)mPageHelps[7], 3, 0);
        AddEntry((u32)mPageHelps[7], 1, 0);
        AddEntry((u32)mPageHelps[8], 3, 0);
        AddEntry((u32)mPageHelps[8], 1, 0);
        break;
    case 1:
        mUnk24C->AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59e0, std::string(gDinoResource->GetText(data_0208a704))));
        mUnk24C->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59f0, std::string(gDinoResource->GetText(data_0208a700))));
        AddEntry((u32)mPageHelps[1], 2, (u32)mHelps[0]);
        break;
    case 2:
        mUnk24C->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a00, std::string(gDinoResource->GetText(data_0208a6fc))));
        mUnk24C->AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a10, std::string(gDinoResource->GetText(data_0208a6f8))));
        mUnk24C->AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a20, std::string(gDinoResource->GetText(data_0208a6f4))));
        mUnk24C->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a30, std::string(gDinoResource->GetText(data_0208a6f0))));
        mUnk24C->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6e0))));
        mUnk24C->AddTail(mPageHelps[5] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6ec))));
        mUnk24C->AddTail(mPageHelps[6] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6e8))));
        mUnk24C->AddTail(mPageHelps[7] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6e4))));
        AddEntry((u32)mPageHelps[0], 2, (u32)mPageHelps[4]);
        AddEntry((u32)mPageHelps[3], 2, (u32)mPageHelps[4]);
        AddEntry((u32)mPageHelps[4], 4, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[4], 3, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        AddEntry((u32)mPageHelps[5], 3, 0);
        AddEntry((u32)mPageHelps[5], 1, 0);
        AddEntry((u32)mPageHelps[6], 3, 0);
        AddEntry((u32)mPageHelps[6], 1, 0);
        AddEntry((u32)mPageHelps[7], 3, 0);
        AddEntry((u32)mPageHelps[7], 1, 0);
        break;
    case 3:
    case 4:
    case 5:
        switch (mUnk254) {
        case 3:
            mUnk24C->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a6dc))));
            mUnk24C->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6cc))));
            mUnk24C->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6d8))));
            mUnk24C->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6d4))));
            mUnk24C->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6d0))));
            break;
        case 4:
            mUnk24C->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a6c8))));
            mUnk24C->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6c4))));
            mUnk24C->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6c0))));
            mUnk24C->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6bc))));
            mUnk24C->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6b8))));
            break;
        case 5:
            mUnk24C->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a6b4))));
            mUnk24C->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6b0))));
            mUnk24C->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6ac))));
            mUnk24C->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6a8))));
            mUnk24C->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6a4))));
            break;
        }
        AddEntry((u32)mHelp26C, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[0], 3, 0);
        AddEntry((u32)mPageHelps[0], 1, 0);
        AddEntry((u32)mPageHelps[1], 3, 0);
        AddEntry((u32)mPageHelps[1], 1, 0);
        AddEntry((u32)mPageHelps[2], 3, 0);
        AddEntry((u32)mPageHelps[2], 1, 0);
        AddEntry((u32)mPageHelps[3], 3, 0);
        AddEntry((u32)mPageHelps[3], 1, 0);
        AddEntry((u32)mPageHelps[4], 3, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        break;
    }
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

// Fades out and exits once the 3D model is done.
inline void DinoUiOrganizationMenu::Leave() {
    MTask *exit = CreateExitTransfer();
    MTask *wait = mInfo->CreateWaitModelTask();
    MTask *subFade = gLcd->mSub->mBright->CreateFadeOutToBlack(30, 0);
    MTask *mainFade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(mainFade);
    fades->AddTail(subFade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(wait);
    serial->AddTail(exit);
}

extern const u32 data_0208aa40, data_0208aa4c;

// Saves the team and leaves; a team needs its leader.
void DinoUiOrganizationMenu::Confirm() {
    if (mUnk23C == TRUE) {
        return;
    }
    if (mTeam.slots[0] != 0) {
        DinoTeam saved;
        gDinoBackup->GetTeam(mTeamIndex, &saved);
        if (DinoTeamDiffers(&mTeam, &saved)) {
            gDinoBackup->SetTeam(mTeamIndex, &mTeam);
            mCursor->Hide();
            mSerial.AddTail(
                new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208aa40), 0x78, 4));
            Leave();
        } else {
            mCursor->Hide();
            Leave();
        }
    } else {
        mSerial.AddTail(
            new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208aa4c), 0x78, 4));
        mCursor->Hide();
        mSerial.AddTail(mCursor->CreateShowTransfer());
    }
}

// Puts a moving member back, or leaves (saving the team if it changed).
void DinoUiOrganizationMenu::Back() {
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
    mCursor->Hide();
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
    Leave();
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

// The sort and tab items (see dino_ui_medal_menu.cpp and dino_ui_parts_tab_icon_menu_item.cpp) are compiled into
// this file again.

DinoUiSortMenuItem::~DinoUiSortMenuItem() {
}

DinoUiPartsTabIconMenuItem::~DinoUiPartsTabIconMenuItem() {
}

MRect DinoUiSortMenuItem::GetRect() {
    int y = mPos.y;
    int x = mPos.x;
    return MRect(MPoint(x, y - 2), MPoint(x + 0x28, y + 0x10));
}

MDisplay *DinoUiSortMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoUiSortMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiSortMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiSortMenuItem::OnDecide() {
}

void DinoUiSortMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiSortMenuItem::GetFrame() {
    return MRect(mPos, MPoint(mPos.x + 0x28, mPos.y + 0xE));
}

MRect DinoUiPartsTabIconMenuItem::GetRect() {
    return GetFrame();
}

MRect DinoUiPartsTabIconMenuItem::GetFrame() {
    return mFrame;
}

MDisplay *DinoUiPartsTabIconMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoUiPartsTabIconMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiPartsTabIconMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiPartsTabIconMenuItem::OnDecide() {
}

void DinoUiPartsTabIconMenuItem::OnFocusChanged(BOOL value) {
}

#pragma auto_inline off
// Vector members the game emits in this file (made explicitly until the code that uses them is decompiled).
template void std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::sub_push_back(const MMotionFile &x, Metrowerks::int2type<false>);
template void std::__vec_deleter<u32, std::allocator<u32> >::assign<u32 *>(u32 *first, u32 *last, Metrowerks::int2type<true>);

// The list's sorts (the game emits the whole stable_sort machinery here, after SetSort's caller).
// NONMATCHING: std::__inplace_merge, std::__stable_sort_move, std::__insertion_sort_move and std::get_temporary_buffer,
// as in ov006_02178b80.cpp.
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk0D);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk02031104);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByLevel);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk88);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByVoice);
