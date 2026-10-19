// DinoUiOrderSelect.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/blend.h"
#include "game/dino_sound.h"
#include "game/dino_ui_order_select.h"
#include "game/dino_ui_rv_information.h"
#include "types.h"
#include "m/smart_oam.h"
#include "game/dino_team.h"
#include "game/dino_ui_parts_team_tech.h"
#include "m/call_task.h"
#include "m/input.h"

DinoUiOrderSelect::~DinoUiOrderSelect() {
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
        if (mTabData[j].present) {
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
