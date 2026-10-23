// DinoUiOrganizationMenu (team organization).
// NONMATCHING: RemoveMember and SetMember (the search loop).

#pragma ipa file

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "game/dino_sound.h"
#include "m/smart_oam.h"
#include "game/dino_ui_common.h"
#include "m/oam_canvas.h"
#include "m/obj.h"
#include "m/font.h"
#include "game/dino_static_init.h"
#include "game/dino_guide_message.h"
#include "game/dino_ui_parts_black_belt.h"
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

// The call-task makers, instantiated ahead of their users so none directly follows a function that calls it.
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)());
template MTask *MCreateBgCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(), int thread);
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(u8), u8 a1);

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
    SetHelpMode(0);
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

// OnStart's pooled statics: path pools, positions and text ids.
extern "C" const char *data_ov007_0218e9ec[], *data_ov007_0218ea6c[], *data_ov007_0218eb6c[];
extern "C" u32 data_ov007_02192640[2];  // the two static-init guards
extern "C" u32 data_ov007_0218edec[];
extern MPoint data_ov007_0218ec8c;      // the page fraction's place
extern MPoint data_ov007_0218ec4c;      // the team name's place
extern MPoint data_ov007_0218ee74[];    // the team tech's places
extern "C" const u32 data_ov007_0218ee50[4], data_ov007_0218ee60[5];  // the tabs' and sort buttons' texts
extern const MRect data_020b58e0[4];    // the tabs' frames
extern const MRect data_020b5920, data_020b5930, data_020b5940;
extern "C" const char data_ov007_02190ef4[], data_ov007_02190f08[];  // the sort cursor's picture, the numbers' font
extern const u32 data_0208a708, data_0208a70c, data_0208a710, data_0208a714, data_0208a718, data_0208a71c, data_0208a720,
    data_0208a724, data_0208a728, data_0208a72c, data_0208a730, data_0208a734, data_0208a748, data_0208a770,
    data_0208a774, data_0208a778, data_0208a948, data_0208aa34, data_0208aa74;

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

inline DinoUiOrganizationPageLButton::DinoUiOrganizationPageLButton(DinoUiOrganizationMenu *owner)
    : DinoUiOrganizationMenuItem(owner, data_ov007_0218e9ec[14], data_ov007_0218ec6c.pageLPos, data_0208aa34) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiOrganizationPageRButton::DinoUiOrganizationPageRButton(DinoUiOrganizationMenu *owner)
    : DinoUiOrganizationMenuItem(owner, data_ov007_0218eb6c[17], data_ov007_0218ec6c.pageRPos, data_0208aa34) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiOrganizationBackButton::DinoUiOrganizationBackButton(DinoUiOrganizationMenu *owner)
    : DinoUiOrganizationMenuItem(owner, data_ov007_0218ea6c[13], data_ov007_0218ec6c.buttonPos, data_0208a70c) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
}

inline DinoUiOrganizationOKButton::DinoUiOrganizationOKButton(DinoUiOrganizationMenu *owner)
    : DinoUiOrganizationMenuItem(owner, data_ov007_0218eb6c[22], data_ov007_0218ec6c.buttonPos, data_0208a708) {
    SetKeys(8);
    mMotion->Play(2, 0);
}

// &mParallel through a non-member inline: the numbers loop hoists it into the frame's lowest slot, as the game does.
static inline MTaskSet *OrganizationParallel(DinoUiOrganizationMenu *menu) {
    return &menu->mParallel;
}

// The page fraction and buttons, the sort cursor, the help areas, the back and OK buttons, the tabs and sort buttons,
// the team's name, the cursor, the slots' numbers, the name, the information, the black belt and the team tech; then
// the list and slots, and a fade in. With no team members yet, a message says so and the slots get the focus.
#pragma opt_common_subs off
void DinoUiOrganizationMenu::OnStart() {
    u32 guard = data_ov007_02192640[1];
    if (!(guard & 1)) {
        DINO_POOL_WORD(data_ov007_0218edec, 0x64) = data_0208a71c;
        DINO_POOL_WORD(data_ov007_0218edec, 0x68) = data_0208a718;
        DINO_POOL_WORD(data_ov007_0218edec, 0x6c) = data_0208a714;
        DINO_POOL_WORD(data_ov007_0218edec, 0x70) = data_0208a710;
        DINO_POOL_WORD(data_ov007_02192640, 4) = guard | 1;
    }
    guard = data_ov007_02192640[0];
    if (!(guard & 1)) {
        DINO_POOL_WORD(data_ov007_0218edec, 0x74) = data_0208a730;
        DINO_POOL_WORD(data_ov007_0218edec, 0x78) = data_0208a72c;
        DINO_POOL_WORD(data_ov007_0218edec, 0x7c) = data_0208a728;
        DINO_POOL_WORD(data_ov007_0218edec, 0x80) = data_0208a724;
        DINO_POOL_WORD(data_ov007_0218edec, 0x84) = data_0208a720;
        DINO_POOL_WORD(data_ov007_02192640, 0) = guard | 1;
    }
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    gDinoBackup->GetTeam(mTeamIndex, &mTeam);
    mOam = new MSmartOam(0x3B, 0x33, TRUE);
    mFraction = new DinoUiFieldMenuFraction(mOam, data_ov007_0218ec8c);
    mParallel.AddTail(mFraction);
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetPriority(2);
    LoadCreatures();
    if (mLastPage > 0) {
        mUnkC0 = new DinoUiOrganizationPageLButton(this);
        AddItem(mUnkC0);
        mUnkC4 = new DinoUiOrganizationPageRButton(this);
        AddItem(mUnkC4);
    }
    mParallel.AddTail(mSortCursor = new MTextPicture(std::string(data_ov007_02190ef4), TRUE, false));
    mSortCursor->Show();
    mSortCursor->SetPriority(2);
    mUnk24C = new MParallelTaskSet();
    mParallel.AddTail(mUnk24C);
    mUnk250 = new MParallelTaskSet();
    mParallel.AddTail(mUnk250);
    mParallel.AddTail(mHelp26C = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5920,
                                                       std::string(gDinoResource->GetText(data_0208a778))));
    mParallel.AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5930,
                                            std::string(gDinoResource->GetText(data_0208a774))));
    mParallel.AddTail(mHelp270 = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5940,
                                                       std::string(gDinoResource->GetText(data_0208a770))));
    mParallel.AddTail(mHelps[0] = new DinoHelpDummyItem(GetDisplay(), MRect(0x26, 3, 0x9C, 0x10),
                                                        std::string(gDinoResource->GetText(data_0208a748))));
    mParallel.AddTail(mHelps[5] = new DinoHelpDummyItem(GetDisplay(), MRect(0xEF, 0x8B, 0xFD, 0x9D),
                                                        std::string(gDinoResource->GetText(data_0208a734))));
    AddEntry((u32)mHelps[0], 1, 0);
    AddEntry((u32)mHelps[0], 3, 0);
    mUnkB8 = new DinoUiOrganizationBackButton(this);
    AddItem(mUnkB8);
    mUnkBC = new DinoUiOrganizationOKButton(this);
    AddItem(mUnkBC);
    for (int i = 0; i < 4; i++) {
        mTabs[i] = new DinoUiPartsTabIconMenuItem(data_ov007_0218ee50[i], data_020b58e0[i]);
        AddItem(mTabs[i]);
    }
    for (int i = 0; i < 5; i++) {
        mButtons[i] = new DinoUiSortMenuItem(data_ov007_0218ee60[i],
                                             MPoint(data_ov007_0218ebec.sortPos.x + data_ov007_0218ec6c.sortStep.x * i,
                                                    data_ov007_0218ebec.sortPos.y + data_ov007_0218ec6c.sortStep.y * i));
        AddItem(mButtons[i]);
    }
    for (int i = 1; i < 5; i++) {
        AddLink(mButtons[i], 3, mButtons[i - 1]);
        AddLink(mButtons[i - 1], 1, mButtons[i]);
    }
    AddLink(mButtons[0], 3, mButtons[4]);
    AddLink(mButtons[4], 1, mButtons[0]);
    SetHelpMode(0);
    MFont *font;
    if (mTeamIndex == gDinoBackup->GetUnk34_26()) {
        font = gDinoResource->CreateFont12(0, 10, 11);
    } else {
        font = gDinoResource->CreateFont12(0, 1, 11);
    }
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, MPoint(0x80, 0x20), gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    MIndicator *teamName = new MIndicator(canvas, font, 0);
    mParallel.AddTail(teamName);
    teamName->SetPosition(data_ov007_0218ec4c);
    teamName->SetPriority(2);
    teamName->Show();
    teamName->SetText(std::wstring((const wchar_t *)mTeam.name));
    mCursorTasks = new MParallelTaskSet();
    mParallel.AddTail(mCursorTasks);
    mCursor = new DinoUiFieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(0);
    mCursor->mUnk10->GetUnit()->SetMode(1);
    mCursor->mUnk14->GetUnit()->SetMode(1);
    mCursor->Hide();
    MFontSet *numbers = new MFontSet(std::string(data_ov007_02190f08), 0, 0, 0);
    for (int i = 0; i < 15; i++) {
        MTaskSet *tasks = OrganizationParallel(this);
        MObjCanvas *slotCanvas = new MObjCanvas(gCurrentDisplay, MPoint(0x10, 0x10), numbers->GetPalette(), TRUE);
        slotCanvas->GetOamUnit()->SetMode(1);
        mNumbers[i] = new MIndicator(slotCanvas, numbers, 0);
        tasks->AddTail(mNumbers[i]);
        int row = i / 5;
        int column = i % 5;
        mNumbers[i]->SetPosition(MPoint(data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * column +
                                            data_ov007_0218ebec.slotRowX * row,
                                        data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * column +
                                            data_ov007_0218ebec.slotRowY * row) +
                                 data_ov007_0218ebec.numberOffset);
        mNumbers[i]->SetPriority(2);
        mNumbers[i]->Show();
    }
    mParallel.AddTail(mOam);
    mName = new DinoUiPartsReviverName();
    mParallel.AddTail(mName);
    mName->Hide();
    mUnk230 = new MSerialTaskSet();
    mParallel.AddTail(mUnk230);
    mInfo = new DinoUiRvInformation(1);
    mParallel.AddTail(mInfo);
    mParallel.AddTail(new DinoUiPartsBlackBelt());
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a948))));
    mTeamTech = new DinoUiPartsTeamTech(data_ov007_0218ee74);
    mParallel.AddTail(mTeamTech);
    mTeamTech->Hide();
    SetTab(0);
    CreateList();
    LinkList();
    CreateSlots();
    UpdateDetails(0xFF);
    UpdateTeamTech();
    AddLink(mButtons[3], 2, mUnkBC);
    AddLink(mButtons[4], 2, mUnkB8);
    AddLink(mUnkBC, 4, mButtons[3]);
    AddLink(mUnkB8, 4, mButtons[4]);
    for (int i = 0; i < 4; i++) {
        AddLink(mTabs[i], 4, mButtons[0]);
    }
    AddLink(mTabs[3], 1, mUnkBC);
    AddLink(mUnkBC, 3, mTabs[3]);
    AddLink(mUnkBC, 1, mUnkB8);
    AddLink(mUnkB8, 3, mUnkBC);
    AddLink(mTabs[0], 3, mUnkB8);
    AddLink(mUnkB8, 1, mTabs[0]);
    DinoDimScreen();
    MTask *main;
    MTask *sub;
    sub = gLcd->mSub->mBright->CreateFadeInFromBlack(30, 0);
    main = gLcd->mMain->mBright->CreateFadeInFromBlack(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(main, sub);
    mSerial.AddTail(fades);
    u32 i;
    u32 count = 0;
    for (i = 0; i < 5; i++) {
        if (mTeam.slots[i]) {
            count++;
        }
    }
    if (count == 0) {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60),
                                                       std::string(gDinoResource->GetText(data_0208aa74)), 4));
        mUnk23D = 1;
        FocusItem(mSlots[0]);
    } else {
        FocusItem(mListItems[0]);
    }
    mCursor->Hide();
    MTask *play;
    MTask *show;
    show = mCursor->CreateShowTransfer();
    play = mCursor->CreatePlayTask();
    MParallelTaskSet *cursorSet = new MParallelTaskSet(TRUE);
    cursorSet->AddTail(play);
    cursorSet->AddTail(show);
    mSerial.AddTail(cursorSet);
}
#pragma opt_common_subs reset

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
        SetHelpMode(1);
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
        SetHelpMode(1);
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
        SetHelpMode(2);
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
    SetHelpMode(0);
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
    SetHelpMode(0);
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

extern const u32 data_0208a680, data_0208a684, data_0208a688, data_0208a68c, data_0208a690, data_0208a70c, data_0208a738, data_0208a73c, data_0208a740, data_0208a744;

// Swaps the help areas for the mode: the list (0), a member menu (1) or a slot menu (2).
void DinoUiOrganizationMenu::SetHelpMode(int mode) {
    RemoveEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[3], 2);
    RemoveEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0], 1);
    RemoveEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mUnkB8, 3);
    mUnk250->Clear();
    switch (mode) {
    case 0:
        mUnk250->AddTail(mHelps[1] = new DinoHelpDummyItem(GetDisplay(), MRect(0x1C, 0x16, 0x47, 0x3A), std::string(gDinoResource->GetText(data_0208a744))));
        mUnk250->AddTail(new DinoHelpDummyItem(GetDisplay(), MRect(0x4A, 0x16, 0x9B, 0x3A), std::string(gDinoResource->GetText(data_0208a740))));
        mUnk250->AddTail(mHelps[3] = new DinoHelpDummyItem(GetDisplay(), MRect(0xA4, 0x16, 0xEE, 0x3A), std::string(gDinoResource->GetText(data_0208a73c))));
        mUnk250->AddTail(mHelps[4] = new DinoHelpDummyItem(GetDisplay(), MRect(0xF, 0x41, 0xEE, 0x9E), std::string(gDinoResource->GetText(data_0208a738))));
        AddEntry((u32)mHelps[1], 2, (u32)mHelps[4]);
        AddEntry((u32)mHelps[3], 4, (u32)mHelps[0]);
        AddEntry((u32)mHelps[3], 2, (u32)mHelps[4]);
        AddEntry((u32)mHelps[5], 3, (u32)mHelps[4]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0], 4, (u32)mHelps[4]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[4], 4, (u32)mHelps[4]);
        ((DinoUiOrganizationMenuItem *)mUnkB8)->mTextId = data_0208a70c;
        mUnkBC->DinoHelpItem::Enable();
        break;
    case 1:
        mUnk250->AddTail(mHelps[1] = new DinoHelpDummyItem(GetDisplay(), MRect(0x1B, 0x15, 0xEF, 0x3C), std::string(gDinoResource->GetText(data_0208a690))));
        mUnk250->AddTail(mHelps[2] = new DinoHelpDummyItem(GetDisplay(), MRect(0xE, 0x42, 0xEE, 0x9F), std::string(gDinoResource->GetText(data_0208a68c))));
        AddEntry((u32)mHelps[1], 4, (u32)mHelps[0]);
        AddEntry((u32)mHelps[5], 3, (u32)mHelps[2]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0], 4, (u32)mHelps[2]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[4], 4, (u32)mHelps[2]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[3], 2, (u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mUnkB8);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0], 1, (u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mUnkB8);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mUnkB8, 3, (u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0]);
        ((DinoUiOrganizationMenuItem *)mUnkB8)->mTextId = data_0208a688;
        mUnkBC->DinoHelpItem::Disable();
        break;
    case 2:
        mUnk250->AddTail(mHelps[1] = new DinoHelpDummyItem(GetDisplay(), MRect(0x1B, 0x15, 0xEF, 0x3C), std::string(gDinoResource->GetText(data_0208a684))));
        mUnk250->AddTail(mHelps[2] = new DinoHelpDummyItem(GetDisplay(), MRect(0xE, 0x42, 0xEE, 0x9F), std::string(gDinoResource->GetText(data_0208a680))));
        AddEntry((u32)mHelps[1], 4, (u32)mHelps[0]);
        AddEntry((u32)mHelps[5], 3, (u32)mHelps[2]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0], 4, (u32)mHelps[2]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[4], 4, (u32)mHelps[2]);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[3], 2, (u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mUnkB8);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0], 1, (u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mUnkB8);
        AddEntry((u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mUnkB8, 3, (u32)(DinoHelpItem *)(DinoUiOrganizationMenuItem *)mButtons[0]);
        ((DinoUiOrganizationMenuItem *)mUnkB8)->mTextId = data_0208a688;
        mUnkBC->DinoHelpItem::Disable();
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
            mSerial.AddTail(CreateRevertTasks(&saved));
        }
    }
    Leave();
}

// Where a list slot is on the page.
static inline MPoint DinoOrgSlotPos(int slot) {
    int row = slot / 5;
    int column = slot % 5;
    MPoint pos;
    pos.x = data_ov007_0218ebec.slotX + data_ov007_0218ebec.slotColumnX * column + data_ov007_0218ebec.slotRowX * row;
    pos.y = data_ov007_0218ebec.slotY + data_ov007_0218ebec.slotColumnY * column + data_ov007_0218ebec.slotRowY * row;
    return pos;
}

// Backing out of changes: each changed member flies back to the list (or to the slot it came from), and each saved
// member flies back into its slot (a medal made for it if its page isn't showing).
MTask *DinoUiOrganizationMenu::CreateRevertTasks(DinoTeam *saved) {
    MParallelTaskSet *moves = new MParallelTaskSet(TRUE);
    for (int i = 0; i < 5; i++) {
        if (mTeam.slots[i] == saved->slots[i]) {
            continue;
        }
        if (mTeam.slots[i] != 0) {
            int j = 5;
            for (int k = 0; k < 5; k++) {
                if (mTeam.slots[i] == saved->slots[k]) {
                    j = k;
                }
            }
            if (j != 5) {
                moves->AddTail(mListItems[i]->CreateFlyInTask(data_ov007_0218ef0c[j]));
            } else {
                int index = std::find(mCreatures.begin(), mCreatures.end(), mTeam.slots[i]) - mCreatures.begin();
                if (index / 15 < mPage) {
                    moves->AddTail(mListItems[i]->CreateFlyInTask(data_ov007_0218ecbc));
                } else if (index / 15 > mPage) {
                    moves->AddTail(mListItems[i]->CreateFlyInTask(data_ov007_0218ecc4));
                } else {
                    moves->AddTail(mListItems[i]->CreateFlyInTask(DinoOrgSlotPos(index % 15)));
                }
            }
        }
        u8 old = saved->slots[i];
        if (old == 0) {
            continue;
        }
        int k, j;
        j = 5;
        for (k = 0; k < 5; k++) {
            if (old == mTeam.slots[k]) {
                j = k;
            }
        }
        if (j != 5) {
            continue;
        }
        int index = std::find(mCreatures.begin(), mCreatures.end(), saved->slots[i]) - mCreatures.begin();
        if (index / 15 == mPage) {
            moves->AddTail(mSlots[index % 15]->CreateFlyInTask(data_ov007_0218ef0c[i]));
            continue;
        }
        DinoCreatureState creature;
        gDinoBackup->GetCreature(old, &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        DinoUiPartsReviverMedalButton *button;
        if (index / 15 < mPage) {
            button = new DinoUiPartsReviverMedalButton(mOam, data_ov007_0218ecbc, saved->slots[i], maxed, 0);
        } else {
            button = new DinoUiPartsReviverMedalButton(mOam, data_ov007_0218ecc4, saved->slots[i], maxed, 0);
        }
        AddItem(button);
        moves->AddTail(button->CreateFlyInTask(data_ov007_0218ef0c[i]));
    }
    return moves;
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

// The file's static initialization (see dino_static_init.h), as in the medal menu's; then the tab backgrounds.
// NONMATCHING: 99.6%, the game computes -0x28 (from 0xB7) after the last point's second store.
extern "C" u32 data_ov007_0218eaec[];
extern "C" u32 data_ov007_0218ecec[];
extern "C" u32 data_ov007_0218ee6c[];
extern "C" u32 data_ov007_0218ef14[];
extern "C" u32 data_ov007_0218ef1c[];
extern "C" u32 data_ov007_0218ef24[];
extern "C" u32 data_ov007_0218ef2c[];
extern "C" u32 data_ov007_0218ef6c[];
extern "C" u32 data_ov007_0218efec[];
extern "C" u32 data_ov007_0218f06c[];
extern "C" u32 data_ov007_0218f0ec[];
extern "C" u32 data_ov007_0218f16c[];
extern "C" u32 _ZTI26DinoUiPartsTabIconMenuItem_ov007[];
static struct DinoUiOrganizationMenuStaticInit {
    DinoUiOrganizationMenuStaticInit() {
        DINO_POOL_WORD(data_ov007_0218ecec, 0x68) = 0x80;
        DINO_POOL_WORD(data_ov007_0218ecec, 0x6C) = 0xB0;
        DINO_POOL_WORD(data_ov007_0218ecec, 0x70) = 8;
        DINO_POOL_WORD(data_ov007_0218ecec, 0x74) = 0x96;
        DINO_POOL_WORD(data_ov007_0218ecec, 0x78) = 0x40;
        DINO_POOL_WORD(data_ov007_0218ecec, 0x7C) = 0x70;
        {
            s32 v = DINO_POOL_WORD(data_ov007_0218e9ec, 0x4C);
            DINO_POOL_WORD(data_ov007_0218ef6c, 0x70) = v;
            DINO_POOL_WORD(data_ov007_0218ef6c, 0x74) = v;
        }
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x78) = DINO_POOL_WORD(data_ov007_0218eaec, 0x34);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x7C) = DINO_POOL_WORD(data_ov007_0218eaec, 0x7C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x0) = DINO_POOL_WORD(data_ov007_0218ebec, 0x1C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x4) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x6C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x8) = DINO_POOL_WORD(data_ov007_0218eaec, 0x38);
        DINO_POOL_WORD(data_ov007_0218efec, 0xC) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x68);
        DINO_POOL_WORD(data_ov007_0218efec, 0x10) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x54);
        DINO_POOL_WORD(data_ov007_0218efec, 0x14) = DINO_POOL_WORD(data_ov007_0218ebec, 0x2C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x18) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x8);
        DINO_POOL_WORD(data_ov007_0218efec, 0x1C) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x24);
        DINO_POOL_WORD(data_ov007_0218efec, 0x20) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x18);
        DINO_POOL_WORD(data_ov007_0218efec, 0x24) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x5C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x28) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x10);
        DINO_POOL_WORD(data_ov007_0218efec, 0x2C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x8);
        DINO_POOL_WORD(data_ov007_0218efec, 0x30) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x6C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x34) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x14);
        DINO_POOL_WORD(data_ov007_0218efec, 0x38) = DINO_POOL_WORD(data_ov007_0218ebec, 0x30);
        DINO_POOL_WORD(data_ov007_0218efec, 0x3C) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x60);
        DINO_POOL_WORD(data_ov007_0218efec, 0x40) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x7C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x44) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x30);
        DINO_POOL_WORD(data_ov007_0218efec, 0x48) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x78);
        DINO_POOL_WORD(data_ov007_0218efec, 0x4C) = DINO_POOL_WORD(data_ov007_0218eaec, 0x14);
        DINO_POOL_WORD(data_ov007_0218efec, 0x50) = DINO_POOL_WORD(data_ov007_0218eb6c, 0xC);
        DINO_POOL_WORD(data_ov007_0218efec, 0x54) = DINO_POOL_WORD(data_ov007_0218ebec, 0x20);
        DINO_POOL_WORD(data_ov007_0218efec, 0x58) = DINO_POOL_WORD(data_ov007_0218ebec, 0x10);
        DINO_POOL_WORD(data_ov007_0218efec, 0x5C) = DINO_POOL_WORD(data_ov007_0218ebec, 0x34);
        DINO_POOL_WORD(data_ov007_0218efec, 0x60) = DINO_POOL_WORD(data_ov007_0218ebec, 0x28);
        DINO_POOL_WORD(data_ov007_0218efec, 0x64) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x2C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x68) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x10);
        DINO_POOL_WORD(data_ov007_0218efec, 0x6C) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x64);
        DINO_POOL_WORD(data_ov007_0218efec, 0x70) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x28);
        DINO_POOL_WORD(data_ov007_0218efec, 0x74) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x60);
        DINO_POOL_WORD(data_ov007_0218efec, 0x78) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x5C);
        DINO_POOL_WORD(data_ov007_0218efec, 0x7C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x60);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x0) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x54);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x4) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x58);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x8) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x50);
        DINO_POOL_WORD(data_ov007_0218f06c, 0xC) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x40);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x10) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x48);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x14) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x1C);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x18) = DINO_POOL_WORD(data_ov007_0218eaec, 0x40);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x1C) = DINO_POOL_WORD(data_ov007_0218eaec, 0x28);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x20) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x40);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x24) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x34);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x28) = DINO_POOL_WORD(data_ov007_0218eaec, 0x50);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x2C) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x30);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x30) = DINO_POOL_WORD(data_ov007_0218eaec, 0x5C);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x34) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x74);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x38) = DINO_POOL_WORD(data_ov007_0218eaec, 0x64);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x3C) = DINO_POOL_WORD(data_ov007_0218eaec, 0x54);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x40) = DINO_POOL_WORD(data_ov007_0218ebec, 0x0);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x44) = DINO_POOL_WORD(data_ov007_0218eaec, 0x6C);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x48) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x34);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x4C) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x2C);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x50) = DINO_POOL_WORD(data_ov007_0218eaec, 0x70);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x54) = DINO_POOL_WORD(data_ov007_0218eaec, 0x78);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x58) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x3C);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x5C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x44);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x60) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x40);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x64) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x0);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x68) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x1C);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x6C) = DINO_POOL_WORD(data_ov007_0218eaec, 0x2C);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x70) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x18);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x74) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x38);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x78) = DINO_POOL_WORD(data_ov007_0218eaec, 0x24);
        DINO_POOL_WORD(data_ov007_0218f06c, 0x7C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x64);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x0) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x4C);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x4) = DINO_POOL_WORD(data_ov007_0218eaec, 0x60);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x8) = DINO_POOL_WORD(data_ov007_0218eaec, 0x4C);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0xC) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x64);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x10) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x50);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x14) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x0);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x18) = DINO_POOL_WORD(data_ov007_0218eaec, 0x68);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x1C) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x10);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x20) = DINO_POOL_WORD(data_ov007_0218eaec, 0xC);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x24) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x18);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x28) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x5C);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x2C) = DINO_POOL_WORD(data_ov007_0218eaec, 0x74);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x30) = DINO_POOL_WORD(data_ov007_0218eaec, 0x3C);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x34) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x48);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x38) = DINO_POOL_WORD(data_ov007_0218eaec, 0x10);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x3C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x24);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x40) = DINO_POOL_WORD(data_ov007_0218ebec, 0x8);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x44) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x78);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x48) = DINO_POOL_WORD(data_ov007_0218eaec, 0x20);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x4C) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x70);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x50) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x28);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x54) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x7C);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x58) = DINO_POOL_WORD(data_ov007_0218eaec, 0x48);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x5C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x68);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x60) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x78);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x64) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x70);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x68) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x7C);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x6C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x4);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x70) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x1C);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x74) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x20);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x78) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x70);
        DINO_POOL_WORD(data_ov007_0218f0ec, 0x7C) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x50);
        DINO_POOL_WORD(data_ov007_0218f16c, 0x0) = DINO_POOL_WORD(data_ov007_0218ebec, 0xC);
        DINO_POOL_WORD(data_ov007_0218f16c, 0x4) = DINO_POOL_WORD(data_ov007_0218eaec, 0x4);
        DINO_POOL_WORD(data_ov007_0218f16c, 0x8) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x68);
        DINO_POOL_WORD(data_ov007_0218f16c, 0xC) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x30);
        DINO_POOL_WORD(data_ov007_0218f16c, 0x10) = DINO_POOL_WORD(data_ov007_0218e9ec, 0xC);
        DINO_POOL_WORD(data_ov007_0218f16c, 0x14) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x8);
        DINO_POOL_WORD(data_ov007_0218f16c, 0x18) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x14);
        {
            s32 v = DINO_POOL_WORD(data_ov007_0218e9ec, 0x44);
            DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x78) = v;
            DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x7C) = v;
        }
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x0) = DINO_POOL_WORD(data_ov007_0218ebec, 0x18);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x4) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x3C);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x8) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x14);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0xC) = DINO_POOL_WORD(data_ov007_0218ebec, 0x4);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x10) = DINO_POOL_WORD(data_ov007_0218ebec, 0x14);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x14) = DINO_POOL_WORD(data_ov007_0218ebec, 0x24);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x18) = DINO_POOL_WORD(data_ov007_0218eaec, 0x0);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x1C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x58);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x20) = DINO_POOL_WORD(data_ov007_0218eaec, 0x8);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x24) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x3C);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x28) = DINO_POOL_WORD(data_ov007_0218eaec, 0x18);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x2C) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x24);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x30) = DINO_POOL_WORD(data_ov007_0218eaec, 0x30);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x34) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x20);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x38) = DINO_POOL_WORD(data_ov007_0218eaec, 0x44);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x3C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0xC);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x40) = DINO_POOL_WORD(data_ov007_0218e9ec, 0x48);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x44) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x28);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x48) = DINO_POOL_WORD(data_ov007_0218eaec, 0x58);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x4C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x38);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x50) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x4C);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x54) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x74);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x58) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x4);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x5C) = DINO_POOL_WORD(data_ov007_0218eaec, 0x1C);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x60) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x74);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x64) = DINO_POOL_WORD(data_ov007_0218eb6c, 0x20);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x68) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x6C);
        DINO_POOL_WORD(data_ov007_0218ef6c, 0x6C) = DINO_POOL_WORD(data_ov007_0218ea6c, 0x2C);
        DINO_POOL_WORD(data_ov007_0218ebec, 0x40) = 0x2B;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x44) = 0x50;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x48) = 0x2D;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x4C) = 0;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x50) = 0;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x54) = 0x20;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x58) = -0x1B;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x5C) = -0xE;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x60) = 0x28;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x64) = 4;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x68) = 0x13;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x6C) = 0xB3;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x70) = 0x1B;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x74) = 0;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x78) = 0x13;
        DINO_POOL_WORD(data_ov007_0218ebec, 0x7C) = 0x9E;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x0) = 0x2D;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x4) = 0;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x8) = 0x80;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0xC) = 0x60;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x10) = 0x79;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x14) = 0x74;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x18) = 0x87;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x1C) = 0x74;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x20) = 0xF7;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x24) = 0x97;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x28) = 0;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x2C) = 0xB0;
        DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x20) = 0x33;
        DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x24) = 0x29;
        DINO_POOL_WORD(data_ov007_0218ef14, 0x0) = 0x60;
        DINO_POOL_WORD(data_ov007_0218ef14, 0x4) = 0x29;
        volatile s32 *p = (volatile s32 *) &data_ov007_0218ef1c;
        p[0x0 / 4] = 0x85;
        p[0x4 / 4] = 0x29;
        p = (volatile s32 *) &data_ov007_0218ef24;
        p[0x0 / 4] = 0xB7;
        p[0x4 / 4] = 0x29;
        p = (volatile s32 *) &data_ov007_0218ef2c;
        p[0x0 / 4] = 0xDB;
        p[0x4 / 4] = 0x29;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x30) = -0x28;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x34) = 0x1C;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x38) = -0x20;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x3C) = 8;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x40) = -6;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x44) = 0xC;
        DINO_POOL_WORD(data_ov007_0218ee6c, 0x8) = DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x20);
        DINO_POOL_WORD(data_ov007_0218ee6c, 0xC) = DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x24);
        DINO_POOL_WORD(data_ov007_0218ee6c, 0x10) = DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x28);
        DINO_POOL_WORD(data_ov007_0218ee6c, 0x14) = DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x2C);
        DINO_POOL_WORD(data_ov007_0218ee6c, 0x18) = DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x30);
        DINO_POOL_WORD(data_ov007_0218ee6c, 0x1C) = DINO_POOL_WORD(_ZTI26DinoUiPartsTabIconMenuItem_ov007, 0x34);
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x48) = 0x120;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x4C) = 0x29;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x50) = -0x20;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x54) = 0x29;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x58) = 0x120;
        DINO_POOL_WORD(data_ov007_0218ec6c, 0x5C) = 0x29;
    }
} sDinoUiOrganizationMenuStaticInit;

std::string data_ov007_0218ef34[4] = {"image/bg_hensei_0", "image/bg_hensei_1", "image/bg_hensei_2", "image/bg_hensei_3"};
