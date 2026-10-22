// DinoUi2ContainerBaseMenuItem.
// Only the destructors are decompiled so far.

// The buttons' destructors have the base destructor body inlined; auto-inlining the one defined above does that.
#pragma auto_inline on

#include "game/dino_ui2_parts_rock_name.h"
#include "game/dino_ui_fossil_entry.h"

#include "game/dino_ui2_field_menu_cursor.h"

#include "m/input.h"

#include "game/dino_ui2_parts_kaseki_button.h"

#include "m/blend.h"
#include "game/dino_ui2_container_base_menu_item_menu.h"
#include "game/dino_ui2_container_base_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_backup.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_guide_message.h"
#include "game/help.h"
#include "m/picture.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"

#pragma auto_inline off

BOOL DinoUi2ContainerBaseMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

#pragma auto_inline on

#pragma auto_inline off

BOOL DinoUi2ContainerBaseMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

#pragma auto_inline on

DinoUi2ContainerBaseMenuItem::~DinoUi2ContainerBaseMenuItem() {
}

DinoUi2ContainerBaseBackButton::~DinoUi2ContainerBaseBackButton() {
}

DinoUi2ContainerBasePageLButton::~DinoUi2ContainerBasePageLButton() {
}

DinoUi2ContainerBasePageRButton::~DinoUi2ContainerBasePageRButton() {
}

DinoUi2ContainerBaseDecideButton::~DinoUi2ContainerBaseDecideButton() {
}

#pragma auto_inline off

DinoUi2ContainerBaseMenu::~DinoUi2ContainerBaseMenu() {
    GetDisplay()->mBlend->Reset();
}

DinoUi2CannonShootMenu::~DinoUi2CannonShootMenu() {
}

#pragma auto_inline on

MRect DinoUi2ContainerBaseMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUi2ContainerBaseMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUi2ContainerBaseMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUi2ContainerBaseMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUi2ContainerBaseMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUi2ContainerBaseBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUi2ContainerBaseBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUi2ContainerBasePageRButton::OnDecide() {
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

void DinoUi2ContainerBasePageLButton::OnDecide() {
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

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUi2CannonShootMenu, DinoUi2CannonShootMenu>;

void DinoUi2ContainerBaseDecideButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mOwner->mUnkFC) {
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
struct DinoUi2ContainerBaseStatics {
    u8 unk00[4];
    const char *mBackPath;      // 0x04
    const char *mPageRPath;     // 0x08
    const char *mPageLPath;     // 0x0C
    MPoint mPageRPos;           // 0x10
    u8 unk18[0x20 - 0x18];
    MPoint mBackPos;            // 0x20
    MPoint mRow;                // 0x28: from a page button to the next row's
    u8 unk30[0x38 - 0x30];
    MPoint mPageLPos;           // 0x38
    u8 unk40[0x50 - 0x40];
    MPoint mDecideTopLeft;      // 0x50
    u8 unk58[0x60 - 0x58];
    MPoint mRockNamePos;        // 0x60
    MPoint mDecidePos;          // 0x68
    MPoint mColumn;             // 0x70: from a page button to the next in its row
    MPoint mDecideBottomRight;  // 0x78
};
extern "C" DinoUi2ContainerBaseStatics data_ov004_02176b44;

MRect DinoUi2ContainerBaseDecideButton::GetFrame() {
    return MRect(data_ov004_02176b44.mDecideTopLeft, data_ov004_02176b44.mDecideBottomRight);
}

void DinoUi2ContainerBaseDecideButton::OnDecide() {
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

void DinoUi2ContainerBaseMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
    mItems.clear();
}

extern MView *data_ov004_02177284; // the view the cross-fade keeps

#pragma auto_inline off

void DinoUi2ContainerBaseMenu::OnBackDecided() {
    MTask &self = *this;
    MTask *timer;
    MTask *hide;
    MTask *fade;
    MTask *exit;
    exit = MTaskExitTransfer::Create(&self);
    MBlend *blend = gCurrentDisplay->mBlend;
    fade = blend->CreateCrossFadeOutTask(data_ov004_02177284->GetPlaneMask() ^ 0x3F, 5, 0);
    hide = mOam->CreateHideTransfer();
    timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(hide);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

#pragma auto_inline off

template MTask *MCreateCallTask(DinoUi2CannonShootMenu *obj, void (DinoUi2CannonShootMenu::*func)());

void DinoUi2ContainerBaseMenu::CreateDecideButton(const char *motion, u32 textId) {
    AddItem(mLinkFrom = new DinoUi2ContainerBaseDecideButton(this, motion, textId));
    AddLink(mLinkFrom, 1, NULL);
    AddLink(mLinkFrom, 3, NULL);
}

DinoUi2ContainerBaseDecideButton::DinoUi2ContainerBaseDecideButton(DinoUi2ContainerBaseMenu *owner,
                                                                   std::string motion, u32 textId)
    : DinoUi2ContainerBaseMenuItem(owner, motion, data_ov004_02176b44.mDecidePos, textId) {
}

DinoUi2ContainerBaseMenuItem::DinoUi2ContainerBaseMenuItem(DinoUi2ContainerBaseMenu *owner, std::string motion,
                                                           MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(motion), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

u8 DinoUi2ContainerBaseMenu::GetFlag(int index) {
    return mFlags[index];
}

DinoUi2ContainerBaseMenu::DinoUi2ContainerBaseMenu()
    : MMenu(NULL, 0), mPage(0), mLastPage(0), mUnkFC(0), mSelected(0xFF), mIdled(FALSE) {
    for (int i = 0; i < 0x40; i++) {
        mFlags[i] = 0;
    }
    SetDeferStart(TRUE);
    mItems.clear();
}

// The help for the decide and back buttons.
void DinoUi2ContainerBaseMenu::AddHelpEntries(u32 backText) {
    AddEntry((u32)(DinoHelpItem *)mLinkFrom, 1, 0);
    AddEntry((u32)(DinoHelpItem *)mBack, 4, backText);
}

extern "C" const u32 data_0208a574; // the back button's help text
extern "C" const u32 data_0208aacc; // the page buttons' help text

inline DinoUi2ContainerBaseBackButton::DinoUi2ContainerBaseBackButton(DinoUi2ContainerBaseMenu *owner)
    : DinoUi2ContainerBaseMenuItem(owner, data_ov004_02176b44.mBackPath, data_ov004_02176b44.mBackPos, data_0208a574) {
    mMotion->Play(2, 0);
}

inline DinoUi2ContainerBasePageLButton::DinoUi2ContainerBasePageLButton(DinoUi2ContainerBaseMenu *owner)
    : DinoUi2ContainerBaseMenuItem(owner, data_ov004_02176b44.mPageLPath, data_ov004_02176b44.mPageLPos, data_0208aacc) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUi2ContainerBasePageRButton::DinoUi2ContainerBasePageRButton(DinoUi2ContainerBaseMenu *owner)
    : DinoUi2ContainerBaseMenuItem(owner, data_ov004_02176b44.mPageRPath, data_ov004_02176b44.mPageRPos, data_0208aacc) {
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

void DinoUi2ContainerBaseMenu::OnStart() {
    Disable();
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    mOam = new MSmartOam(0x54, 0, TRUE);
    mOam->mHidden = TRUE;
    mBack = new DinoUi2ContainerBaseBackButton(this);
    AddItem(mBack);
    RefreshEntries();
    if (mLastPage > 0) {
        mPageL = new DinoUi2ContainerBasePageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUi2ContainerBasePageRButton(this);
        AddItem(mPageR);
    }
    Setup();
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUi2FieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(1);
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mRockName = new DinoUi2PartsRockName(data_ov004_02176b44.mRockNamePos);
    mParallel.AddTail(mRockName);
    RefreshPage2();
    if (mItems.size() == 0) {
        FocusItem(mBack);
    } else {
        FocusItem((MMenuItem *)mItems[0]);
    }
    AddLink(mBack, 4, mLinkFrom);
    mSerial.AddTail(mOam->CreateShowTransfer());
    Enable();
}

void DinoUi2ContainerBaseMenu::ResetDecideButton() {
    DinoUi2ContainerBaseMenuItem *decide = mLinkFrom;
    decide->mSerial.Clear();
    decide->mMotion->Play(1, 0);
}

// The page's first button.
extern "C" MPoint data_ov004_02176bc4;
extern "C" const u32 data_0208aac8; // the empty slots' help text

// Makes the page's buttons (blank past the last entry) and links them in a grid.
void DinoUi2ContainerBaseMenu::RefreshPage2() {
    for (int i = 0; i < 8; i++) {
        DinoUi2PartsKasekiButton *button;
        if (mUnk134.size() > i + mPage * 8) {
            DinoBackupEntry entry;
            GetEntry(i + mPage * 8, &entry);
            button = new DinoUi2PartsKasekiButton(
                mOam,
                MPoint(data_ov004_02176bc4.x + data_ov004_02176b44.mColumn.x * (i % 4) +
                           data_ov004_02176b44.mRow.x * (i / 4),
                       data_ov004_02176bc4.y + data_ov004_02176b44.mColumn.y * (i % 4) +
                           data_ov004_02176b44.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)mUnk134[i + mPage * 8], GetFlag(i + mPage * 8), mUnk100);
            button->SetActive(IsMarkable(&entry));
        } else {
            button = new DinoUi2PartsKasekiButton(
                mOam,
                MPoint(data_ov004_02176bc4.x + data_ov004_02176b44.mColumn.x * (i % 4) +
                           data_ov004_02176b44.mRow.x * (i / 4),
                       data_ov004_02176bc4.y + data_ov004_02176b44.mColumn.y * (i % 4) +
                           data_ov004_02176b44.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)DinoUi2GetEmptyKasekiInfo(), 0, data_0208aac8);
            button->HideMotion();
        }
        mItems.push_back(button);
        AddItem(button);
    }
    DinoUi2LinkKasekiGrid(this, (std::vector<MMenuItem *> &)mItems, mLinkFrom);
    AddLink(mLinkFrom, 4, (MMenuItem *)mItems[4]);
    RefreshMarks();
}

// Entries that can't be marked now (and aren't) are greyed out.
void DinoUi2ContainerBaseMenu::RefreshMarks() {
    for (int i = 0; i < mItems.size(); i++) {
        DinoBackupEntry entry;
        GetEntry(mPage * 8 + i, &entry);
        if (!GetFlag(mPage * 8 + i) && (!IsMarkable(&entry) || !CanMark())) {
            ((DinoUi2PartsKasekiButton *)mItems[i])->SetActive(FALSE);
        } else {
            ((DinoUi2PartsKasekiButton *)mItems[i])->SetActive(TRUE);
        }
    }
}

void DinoUi2ContainerBaseMenu::SetFlag(int index, u8 value) {
    if (value != mFlags[index]) {
        mFlags[index] = value;
        if (value) {
            mUnkFC++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mUnkFC--;
            DinoSoundPlaySe(0x9D, 0);
        }
        RefreshMarks();
    }
}

// X jumps to the decide button; B unmarks the selected entry, or goes to (and then presses) the back button.
void DinoUi2ContainerBaseMenu::OnIdle() {
    mIdled = TRUE;
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi2PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
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
void DinoUi2ContainerBaseMenu::OnFocus(MMenuItem *item) {
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
            if (index >= mUnk134.size()) {
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
void DinoUi2ContainerBaseMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            if (!mIdled) {
                return;
            }
            u32 index = mPage * 8 + i;
            if (index >= mUnk134.size()) {
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
            ((DinoUi2PartsKasekiButton *)mItems[i])->SetMarked(flag);
            return;
        }
    }
}

void DinoUi2CannonShootMenu::GetEntry(u32 index, DinoBackupEntry *entry) {
    gDinoBackup->GetUnk14C(index, entry);
}

BOOL DinoUi2CannonShootMenu::CanMark() {
    return mUnkFC < 10;
}

BOOL DinoUi2CannonShootMenu::IsMarkable(DinoBackupEntry *entry) {
    return TRUE;
}

extern const u32 data_0208a9c4, data_0208aa04, data_0208aa08;

DinoUi2CannonShootMenu::DinoUi2CannonShootMenu(std::vector<void *> *chosen) {
    mChosen = chosen;
    mChosen->clear();
    mUnkFD = 8;
}

// The background picture's path (the TU's string literals are pooled data too).
extern "C" const char data_ov004_02177130[];
extern "C" struct {
    u32 unk00;
    const char *decideMotion;   // 0x04
} data_ov004_02176fb0;
extern "C" const u32 data_0208a4d8, data_0208a4d0, data_0208a4d4, data_0208a918;

void DinoUi2CannonShootMenu::Setup() {
    mParallel.AddTail(mBackground = new MTextPicture(data_ov004_02177130, TRUE, false));
    mBackground->Hide();
    mBackground->SetPriority(2);
    MBlend *blend = gCurrentDisplay->mBlend;
    GetSerial()->AddTail(mBackground->CreateShowTransfer(),
                         blend->CreateCrossFadeInTask(mBackground->GetPlaneMask(), 5, 0));
    gDinoBackup->CompactUnk14C();
    mParallel.AddTail(new DinoHelpDummyItem(GetDisplay(), MRect(0x1E, 0xC, 0xE2, 0x6C),
                                            std::string(gDinoResource->GetText(data_0208a4d8))));
    DinoHelpDummyItem *back;
    mParallel.AddTail(back = new DinoHelpDummyItem(GetDisplay(), MRect(0x13, 0x90, 0xF1, 0x9F),
                                                   std::string(gDinoResource->GetText(data_0208a4d0))));
    CreateDecideButton(data_ov004_02176fb0.decideMotion, data_0208a4d4);
    AddHelpEntries((u32)back);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a918))));
}

// After the yes/no: hands back the marked entries and leaves.
void DinoUi2CannonShootMenu::FinishDecide() {
    if (mConfirmed == TRUE) {
        for (int i = 0; i < 8; i++) {
            if (mFlags[i]) {
                mFlags[i] = FALSE;
                mChosen->push_back((void *)i);
            }
        }
        mUnkFC = 0;
        ResetDecideButton();
        OnBackDecided();
    }
}

// Asks whether to load the marked fossils.
BOOL DinoUi2CannonShootMenu::TryDecide() {
    if (mUnkFC == 0) {
        DinoSoundPlaySe(0xA4, 0);
        return FALSE;
    }
    mCursor->Hide();
    mConfirmed = FALSE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208a9c4), &mConfirmed);
    dialog->SetLabels(gDinoResource->GetText(data_0208aa08), gDinoResource->GetText(data_0208aa04));
    MTask *finish = MCreateCallTask(this, &DinoUi2CannonShootMenu::FinishDecide);
    MTask *show = mCursor->CreateShowTransfer();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(dialog);
    serial->AddTail(show);
    serial->AddTail(finish);
    return TRUE;
}

extern const u32 data_0208a988; // text: no more can be loaded

MTask *DinoUi2CannonShootMenu::CreateUnk2C() {
    return new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208a988), 4);
}

#pragma auto_inline off
// The fossil list's vector members the game emits in this file (made explicitly until the code that fills the list is
// decompiled).
template void std::__vec_deleter<DinoUiFossilEntry, std::allocator<DinoUiFossilEntry> >::sub_push_back(const DinoUiFossilEntry &x, Metrowerks::int2type<true>);

// (DinoUi2GetKasekiButtonInfo seen as returning the list entry, as DinoGetFossilEntry.)
void DinoUi2ContainerBaseMenu::RefreshEntries() {
    mUnk134.clear();
    for (int i = 0; i < mUnkFD; i++) {
        DinoBackupEntry entry;
        GetEntry(i, &entry);
        if (entry.id) {
            mUnk134.push_back(((DinoUiFossilEntry(*)(const DinoBackupEntry *))DinoUi2GetKasekiButtonInfo)(&entry));
        }
    }
    if (mUnk134.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mUnk134.size() - 1) / 8;
    }
}


// The file's static initialization (see dino_static_init.h).
// NONMATCHING: register choice/scheduling of the pooled stores.
static struct DinoUi2ContainerBaseMenuItemStaticInit {
    DinoUi2ContainerBaseMenuItemStaticInit() {
        DINO_POOL_WORD(data_ov004_02176b44, 0x20) = 128;
        DINO_POOL_WORD(data_ov004_02176b44, 0x24) = 96;
        DINO_POOL_WORD(data_ov004_02176b44, 0x38) = 128;
        DINO_POOL_WORD(data_ov004_02176b44, 0x3c) = 96;
        DINO_POOL_WORD(data_ov004_02176b44, 0x10) = 128;
        DINO_POOL_WORD(data_ov004_02176b44, 0x14) = 96;
        DINO_POOL_WORD(data_ov004_02176bc4, 0x0) = 53;
        DINO_POOL_WORD(data_ov004_02176bc4, 0x4) = 35;
        DINO_POOL_WORD(data_ov004_02176b44, 0x70) = 50;
        DINO_POOL_WORD(data_ov004_02176b44, 0x74) = 0;
        DINO_POOL_WORD(data_ov004_02176b44, 0x28) = 0;
        DINO_POOL_WORD(data_ov004_02176b44, 0x2c) = 50;
        DINO_POOL_WORD(data_ov004_02176b44, 0x18) = -32;
        DINO_POOL_WORD(data_ov004_02176b44, 0x1c) = 4;
        DINO_POOL_WORD(data_ov004_02176b44, 0x30) = 241;
        DINO_POOL_WORD(data_ov004_02176b44, 0x34) = 103;
        DINO_POOL_WORD(data_ov004_02176b44, 0x68) = 128;
        DINO_POOL_WORD(data_ov004_02176b44, 0x6c) = 96;
        DINO_POOL_WORD(data_ov004_02176b44, 0x60) = 24;
        DINO_POOL_WORD(data_ov004_02176b44, 0x64) = 144;
        DINO_POOL_WORD(data_ov004_02176b44, 0x50) = 54;
        DINO_POOL_WORD(data_ov004_02176b44, 0x54) = 115;
        DINO_POOL_WORD(data_ov004_02176b44, 0x78) = 202;
        DINO_POOL_WORD(data_ov004_02176b44, 0x7c) = 139;
    }
} sDinoUi2ContainerBaseMenuItemStaticInit;
