// DinoUiMedalMenu.
// Only the destructor is decompiled so far.

#pragma ipa file

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/bright.h"
#include "m/obj.h"
#include "m/lcd.h"
#include <algorithm>
#include "game/dino_ui_medal_menu.h"
#include "m/bg.h"
#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "game/dino_backup.h"
#include "game/dino_creature_sort.h"
#include "game/dino_ui_parts_reviver_medal_button.h"
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

MRect DinoUiSortMenuItem::GetFrame() {
    return MRect(mPos, MPoint(mPos.x + 0x28, mPos.y + 0xE));
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

MRect DinoUiSortMenuItem::GetRect() {
    int y = mPos.y;
    int x = mPos.x;
    return MRect(MPoint(x, y - 2), MPoint(x + 0x28, y + 0x10));
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

DinoUiMedalMenu::DinoUiMedalMenu(u32 *mode, u32 *index, u8 *unk)
    : MMenu(gLcd->mMain, NULL, 0), mUnkBC(0), mResult((int *)mode), mIndex(index), mUnk198(unk), mPage(0), mLastPage(0),
      mSort(*unk), mUnk204(0), mUnk21C(0xFF), mUnk21D(0) {
    mMotions.clear();
}

// Where the mUnk1EC creatures' medals go.
extern "C" const MPoint data_ov006_021942f4[5];
void DinoUiMedalMenu::CreateUnkF0Items() {
    for (int i = 0; i < 5; i++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(mUnk1EC[i], &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        mUnkF0[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_021942f4[i], mUnk1EC[i], maxed, 0);
        AddItem(mUnkF0[i]);
    }
}

void DinoUiMedalMenu::LinkItems() {
    if (!gDinoBackup->GetSystemFlag(2)) {
        AddLink(mUnkB8, 2, mUnkF0[0]);
        for (int i = 0; i < 5; i++) {
            AddLink(mUnkF0[i], 4, mUnkB8);
        }
        AddLink(mUnkF0[0], 3, mUnkF0[4]);
        AddLink(mUnkF0[4], 1, mUnkF0[0]);
    }
    AddLink(mSortButtons[0], 3, mSortButtons[4]);
    AddLink(mSortButtons[4], 1, mSortButtons[0]);
    for (int i = 0; i < 5; i++) {
        AddLink(mSortButtons[i], 2, mUnkB8);
    }
    AddLink(mUnkB8, 4, mSortButtons[4]);
    AddLink(mUnkB8, 3, NULL);
}

// Loads the motions of the page's slots.
void DinoUiMedalMenu::LoadPage() {
    int count;
    if (mPage == mLastPage) {
        count = mUnk19C.size() - mPage * 15;
    } else {
        count = 15;
    }
    mMotions.clear();
    for (int i = 0; i < count; i++) {
        char path[0x100];
        OS_SPrintf(path, "/motion/ui_reviver_medal/reviver_%03d.bin", mUnk19C[mPage * 15 + i]);
        mMotions.push_back(MMotionFile(std::string(path), 7));
    }
}

void DinoUiMedalMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 5; i++) {
        if (item == mSortButtons[i]) {
            SetSort(i);
        }
    }
    for (int i = 0; i < 5; i++) {
        if (item == mUnkF0[i]) {
            if (mUnk1EC[i] && mUnk204) {
                *mIndex = mPage * 15 + i;
                *mUnk198 = mSort;
                OnBackDecided(1);
                DinoSoundPlaySe(0x98, 0);
            }
            return;
        }
    }
    for (int i = 0; i < (int)mSlotCount; i++) {
        if (item == mSlots[i]) {
            if (mUnk204) {
                *mIndex = mPage * 15 + i;
                *mUnk198 = mSort;
                OnBackDecided(2);
                DinoSoundPlaySe(0x98, 0);
            }
            return;
        }
    }
}

DinoUiMedalMenuItem::DinoUiMedalMenuItem(DinoUiMedalMenu *owner, const std::string &path, MPoint pos, u32 textId) {
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

// Lists the creatures in the order: 0 and 1 are kept lists, the rest sort mUnk1A8.
void DinoUiMedalMenu::Sort(u8 sort) {
    if (sort > 4) {
        return;
    }
    switch (sort) {
    case 0:
        mUnk19C = mUnk1A8;
        break;
    case 1:
        mUnk19C = mUnk1B4;
        break;
    case 2:
        mUnk19C = mUnk1A8;
        {
            DinoCreatureByLevel order;
            std::stable_sort(mUnk19C.begin(), mUnk19C.end(), order);
        }
        break;
    case 3:
        mUnk19C = mUnk1A8;
        {
            DinoCreatureByUnk02031104 order;
            std::stable_sort(mUnk19C.begin(), mUnk19C.end(), order);
        }
        break;
    case 4:
        mUnk19C = mUnk1A8;
        {
            DinoCreatureByUnk0D order;
            std::stable_sort(mUnk19C.begin(), mUnk19C.end(), order);
        }
        break;
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
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<u32, std::allocator<u32> >;
template void std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::sub_push_back(const MMotionFile &x, Metrowerks::int2type<false>);
template void std::__vec_deleter<u32, std::allocator<u32> >::assign<u32 *>(u32 *first, u32 *last, Metrowerks::int2type<true>);

// The list's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x1C];
    void (DinoUiMedalList::*Start_1C)();
    void (DinoUiMedalList::*StateResult_24)();
    void (DinoUiMedalList::*StateMenu_2C)();
    void (DinoUiMedalList::*StateMenu_34)();
} data_ov006_02194158;

DinoUiMedalList::DinoUiMedalList() : MProcess(NULL), mUnk7C(1), mUnk80(0), mUnk84(0), mUnk85(0) {
    mState = data_ov006_02194158.Start_1C;
}

void DinoUiMedalList::StateMenu() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 1, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    if (mUnk7C == 1) {
        mUnk80 += mUnk85 * 15;
    }
    mSerial.AddTail(new DinoUiMedalMenu(&mUnk7C, &mUnk80, &mUnk84));
    mState = data_ov006_02194158.StateResult_24;
}

void DinoUiMedalList::StateResult() {
    if (mUnk7C == 0) {
        gLcd->GetTopDisplay()->mBright->Reset();
        gLcd->GetBottomDisplay()->mBright->Reset();
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    if (mUnk7C == 1) {
        mUnk85 = mUnk80 / 15;
        mUnk80 = mUnk80 % 15;
        mSerial.AddTail(new DinoUiRvStatusMenu((int *)&mUnk80));
        mState = data_ov006_02194158.StateMenu_2C;
    } else if (mUnk7C == 2) {
        mSerial.AddTail(new DinoUiRvStatusMenu((int *)&mUnk80, mUnk84));
        mState = data_ov006_02194158.StateMenu_34;
    } else {
        Exit();
    }
}

extern const int data_02089aa4, data_02089aa8, data_02089aac, data_02089ab0, data_02089ab4, data_02089ab8;

void DinoUiMedalMenu::OnBackDecided(BOOL result) {
    *mResult = result;
    if (result == 0) {
        MTask *exit = MTaskExitTransfer::Create(&(MTask &)*this);
        MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(data_02089ab4, data_02089ab0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fade);
        GetSerial()->AddTail(new MTimer(data_02089ab8), MTaskSetWaitEmptyTask::Create(mUnk220), fades, exit);
    } else {
        MTask *exit = MTaskExitTransfer::Create(&(MTask &)*this);
        MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(data_02089aa8, data_02089aa4);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fade);
        GetSerial()->AddTail(new MTimer(data_02089aac), MTaskSetWaitEmptyTask::Create(mUnk220), fades, exit);
    }
}
