#pragma ipa file
// DinoUiMedalMenu.
// Only the destructor is decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_medal_menu.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "m/display.h"
#include "m/blend.h"
#include "game/dino_sound.h"
#include "m/call_task.h"
#include "m/bg_new_task.h"
#include "m/picture.h"

DinoUiMedalMenu::~DinoUiMedalMenu() {
    gCurrentDisplay->mBlend->Reset();
}

DinoUiSortMenuItem::~DinoUiSortMenuItem() {
}

DinoUiMedalList::~DinoUiMedalList() {
}

DinoUiMedalMenuItem::~DinoUiMedalMenuItem() {
}

DinoUiMedalBackButton::~DinoUiMedalBackButton() {
}

DinoUiMedalPageLButton::~DinoUiMedalPageLButton() {
}

DinoUiMedalPageRButton::~DinoUiMedalPageRButton() {
}

DinoUiMedalSlotCreateTask::~DinoUiMedalSlotCreateTask() {
}

void DinoUiSortMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiSortMenuItem::OnDecide() {
}

void DinoUiSortMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiMedalMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMedalMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiSortMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMedalMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMedalBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiMedalMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiSortMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMedalList::Idle() {
    (this->*mState)();
}

void DinoUiMedalBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided(FALSE);
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMedalPageRButton::OnDecide() {
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

void DinoUiMedalPageLButton::OnDecide() {
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

void DinoUiMedalMenu::OnIdle() {
    mUnk204 = TRUE;
}

// NONMATCHING: the game loads mPos.x once for both corners
MRect DinoUiSortMenuItem::GetRect() {
    return MRect(mPos.Offset(0, -2), mPos.Offset(0x28, 0x10));
}

extern "C" const int data_02089a0c;

void DinoUiMedalSlotCreateTask::Idle() {
    if (mIndex <= 14) {
        mMenu->CreateSlot(mIndex);
        if (mIndex < mMenu->mSlotCount - 1) {
            mSerial.AddTail(new MTimer(data_02089a0c));
        }
    } else {
        Exit();
    }
    mIndex++;
}


#pragma auto_inline off

// The call-task makers, instantiated ahead of their users so none directly follows a function that calls it
// (DinoUiMedalMenu::OnFocus, not decompiled yet, uses the DinoHelpBase one).
template MTask *MCreateCallTask(DinoUiMedalMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiMedalMenu *obj, void (DinoUiMedalMenu::*func)());
template MTask *MCreateBgCallTask(DinoUiMedalMenu *obj, void (DinoUiMedalMenu::*func)(), int thread);

void DinoUiMedalMenu::PageRight() {
    if (mPage == mLastPage) {
        mPage = 0;
    } else {
        mPage++;
    }
    mSerial.AddTail(MCreateBgCallTask(this, &DinoUiMedalMenu::LoadPage, 1));
    mSerial.AddTail(MCreateCallTask(this, &DinoUiMedalMenu::FinishPageRight));
    Disable();
}

void DinoUiMedalMenu::PageLeft() {
    if (mPage == 0) {
        mPage = mLastPage;
    } else {
        mPage--;
    }
    mSerial.AddTail(MCreateBgCallTask(this, &DinoUiMedalMenu::LoadPage, 1));
    mSerial.AddTail(MCreateCallTask(this, &DinoUiMedalMenu::FinishPageLeft));
    Disable();
}

DinoUiMedalSlotCreateTask::DinoUiMedalSlotCreateTask(DinoUiMedalMenu *menu) : MProcess(NULL) {
    mMenu = menu;
    mIndex = 0;
}

BOOL DinoUiMedalMenu::HasUnk1EC(u8 id) {
    if (id == mUnk1EC[0] || id == mUnk1EC[1] || id == mUnk1EC[2] || id == mUnk1EC[3] || id == mUnk1EC[4]) {
        return TRUE;
    }
    return FALSE;
}

void DinoUiMedalMenu::ClearSlots() {
    for (int i = 0; i < 15; i++) {
        RemoveItem(mSlots[i]);
        delete mSlots[i];
    }
}

void DinoUiMedalMenu::SetSort(u8 sort) {
    if (sort < 5 && mSort != sort) {
        mSort = sort;
        DinoSoundPlaySe(0x98, 0);
        mSortCursor->SetPosition(0, sort * -0xC0);
        ClearSlots();
        Sort(sort);
        Disable();
        MTask *enable = MCreateCallTask(this, &DinoHelpBase::Enable);
        MTask *create = new DinoUiMedalSlotCreateTask(this);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(create);
        serial->AddTail(enable);
    }
}

void DinoUiMedalMenu::CreateSlots() {
    for (int i = 0; i < 15; i++) {
        CreateSlot(i);
    }
}

void DinoUiMedalMenu::FinishPageRight() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

void DinoUiMedalMenu::FinishPageLeft() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

#pragma auto_inline on


#pragma auto_inline off
template class std::__vec_deleter<u32, std::allocator<u32> >;
template void std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::sub_push_back(const MMotionFile &x, Metrowerks::int2type<false>);
template void std::__vec_deleter<u32, std::allocator<u32> >::assign<u32 *>(u32 *first, u32 *last, Metrowerks::int2type<true>);
