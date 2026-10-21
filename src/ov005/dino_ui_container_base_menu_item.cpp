// DinoUiContainerBaseMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui3_parts_rock_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "game/dino_backup.h"
#include "game/dino_ui_common.h"

#include "m/input.h"

#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui2_parts_kaseki_button.h"

#include "game/dino_ui_container_base_menu_item_menu.h"
#include "game/dino_ui_container_base_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_static_init.h"

DinoUiContainerBaseMenuItem::~DinoUiContainerBaseMenuItem() {
}

DinoUiContainerBaseBackButton::~DinoUiContainerBaseBackButton() {
}

DinoUiContainerBasePageLButton::~DinoUiContainerBasePageLButton() {
}

DinoUiContainerBasePageRButton::~DinoUiContainerBasePageRButton() {
}

DinoUiContainerBaseDecideButton::~DinoUiContainerBaseDecideButton() {
}

DinoUiContainerBaseMenu::DinoUiContainerBaseMenu()
    : MMenu(NULL, 0), mPage(0), mLastPage(0), mCount(0), mUnk104(0), mSelected(0xFF), mIdled(FALSE) {
    for (int i = 0; i < 0x40; i++) {
        mFlags[i] = 0;
    }
    mItems.clear();
}

DinoUiContainerBaseMenu::~DinoUiContainerBaseMenu() {
}

MRect DinoUiContainerBaseMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerBaseMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerBaseMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerBaseMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerBaseMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerBaseBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBaseBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiContainerBasePageRButton::OnDecide() {
    if (mOwner->PageRight()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiContainerBasePageLButton::OnDecide() {
    if (mOwner->PageLeft()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiContainerBaseMenu::OnBackDecided() {
    MTask *fade;
    MTask *exit;
    MTask &self = *this;
    exit = MTaskExitTransfer::Create(&self);
    fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

void DinoUiContainerBaseDecideButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mOwner->mCount) {
            mMotion->Play(2, 0);
        } else {
            mMotion->Play(1, 0);
        }
    } else {
        mMotion->Play(1, 0);
        mSerial.Clear();
    }
}

// Pooled file statics: the button corners.
struct DinoUiContainerBaseStatics {
    u8 unk00[4];
    const char *mBackPath;      // 0x04
    const char *mPageRPath;     // 0x08
    const char *mPageLPath;     // 0x0C
    MPoint mColumn;      // 0x10: from a page button to the next in its row
    u8 unk18[0x20 - 0x18];
    MPoint mBackPos;     // 0x20
    MPoint mRockNamePos; // 0x28
    MPoint mFractionPos; // 0x30
    MPoint mPageLPos;    // 0x38
    u8 unk40[0x50 - 0x40];
    MPoint mTopLeft;     // 0x50
    u8 unk58[8];
    MPoint mRow;         // 0x60: to the next row
    MPoint mDecidePos;   // 0x68
    MPoint mFirst;       // 0x70: the page's first button
    MPoint mBottomRight; // 0x78
    MPoint mPageRPos;    // 0x80
};
extern "C" DinoUiContainerBaseStatics data_ov005_0216c2e8;

MRect DinoUiContainerBaseDecideButton::GetFrame() {
    return MRect(data_ov005_0216c2e8.mTopLeft, data_ov005_0216c2e8.mBottomRight);
}

void DinoUiContainerBaseDecideButton::OnDecide() {
    if (mOwner->TryDecide()) {
        DinoSoundPlaySe(0x98, 0);
        mMotion->Play(3, 1);
        MTask *play = mMotion->CreatePlayTransfer(2, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
}

BOOL DinoUiContainerBaseMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

BOOL DinoUiContainerBaseMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

void DinoUiContainerBaseMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
    mItems.clear();
}

#pragma auto_inline off

extern "C" const u32 data_0208aacc; // the page buttons' help text

inline DinoUiContainerBaseBackButton::DinoUiContainerBaseBackButton(DinoUiContainerBaseMenu *owner, u32 textId)
    : DinoUiContainerBaseMenuItem(owner, data_ov005_0216c2e8.mBackPath, data_ov005_0216c2e8.mBackPos, textId) {
    mMotion->Play(2, 0);
}

inline DinoUiContainerBasePageLButton::DinoUiContainerBasePageLButton(DinoUiContainerBaseMenu *owner)
    : DinoUiContainerBaseMenuItem(owner, data_ov005_0216c2e8.mPageLPath, data_ov005_0216c2e8.mPageLPos, data_0208aacc) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiContainerBasePageRButton::DinoUiContainerBasePageRButton(DinoUiContainerBaseMenu *owner)
    : DinoUiContainerBaseMenuItem(owner, data_ov005_0216c2e8.mPageRPath, data_ov005_0216c2e8.mPageRPos, data_0208aacc) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

void DinoUiContainerBaseMenu::OnStart() {
    DinoBackupEntry entry;
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    mDefs = gDinoResource->LoadKasekiDefs();
    mOam = new MSmartOam(0x54, 0, TRUE);
    mBack = new DinoUiContainerBaseBackButton(this, mUnk104);
    AddItem(mBack);
    int count = 0;
    for (int i = 0; i < mUnk101; i++) {
        GetEntry(i, &entry);
        if (entry.id) {
            count++;
        }
    }
    mLastPage = count != 0 ? (u32)(count - 1) / 8 : 0;
    if (mLastPage > 0) {
        mPageL = new DinoUiContainerBasePageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiContainerBasePageRButton(this);
        AddItem(mPageR);
    }
    mFraction = new DinoUi3PartsFraction(mOam, data_ov005_0216c2e8.mFractionPos);
    mParallel.AddTail(mFraction);
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetDenominator(mLastPage + 1);
    mFraction->Hide();
    Setup();
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUi3PartsCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(1);
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mRockName = new DinoUi3PartsRockName(data_ov005_0216c2e8.mRockNamePos);
    mParallel.AddTail(mRockName);
    RefreshEntries();
    RefreshPage2();
    if (mItems.size() == 0) {
        FocusItem(mBack);
    } else {
        FocusItem((MMenuItem *)mItems[0]);
    }
    AddLink(mBack, 4, mLinkFrom);
    DinoDimScreen();
    mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(30, 0));
}

void DinoUiContainerBaseMenu::RefreshEntries() {
    mUnk148.clear();
    for (int i = 0; i < mUnk101; i++) {
        DinoBackupEntry entry;
        GetEntry(i, &entry);
        if (entry.id) {
            mUnk148.push_back(DinoGetFossilEntry(&entry, mDefs));
        }
    }
    if (mUnk148.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mUnk148.size() - 1) / 8;
    }
}

void DinoUiContainerBaseMenu::CreateDecideButton(const char *motion, u32 textId) {
    AddItem(mLinkFrom = new DinoUiContainerBaseDecideButton(this, motion, textId));
    AddLink(mLinkFrom, 1, NULL);
    AddLink(mLinkFrom, 3, NULL);
}

DinoUiContainerBaseDecideButton::DinoUiContainerBaseDecideButton(DinoUiContainerBaseMenu *owner, std::string motion,
                                                                 u32 textId)
    : DinoUiContainerBaseMenuItem(owner, motion, data_ov005_0216c2e8.mDecidePos, textId) {
}

DinoUiContainerBaseMenuItem::DinoUiContainerBaseMenuItem(DinoUiContainerBaseMenu *owner, std::string motion,
                                                         MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(motion), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->GetUnit()->SetMode(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

u8 DinoUiContainerBaseMenu::GetFlag(int index) {
    return mFlags[index];
}

// The help for the decide and back buttons.
void DinoUiContainerBaseMenu::AddHelpEntries(u32 backText) {
    AddEntry((u32)(DinoHelpItem *)mLinkFrom, 1, 0);
    AddEntry((u32)(DinoHelpItem *)mBack, 4, backText);
}

void DinoUiContainerBaseMenu::ResetDecideButton() {
    DinoUiContainerBaseMenuItem *decide = mLinkFrom;
    decide->mSerial.Clear();
    decide->mMotion->Play(1, 0);
}

void DinoUiContainerBaseMenu::HidePageButtons() {
    mPageL->MMenuItem::Disable();
    mPageL->mMotion->Hide();
    mPageR->MMenuItem::Disable();
    mPageR->mMotion->Hide();
}

// Entries that can't be marked now (and aren't) are greyed out.
// Makes the page's buttons (blank past the last entry) and links them in a grid.
void DinoUiContainerBaseMenu::RefreshPage2() {
    for (int i = 0; i < 8; i++) {
        DinoUi3PartsKasekiButton *button;
        if (mUnk148.size() > i + mPage * 8) {
            DinoBackupEntry entry;
            GetEntry(i + mPage * 8, &entry);
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_0216c2e8.mFirst.x + data_ov005_0216c2e8.mColumn.x * (i % 4) +
                           data_ov005_0216c2e8.mRow.x * (i / 4),
                       data_ov005_0216c2e8.mFirst.y + data_ov005_0216c2e8.mColumn.y * (i % 4) +
                           data_ov005_0216c2e8.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)mUnk148[i + mPage * 8], GetFlag(i + mPage * 8));
            button->SetActive(IsMarkable(&entry));
            button->SetMode(4);
        } else {
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_0216c2e8.mFirst.x + data_ov005_0216c2e8.mColumn.x * (i % 4) +
                           data_ov005_0216c2e8.mRow.x * (i / 4),
                       data_ov005_0216c2e8.mFirst.y + data_ov005_0216c2e8.mColumn.y * (i % 4) +
                           data_ov005_0216c2e8.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)DinoUi2GetEmptyKasekiInfo(), 0);
            button->HideMotion();
        }
        mItems.push_back(button);
        AddItem(button);
    }
    DinoUi2LinkKasekiGrid(this, (std::vector<MMenuItem *> &)mItems, mLinkFrom);
    AddLink(mLinkFrom, 4, (MMenuItem *)mItems[4]);
    RefreshMarks();
}

void DinoUiContainerBaseMenu::RefreshMarks() {
    for (int i = 0; i < mItems.size(); i++) {
        DinoBackupEntry entry;
        GetEntry(mPage * 8 + i, &entry);
        if (!GetFlag(mPage * 8 + i) && (!IsMarkable(&entry) || !CanMark())) {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(FALSE);
        } else {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(TRUE);
        }
    }
}

void DinoUiContainerBaseMenu::SetFlag(int index, u8 flag) {
    if (flag != mFlags[index]) {
        mFlags[index] = flag;
        if (flag) {
            mCount++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mCount--;
            DinoSoundPlaySe(0x9D, 0);
        }
        RefreshMarks();
    }
}

// X jumps to the decide button; B unmarks the selected entry, or goes to (and then presses) the back button.
void DinoUiContainerBaseMenu::OnIdle() {
    mIdled = TRUE;
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi3PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
            return;
        }
        if (mBack->IsFocused()) {
            mBack->Decide();
        } else {
            FocusItem(mBack);
        }
    }
}

// Moves the cursor to the item, and shows the entry's name (marked if it can't be marked now).
void DinoUiContainerBaseMenu::OnFocus(MMenuItem *item) {
    mIdled = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), item->GetFrame(), 10, -5));
    }
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            u32 index = mPage * 8 + i;
            if (index >= mUnk148.size()) {
                continue;
            }
            DinoBackupEntry entry;
            GetEntry(index, &entry);
            if (!GetFlag(mPage * 8 + i) && (!IsMarkable(&entry) || !CanMark())) {
                mRockName->SetEntry(&entry, TRUE);
            } else {
                mRockName->SetEntry(&entry, FALSE);
            }
            mSelected = mPage * 8 + i;
            return;
        }
    }
    mRockName->Clear();
    mSelected = 0xFF;
}

// An entry button: marks it (if it and the container allow) or unmarks it.
void DinoUiContainerBaseMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            if (!mIdled) {
                return;
            }
            u32 index = mPage * 8 + i;
            if (index >= mUnk148.size()) {
                return;
            }
            u8 flag = !GetFlag(index);
            if (flag) {
                DinoBackupEntry entry;
                GetEntry(mPage * 8 + i, &entry);
                if (!IsMarkable(&entry)) {
                    DinoSoundPlaySe(0xA4, 0);
                    return;
                }
                if (!CanMark()) {
                    DinoSoundPlaySe(0xA4, 0);
                    mSerial.AddTail(CreateUnk2C());
                    return;
                }
            }
            SetFlag(mPage * 8 + i, flag);
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetMarked(flag);
            return;
        }
    }
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov005_0216c368[];
static struct DinoUiContainerBaseMenuItemStaticInit {
    DinoUiContainerBaseMenuItemStaticInit() {
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x20) = 128;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x24) = 96;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x38) = 128;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x3c) = 96;
        DINO_POOL_WORD(data_ov005_0216c368, 0x0) = 128;
        DINO_POOL_WORD(data_ov005_0216c368, 0x4) = 96;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x70) = 53;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x74) = 35;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x10) = 50;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x14) = 0;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x60) = 0;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x64) = 50;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x18) = -32;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x1c) = 4;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x30) = 241;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x34) = 103;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x68) = 128;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x6c) = 96;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x28) = 24;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x2c) = 144;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x50) = 54;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x54) = 115;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x78) = 202;
        DINO_POOL_WORD(data_ov005_0216c2e8, 0x7c) = 139;
    }
} sDinoUiContainerBaseMenuItemStaticInit;
