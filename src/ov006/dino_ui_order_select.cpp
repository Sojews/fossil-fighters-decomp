// DinoUiOrderSelect.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/blend.h"
#include "game/dino_sound.h"
#include "game/dino_ui_order_select.h"
#include "game/dino_ui_rv_information.h"
#include "types.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/obj.h"
#include "m/bg.h"
#include "game/dino_ui_parts_reviver_medal_button.h"
#include <algorithm>
#include "m/smart_oam.h"
#include "game/dino_team.h"
#include "game/dino_ui_parts_team_tech.h"
#include "m/call_task.h"
#include "m/input.h"

DinoUiOrderSelect::~DinoUiOrderSelect() {
}

DinoUiOrderSelect::DinoUiOrderSelect(u32 team, const DinoUiOrderSelectInfo &info, u8 unk110, u8 unk111, u32 unk114,
                                     u8 unk112)
    : MProcess(NULL), mTeam(team), mInfo(info), mUnk110(unk110), mUnk111(unk111), mUnk112(unk112), mUnk114(unk114) {
}

// Sets up both screens and opens the menu.
void DinoUiOrderSelect::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 0, 3, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
    GetSerial()->AddTail(new DinoUiOrderSelectMenu(mTeam, &mInfo, mUnk110, mUnk111, mUnk114, mUnk112),
                         MTaskExitTransfer::Create(this));
}

DinoUiOrderSelectMenuItem::~DinoUiOrderSelectMenuItem() {
}

DinoUiOrderSelectBackButton::~DinoUiOrderSelectBackButton() {
}

DinoUiOrderSelectDecideButton::~DinoUiOrderSelectDecideButton() {
}

MRect DinoUiOrderSelectMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOrderSelectMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOrderSelectMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiOrderSelectBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrderSelectMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

const char *DinoUiOrderSelectMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

extern "C" void func_ov006_0218d828(void *owner);

void DinoUiOrderSelectDecideButton::OnDecide() {
    mMotion->Play(3, 1);
    func_ov006_0218d828(mOwner);
    DinoSoundPlaySe(0x98, 0);
}

void DinoUiOrderSelectBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    func_ov006_0218d828(mOwner);
    DinoSoundPlaySe(0x90, 0);
}

// DinoUiOrderSelectMenu's OnStart/OnFocus/OnDecide (not decompiled yet) queue these call tasks.
#pragma auto_inline off
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoUiOrderSelectMenu::*func)());
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoUiOrderSelectMenu::*func)(bool), bool a1);
#pragma auto_inline reset

#pragma auto_inline off

DinoUiOrderSelectMenuItem::DinoUiOrderSelectMenuItem(DinoUiOrderSelectMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

DinoUiOrderSelectMenu::~DinoUiOrderSelectMenu() {
    if (mPictures[0] != NULL) {
        for (int i = 0; i < 4; i++) {
            delete mPictures[i];
        }
    }
    gCurrentDisplay->mBlend->Reset();
}

void DinoUiOrderSelectMenu::OnIdle() {
    mUnk1F6 = TRUE;
    if (GetInput()->GetPressed() & 0x800) {
        PageLeft();
    } else if (GetInput()->GetPressed() & 0x400) {
        PageRight();
    }
}

void DinoUiOrderSelectMenu::PageLeft() {
    u8 count = mHasFourthTab ? 4 : 3;
    SetPage((mPage + count - 1) % count);
    if (mFocusTabs) {
        FocusItem(mTabs[mPage]);
    }
}

void DinoUiOrderSelectMenu::PageRight() {
    u8 count = mHasFourthTab ? 4 : 3;
    SetPage((mPage + 1) % count);
    if (mFocusTabs) {
        FocusItem(mTabs[mPage]);
    }
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov006_02197110;

void DinoUiOrderSelectMenu::SetInfoPage(u8 page) {
    mInfo->SetPage(page);
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov006_02197110.Enable_00));
    mUnk1BC->Clear();
    Disable();
}

void DinoUiOrderSelectMenu::ClearItems() {
    RemoveLink(mOKButton, 4);
    RemoveLink(mBackButton, 4);
    RemoveLink(mUnkF8[mHasFourthTab ? 3 : 2], 2);
    for (int i = 0; i < 5; i++) {
        RemoveItem(mItems[i]);
        delete mItems[i];
    }
    mItemsLinked = FALSE;
}

// The OK button lights up when the team can fight; the technique shows when the team has one.
void DinoUiOrderSelectMenu::RefreshOKButton() {
    if (!IsTeamInvalid()) {
        mOKButton->mMotion->GetUnit()->SetMode(1);
    } else {
        mOKButton->mMotion->GetUnit()->SetMode(0);
    }
    if (DinoTeamHasTech(&mTeam)) {
        mTech->Show();
    } else {
        mTech->Hide();
    }
}

void DinoUiOrderSelectMenu::RefreshItems() {
    for (int i = 0; i < 5; i++) {
        mItems[i]->Enable();
    }
    for (int j = 0; j < 5; j++) {
        if (mOpponent.tabs[j].present) {
            mUnkF8[j]->Enable();
        }
    }
    mBackButton->MMenuItem::Disable();
    mBackButton->DinoHelpItem::Disable();
    mBackButton->mMotion->Hide();
    mOKButton->MMenuItem::Enable();
    mOKButton->DinoHelpItem::Enable();
    mOKButton->mMotion->Show();
    RemoveLink(mTabs[0], 3);
    AddLink(mTabs[0], 3, mOKButton);
    if (mHasFourthTab) {
        RemoveLink(mTabs[3], 1);
        AddLink(mTabs[3], 1, mOKButton);
    } else {
        RemoveLink(mTabs[2], 1);
        AddLink(mTabs[2], 1, mOKButton);
    }
    LinkItems();
}

DinoUiOrderSelectMenu::DinoUiOrderSelectMenu(u32 team, const DinoUiOrderSelectInfo *opponent, u8 hasFourthTab,
                                             u8 unk1F7, u32 unk1F8, u8 unk1FD)
    : MMenu(NULL, 0), mUnkD0(NULL), mTeamIndex(team), mOpponent(*opponent), mHasFourthTab(hasFourthTab),
      mFocusIndex(0), mUnk1F0(0), mPage(0xFF), mFocusTabs(0), mUnk1F6(0), mUnk1F7(unk1F7), mUnk1F8(unk1F8),
      mItemsLinked(0), mUnk1FD(unk1FD), mUnk204(0xFF), mUnk205(0) {
    for (int i = 0; i < 4; i++) {
        mPictures[i] = NULL;
    }
}

// Where the team's medals go.
extern "C" const MPoint data_ov006_02197340[5];
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

void DinoUiOrderSelectMenu::CreateItems() {
    for (int i = 0; i < 5; i++) {
        if (mTeam.slots[i]) {
            DinoCreatureState creature;
            gDinoBackup->GetCreature(mTeam.slots[i], &creature);
            u8 maxed;
            if (GetCreatureLevel(creature) == 12) {
                maxed = TRUE;
            } else {
                maxed = FALSE;
            }
            mItems[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_02197340[i], mTeam.slots[i], maxed, 0);
            AddItem(mItems[i]);
        } else {
            mItems[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_02197340[i], 0, 0, 0);
            AddItem(mItems[i]);
            mItems[i]->Disable();
        }
        AddLink(mItems[i], 2, mTabs[mPage]);
    }
    if (mItems[3]->IsEnabled()) {
        AddLink(mUnkF8[mHasFourthTab ? 3 : 2], 2, mItems[3]);
    } else if (mItems[2]->IsEnabled()) {
        AddLink(mUnkF8[mHasFourthTab ? 3 : 2], 2, mItems[2]);
    } else {
        AddLink(mUnkF8[mHasFourthTab ? 3 : 2], 2, mItems[4]);
    }
}

// Links the items left and right to the nearest enabled ones (round the row), and the buttons up to the last.
void DinoUiOrderSelectMenu::LinkItems() {
    MMenuItem *left[5];
    MMenuItem *right[5];
    int i;
    for (i = 0; i < 5; i++) {
        left[i] = mItems[(i + 4) % 5];
        right[i] = mItems[(i + 1) % 5];
    }
    int first = 0;
    for (i = 0; i < 5; i++) {
        if (mItems[i]->IsEnabled()) {
            first = i;
            break;
        }
    }
    for (i = 0; i < 5; i++) {
        int k = first + i;
        if (!mItems[k % 5]->IsEnabled()) {
            left[(k + 1) % 5] = left[k % 5];
        }
    }
    int start = (first + 1) % 5;
    for (i = 4; i >= 0; i--) {
        int k = start + i;
        if (!mItems[k % 5]->IsEnabled()) {
            right[(k + 4) % 5] = right[k % 5];
        }
    }
    if (mItemsLinked) {
        RemoveLink(mBackButton, 4);
        RemoveLink(mOKButton, 4);
        for (i = 0; i < 5; i++) {
            RemoveLink(mItems[i], 3);
            RemoveLink(mItems[i], 1);
        }
    }
    if (mItems[4]->IsEnabled()) {
        AddLink(mBackButton, 4, mItems[4]);
        AddLink(mOKButton, 4, mItems[4]);
    } else if (mItems[3]->IsEnabled()) {
        AddLink(mBackButton, 4, mItems[3]);
        AddLink(mOKButton, 4, mItems[3]);
    } else {
        AddLink(mBackButton, 4, mItems[2]);
        AddLink(mOKButton, 4, mItems[2]);
    }
    for (i = 0; i < 5; i++) {
        AddLink(mItems[i], 3, left[i]);
        AddLink(mItems[i], 1, right[i]);
    }
    mItemsLinked = TRUE;
}

void DinoUiOrderSelectMenu::SelectItem(int index) {
    int i;
    for (i = 0; i < 5; i++) {
        mItems[i]->Enable();
    }
    mItems[index]->Disable();
    for (i = 0; i < 5; i++) {
        mUnkF8[i]->Disable();
    }
    mBackButton->MMenuItem::Enable();
    mBackButton->DinoHelpItem::Enable();
    mBackButton->mMotion->Show();
    mOKButton->MMenuItem::Disable();
    mOKButton->DinoHelpItem::Disable();
    mOKButton->mMotion->Hide();
    RemoveLink(mTabs[0], 3);
    AddLink(mTabs[0], 3, mBackButton);
    if (mHasFourthTab) {
        RemoveLink(mTabs[3], 1);
        AddLink(mTabs[3], 1, mBackButton);
    } else {
        RemoveLink(mTabs[2], 1);
        AddLink(mTabs[2], 1, mBackButton);
    }
    LinkItems();
}

void DinoUiOrderSelectMenu::FinishPage() {
    mUnk1F0 = 0;
    ClearItems();
    CreateItems();
    RefreshItems();
    Enable();
    RefreshOKButton();
    FocusNearestItem(GetCursor());
}

void DinoUiOrderSelectMenu::FinishTab() {
    RefreshItems();
    FocusItem(mItems[mFocusIndex]);
    RefreshOKButton();
}
