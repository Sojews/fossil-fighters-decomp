// DinoUiContainerDropMenuPageIncTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_common.h"

#include "game/dino_ui_container_drop_menu_page_inc_task_menu.h"
#include "game/dino_ui_container_drop_menu_page_inc_task.h"
#include "game/dino_ui_container_drop_menu_item.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_ui3_parts_fraction.h"
#include "game/dino_ui3_parts_cursor.h"
#include "game/dino_ui3_parts_rock_name.h"
#include "game/dino_backup.h"
#include "game/dino_static_init.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_guide_message.h"
#include "m/input.h"
#include "m/smart_oam.h"
#include "m/picture.h"
#include "m/obj.h"

// The text with every tag replaced by the value (unless the value has the tag itself).
// NONMATCHING: ours keeps an unreachable epilogue after the loop (as ov003's copy).
std::string DinoReplaceTag(std::string text, std::string tag, std::string value) {
    if (value.find(tag, 0) != std::string::npos) {
        return text;
    }
    while (true) {
        std::string::size_type pos = text.find(tag, 0);
        if (pos == std::string::npos) {
            return text;
        }
        text = text.replace(pos, tag.size(), value);
    }
}

#pragma auto_inline off

DinoUiContainerDropMenu::DinoUiContainerDropMenu(u32 arg0, u32 arg1, const std::vector<std::string> &names)
    : MMenu(NULL, 0), mUnkDC(arg0), mUnkE0((const DinoBackupEntry *)arg1), mUnkE4(names), mPage(0), mLastPage(0), mUnk120(-1), mStarted(0),
      mUnk128(0) {
}

// The TU's pooled paths (see the static initialization).
extern "C" u32 data_ov005_02169188[];
extern "C" u32 data_ov005_02169288[];
extern "C" u32 data_ov005_02169308[];
// Where the buttons go (the kaseki buttons: two rows of four).
extern struct {
    u8 unk00[0x34];
    MPoint mBackPos;    // 0x34
    MPoint mPageLPos;   // 0x3C
    MPoint mPageRPos;   // 0x44
    int x;          // 0x4C: the first button
    int y;          // 0x50
    int columnX;    // 0x54: to the next in a row
    int columnY;    // 0x58
    int rowX;       // 0x5C: to the next row
    int rowY;       // 0x60
    MPoint mFractionPos;    // 0x64
    MPoint mRockNamePos;    // 0x6C
} data_ov005_02169388;
// The top screen's pictures.
extern "C" const char data_ov005_0216b154[], data_ov005_0216b164[];
extern const u32 data_0208a668, data_0208aab4, data_0208a670, data_0208a924, data_0208aab8;
extern const u32 data_0208a660, data_0208a664;

inline DinoUiContainerDropBackButton::DinoUiContainerDropBackButton(DinoUiContainerDropMenu *owner)
    : DinoUiContainerDropMenuItem(owner, (const char *)data_ov005_02169308[0x6c / 4], data_ov005_02169388.mBackPos,
                                  data_0208a668) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
}

inline DinoUiContainerDropPageLButton::DinoUiContainerDropPageLButton(DinoUiContainerDropMenu *owner)
    : DinoUiContainerDropMenuItem(owner, (const char *)data_ov005_02169188[0x18 / 4], data_ov005_02169388.mPageLPos,
                                  data_0208aab4) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiContainerDropPageRButton::DinoUiContainerDropPageRButton(DinoUiContainerDropMenu *owner)
    : DinoUiContainerDropMenuItem(owner, (const char *)data_ov005_02169288[0x48 / 4], data_ov005_02169388.mPageRPos,
                                  data_0208aab4) {
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

void DinoUiContainerDropMenu::OnStart() {
    DinoPlayerData player;
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->VSyncNow();
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    mOam = new MSmartOam(0x50, 0, TRUE);
    DinoHelpDummyItem *rockName;
    mParallel.AddTail(rockName = new DinoHelpDummyItem(GetDisplay(), MRect(0xE7, 0x73, 0xFA, 0x8A),
                                                       std::string(gDinoResource->GetText(data_0208a670))));
    mParallel.AddTail(mHelp12C = new DinoHelpDummyItem(GetDisplay(), MRect(0x19, 0x20, 0xE6, 0x89),
                                                       std::string(gDinoResource->GetText(data_0208a664))));
    mParallel.AddTail(mHelp130 = new DinoHelpDummyItem(GetDisplay(), MRect(0xF, 0x92, 0xF0, 0xA4),
                                                       std::string(gDinoResource->GetText(data_0208a660))));
    AddEntry((u32)(DinoHelpItem *)rockName, 3, (u32)(DinoHelpItem *)mHelp12C);
    mUnkB8 = new DinoUiContainerDropBackButton(this);
    AddItem(mUnkB8);
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    gDinoBackup->GetPlayer(&player, 0);
    gDinoBackup->CompactUnk98();
    LoadIcons();
    if (mLastPage != 0) {
        mPageL = new DinoUiContainerDropPageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiContainerDropPageRButton(this);
        AddItem(mPageR);
    }
    mPage = mLastPage;
    mFraction = new DinoUi3PartsFraction(mOam, data_ov005_02169388.mFractionPos);
    mParallel.AddTail(mFraction);
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetDenominator(player.containerNum + 1);
    CreateItems();
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUi3PartsCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(1);
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mRockName = new DinoUi3PartsRockName(data_ov005_02169388.mRockNamePos);
    mParallel.AddTail(mRockName);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a924))));
    mParallel.AddTail(mTitle = new MTextPicture(std::string(data_ov005_0216b154), TRUE, false));
    mTitle->Show();
    mTitle->SetPriority(3);
    mParallel.AddTail(mUnkC0 = new MTextPicture(std::string(data_ov005_0216b164), TRUE, false));
    mUnkC0->Show();
    mUnkC0->SetPriority(2);
    if (mItems.size() == 0) {
        FocusItem(mUnkB8);
    } else {
        FocusItem(mItems[0]);
    }
    *(int *)mUnkDC = -1;
    mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(30, 0));
    GetSerial()->AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x70), gDinoResource->GetText(data_0208aab8), 4),
                         new DinoUiContainerDropMenuPageIncTask(this));
}

DinoUiContainerDropMenuItem::DinoUiContainerDropMenuItem(DinoUiContainerDropMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

// (The last entry goes in through an inline: its push's tag temporary comes after the loop's.)
static inline void DinoPushIcon(std::vector<DinoUiFossilEntry> &v, const DinoUiFossilEntry &e) {
    v.push_back(e);
}

// Lists the kaseki in the containers, then the last page's entry.
void DinoUiContainerDropMenu::LoadIcons() {
    mIcons.clear();
    for (int i = 0; i < 0x40; i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk98(i, &entry);
        if (entry.id) {
            mIcons.push_back(DinoGetFossilEntry(&entry, mKasekiDefs));
        }
    }
    DinoPushIcon(mIcons, ((DinoUiFossilEntry(*)(const DinoBackupEntry *))(DinoKasekiButtonInfo(*)(const DinoBackupEntry *))
                          DinoGetKasekiButtonInfo)(mUnkE0));
    if (mIcons.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mIcons.size() - 1) / 8;
    }
}

BOOL DinoUiContainerDropMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ShowPage();
    return TRUE;
}

BOOL DinoUiContainerDropMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ShowPage();
    return TRUE;
}

extern const u32 data_0208a660, data_0208a664, data_0208a66c, data_0208a674;

// Shows the page: the last page has mUnkC0 and its own help texts.
void DinoUiContainerDropMenu::ShowPage() {
    if (mPage == mLastPage) {
        mUnkC0->Show();
        mHelp12C->SetText(gDinoResource->GetText(data_0208a664));
        mHelp130->SetText(gDinoResource->GetText(data_0208a660));
    } else {
        mUnkC0->Hide();
        mHelp12C->SetText(gDinoResource->GetText(data_0208a674));
        mHelp130->SetText(gDinoResource->GetText(data_0208a66c));
    }
    DeleteItems();
    mFraction->SetNumerator(mPage + 1);
    CreateItems();
    FocusNearestItem(GetCursor());
}

#pragma opt_strength_reduction off
void DinoUiContainerDropMenu::DeleteItems() {
    for (u32 i = 0; i < mItems.size(); i++) {
        RemoveLink(mItems[i], 2);
        RemoveLink(mItems[i], 3);
        RemoveLink(mItems[i], 1);
        RemoveItem(mItems[i]);
        delete mItems[i];
    }
}
#pragma opt_strength_reduction reset

#pragma auto_inline on

DinoUiContainerDropMenuPageIncTask::~DinoUiContainerDropMenuPageIncTask() {
}

DinoUiContainerDropMenuItem::~DinoUiContainerDropMenuItem() {
}

DinoUiContainerDropMenu::~DinoUiContainerDropMenu() {
}

DinoUiContainerDropBackButton::~DinoUiContainerDropBackButton() {
}

DinoUiContainerDropPageLButton::~DinoUiContainerDropPageLButton() {
}

DinoUiContainerDropPageRButton::~DinoUiContainerDropPageRButton() {
}

MRect DinoUiContainerDropMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerDropMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerDropMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerDropMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerDropMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerDropBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerDropBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiContainerDropPageRButton::OnDecide() {
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

void DinoUiContainerDropPageLButton::OnDecide() {
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

void DinoUiContainerDropMenuPageIncTask::Execute() {
    mOwner->PageRight();
    Exit();
}


void DinoUiContainerDropMenu::OnIdle() {
    mStarted = TRUE;
    if (mUnk128 == 1) {
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
    }
    if (mUnk128) {
        mUnk128 = 0;
    }
}

// Moves the cursor to the item, and shows its entry's name (the last page's entry is mUnkE0).
void DinoUiContainerDropMenu::OnFocus(MMenuItem *item) {
    mStarted = FALSE;
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
    for (int i = 0; i < mItems.size(); i++) {
        if (item == mItems[i]) {
            if (mPage == mLastPage) {
                mRockName->SetEntry(mUnkE0, FALSE);
                return;
            }
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98(mPage * 8 + i, &entry);
            mRockName->SetEntry(&entry, FALSE);
            return;
        }
    }
    mRockName->Clear();
    mUnk120 = -1;
}

// The tag the drop message names the rock by.
extern "C" const char data_ov005_0216b178[];
extern const u32 data_0208a658, data_0208a65c;

// The back button leaves; a rock's button asks whether to drop it (naming it in mUnkE4[3]).
void DinoUiContainerDropMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < mItems.size(); i++) {
        if (item == mItems[i]) {
            if (!mStarted) {
                return;
            }
            *(u32 *)mUnkDC = i + mPage * 8;
            if (*(u32 *)mUnkDC == mIcons.size() - 1) {
                OnBackDecided();
                return;
            }
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98(*(u32 *)mUnkDC, &entry);
            std::string text = DinoReplaceTag(mUnkE4[3], data_ov005_0216b178, gDinoBackup->GetEntryName(&entry));
            DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(text, &mUnk128);
            dialog->SetLabels(mUnkE4[4], mUnkE4[5]);
            dialog->SetLabels2(std::string(gDinoResource->GetText(data_0208a65c)),
                               std::string(gDinoResource->GetText(data_0208a658)));
            mSerial.AddTail(dialog);
            return;
        }
    }
}

extern const u32 data_0208a650, data_0208a654;

// Asks whether to leave.
void DinoUiContainerDropMenu::OnBackDecided() {
    *(int *)mUnkDC = -1;
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(mUnkE4[0], &mUnk128);
    dialog->SetLabels(mUnkE4[1], mUnkE4[2]);
    dialog->SetLabels2(std::string(gDinoResource->GetText(data_0208a654)),
                       std::string(gDinoResource->GetText(data_0208a650)));
    mSerial.AddTail(dialog);
}



// Links the items as a grid four wide (ov005's copy; see dino_ui2_parts_kaseki_button.h).
void DinoUi2LinkKasekiGrid(MMenu *menu, std::vector<MMenuItem *> &items, MMenuItem *below);

void DinoUiContainerDropMenu::CreateItems() {
    mItems.clear();
    for (int i = 0; i < 8; i++) {
        if (mIcons.size() > i + mPage * 8) {
            MMenuItem *item = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02169388.x + data_ov005_02169388.columnX * (i % 4) + data_ov005_02169388.rowX * (i / 4),
                       data_ov005_02169388.y + data_ov005_02169388.columnY * (i % 4) + data_ov005_02169388.rowY * (i / 4)),
                (const DinoUiKasekiIcon &)mIcons[mPage * 8 + i], 0);
            AddItem(item);
            mItems.push_back(item);
        }
    }
    DinoUi2LinkKasekiGrid(this, mItems, mUnkB8);
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov005_02169508[];
extern "C" u32 data_ov005_02169208[];
extern "C" u32 data_ov005_02169188[];
extern "C" u32 data_ov005_02169308[];
extern "C" u32 data_ov005_02169288[];
extern "C" u32 data_ov005_02169588[];
extern "C" u32 data_ov005_02169608[];
extern "C" u32 data_ov005_02169688[];
extern "C" u32 data_ov005_02169488[];
static struct DinoUiContainerDropMenuPageIncTaskStaticInit {
    DinoUiContainerDropMenuPageIncTaskStaticInit() {
        DINO_POOL_WORD(data_ov005_02169508, 0x10) = ((s32 *) &data_ov005_02169388)[0x8 / 4];
        DINO_POOL_WORD(data_ov005_02169508, 0x14) = ((s32 *) &data_ov005_02169388)[0x8 / 4];
        DINO_POOL_WORD(data_ov005_02169508, 0x18) = DINO_POOL_WORD(data_ov005_02169208, 0x20);
        DINO_POOL_WORD(data_ov005_02169508, 0x1c) = DINO_POOL_WORD(data_ov005_02169188, 0x2c);
        DINO_POOL_WORD(data_ov005_02169508, 0x20) = DINO_POOL_WORD(data_ov005_02169308, 0x58);
        DINO_POOL_WORD(data_ov005_02169508, 0x24) = DINO_POOL_WORD(data_ov005_02169288, 0x4c);
        DINO_POOL_WORD(data_ov005_02169508, 0x28) = DINO_POOL_WORD(data_ov005_02169188, 0x44);
        DINO_POOL_WORD(data_ov005_02169508, 0x2c) = DINO_POOL_WORD(data_ov005_02169288, 0x6c);
        DINO_POOL_WORD(data_ov005_02169508, 0x30) = DINO_POOL_WORD(data_ov005_02169288, 0x54);
        DINO_POOL_WORD(data_ov005_02169508, 0x34) = DINO_POOL_WORD(data_ov005_02169188, 0x8);
        DINO_POOL_WORD(data_ov005_02169508, 0x38) = DINO_POOL_WORD(data_ov005_02169308, 0x8);
        DINO_POOL_WORD(data_ov005_02169508, 0x3c) = DINO_POOL_WORD(data_ov005_02169308, 0x1c);
        DINO_POOL_WORD(data_ov005_02169508, 0x40) = DINO_POOL_WORD(data_ov005_02169308, 0x2c);
        DINO_POOL_WORD(data_ov005_02169508, 0x44) = DINO_POOL_WORD(data_ov005_02169208, 0x5c);
        DINO_POOL_WORD(data_ov005_02169508, 0x48) = DINO_POOL_WORD(data_ov005_02169288, 0x24);
        DINO_POOL_WORD(data_ov005_02169508, 0x4c) = DINO_POOL_WORD(data_ov005_02169308, 0x38);
        DINO_POOL_WORD(data_ov005_02169508, 0x50) = DINO_POOL_WORD(data_ov005_02169388, 0x0);
        DINO_POOL_WORD(data_ov005_02169508, 0x54) = DINO_POOL_WORD(data_ov005_02169308, 0x7c);
        DINO_POOL_WORD(data_ov005_02169508, 0x58) = DINO_POOL_WORD(data_ov005_02169288, 0x4);
        DINO_POOL_WORD(data_ov005_02169508, 0x5c) = DINO_POOL_WORD(data_ov005_02169288, 0xc);
        DINO_POOL_WORD(data_ov005_02169508, 0x60) = DINO_POOL_WORD(data_ov005_02169388, 0xc);
        DINO_POOL_WORD(data_ov005_02169508, 0x64) = DINO_POOL_WORD(data_ov005_02169288, 0x0);
        DINO_POOL_WORD(data_ov005_02169508, 0x68) = DINO_POOL_WORD(data_ov005_02169388, 0x18);
        DINO_POOL_WORD(data_ov005_02169508, 0x6c) = DINO_POOL_WORD(data_ov005_02169388, 0x1c);
        DINO_POOL_WORD(data_ov005_02169508, 0x70) = DINO_POOL_WORD(data_ov005_02169388, 0x28);
        DINO_POOL_WORD(data_ov005_02169508, 0x74) = DINO_POOL_WORD(data_ov005_02169308, 0x74);
        DINO_POOL_WORD(data_ov005_02169508, 0x78) = DINO_POOL_WORD(data_ov005_02169288, 0x10);
        DINO_POOL_WORD(data_ov005_02169508, 0x7c) = DINO_POOL_WORD(data_ov005_02169188, 0x38);
        DINO_POOL_WORD(data_ov005_02169588, 0x0) = DINO_POOL_WORD(data_ov005_02169188, 0x20);
        DINO_POOL_WORD(data_ov005_02169588, 0x4) = DINO_POOL_WORD(data_ov005_02169288, 0x18);
        DINO_POOL_WORD(data_ov005_02169588, 0x8) = DINO_POOL_WORD(data_ov005_02169288, 0x1c);
        DINO_POOL_WORD(data_ov005_02169588, 0xc) = DINO_POOL_WORD(data_ov005_02169188, 0x48);
        DINO_POOL_WORD(data_ov005_02169588, 0x10) = DINO_POOL_WORD(data_ov005_02169288, 0x14);
        DINO_POOL_WORD(data_ov005_02169588, 0x14) = DINO_POOL_WORD(data_ov005_02169188, 0x14);
        DINO_POOL_WORD(data_ov005_02169588, 0x18) = DINO_POOL_WORD(data_ov005_02169188, 0x40);
        DINO_POOL_WORD(data_ov005_02169588, 0x1c) = DINO_POOL_WORD(data_ov005_02169288, 0x2c);
        DINO_POOL_WORD(data_ov005_02169588, 0x20) = DINO_POOL_WORD(data_ov005_02169188, 0x30);
        DINO_POOL_WORD(data_ov005_02169588, 0x24) = DINO_POOL_WORD(data_ov005_02169288, 0x30);
        DINO_POOL_WORD(data_ov005_02169588, 0x28) = DINO_POOL_WORD(data_ov005_02169208, 0x8);
        DINO_POOL_WORD(data_ov005_02169588, 0x2c) = DINO_POOL_WORD(data_ov005_02169188, 0x3c);
        DINO_POOL_WORD(data_ov005_02169588, 0x30) = DINO_POOL_WORD(data_ov005_02169188, 0x24);
        DINO_POOL_WORD(data_ov005_02169588, 0x34) = DINO_POOL_WORD(data_ov005_02169288, 0x44);
        DINO_POOL_WORD(data_ov005_02169588, 0x38) = DINO_POOL_WORD(data_ov005_02169288, 0x50);
        DINO_POOL_WORD(data_ov005_02169588, 0x3c) = DINO_POOL_WORD(data_ov005_02169288, 0x34);
        DINO_POOL_WORD(data_ov005_02169588, 0x40) = DINO_POOL_WORD(data_ov005_02169288, 0x60);
        DINO_POOL_WORD(data_ov005_02169588, 0x44) = DINO_POOL_WORD(data_ov005_02169288, 0x58);
        DINO_POOL_WORD(data_ov005_02169588, 0x48) = DINO_POOL_WORD(data_ov005_02169208, 0x68);
        DINO_POOL_WORD(data_ov005_02169588, 0x4c) = DINO_POOL_WORD(data_ov005_02169208, 0x2c);
        DINO_POOL_WORD(data_ov005_02169588, 0x50) = DINO_POOL_WORD(data_ov005_02169288, 0x38);
        DINO_POOL_WORD(data_ov005_02169588, 0x54) = DINO_POOL_WORD(data_ov005_02169188, 0x34);
        DINO_POOL_WORD(data_ov005_02169588, 0x58) = DINO_POOL_WORD(data_ov005_02169308, 0x4);
        DINO_POOL_WORD(data_ov005_02169588, 0x5c) = DINO_POOL_WORD(data_ov005_02169308, 0x10);
        DINO_POOL_WORD(data_ov005_02169588, 0x60) = DINO_POOL_WORD(data_ov005_02169208, 0x3c);
        DINO_POOL_WORD(data_ov005_02169588, 0x64) = DINO_POOL_WORD(data_ov005_02169308, 0x14);
        DINO_POOL_WORD(data_ov005_02169588, 0x68) = DINO_POOL_WORD(data_ov005_02169208, 0x48);
        DINO_POOL_WORD(data_ov005_02169588, 0x6c) = DINO_POOL_WORD(data_ov005_02169308, 0x28);
        DINO_POOL_WORD(data_ov005_02169588, 0x70) = DINO_POOL_WORD(data_ov005_02169308, 0x30);
        DINO_POOL_WORD(data_ov005_02169588, 0x74) = DINO_POOL_WORD(data_ov005_02169208, 0x50);
        DINO_POOL_WORD(data_ov005_02169588, 0x78) = DINO_POOL_WORD(data_ov005_02169188, 0x5c);
        DINO_POOL_WORD(data_ov005_02169588, 0x7c) = DINO_POOL_WORD(data_ov005_02169308, 0x3c);
        DINO_POOL_WORD(data_ov005_02169608, 0x0) = DINO_POOL_WORD(data_ov005_02169308, 0x48);
        DINO_POOL_WORD(data_ov005_02169608, 0x4) = DINO_POOL_WORD(data_ov005_02169308, 0x50);
        DINO_POOL_WORD(data_ov005_02169608, 0x8) = DINO_POOL_WORD(data_ov005_02169208, 0x60);
        DINO_POOL_WORD(data_ov005_02169608, 0xc) = DINO_POOL_WORD(data_ov005_02169208, 0x64);
        DINO_POOL_WORD(data_ov005_02169608, 0x10) = DINO_POOL_WORD(data_ov005_02169288, 0x74);
        DINO_POOL_WORD(data_ov005_02169608, 0x14) = DINO_POOL_WORD(data_ov005_02169308, 0x68);
        DINO_POOL_WORD(data_ov005_02169608, 0x18) = DINO_POOL_WORD(data_ov005_02169308, 0x70);
        DINO_POOL_WORD(data_ov005_02169608, 0x1c) = DINO_POOL_WORD(data_ov005_02169208, 0x70);
        DINO_POOL_WORD(data_ov005_02169608, 0x20) = DINO_POOL_WORD(data_ov005_02169188, 0x70);
        DINO_POOL_WORD(data_ov005_02169608, 0x24) = DINO_POOL_WORD(data_ov005_02169288, 0x40);
        DINO_POOL_WORD(data_ov005_02169608, 0x28) = DINO_POOL_WORD(data_ov005_02169288, 0x5c);
        DINO_POOL_WORD(data_ov005_02169608, 0x2c) = DINO_POOL_WORD(data_ov005_02169288, 0x78);
        DINO_POOL_WORD(data_ov005_02169608, 0x30) = DINO_POOL_WORD(data_ov005_02169208, 0x14);
        DINO_POOL_WORD(data_ov005_02169608, 0x34) = DINO_POOL_WORD(data_ov005_02169208, 0x34);
        DINO_POOL_WORD(data_ov005_02169608, 0x38) = DINO_POOL_WORD(data_ov005_02169208, 0xc);
        DINO_POOL_WORD(data_ov005_02169608, 0x3c) = DINO_POOL_WORD(data_ov005_02169188, 0x78);
        DINO_POOL_WORD(data_ov005_02169608, 0x40) = DINO_POOL_WORD(data_ov005_02169188, 0x54);
        DINO_POOL_WORD(data_ov005_02169608, 0x44) = DINO_POOL_WORD(data_ov005_02169188, 0x28);
        DINO_POOL_WORD(data_ov005_02169608, 0x48) = DINO_POOL_WORD(data_ov005_02169208, 0x74);
        DINO_POOL_WORD(data_ov005_02169608, 0x4c) = DINO_POOL_WORD(data_ov005_02169208, 0x28);
        DINO_POOL_WORD(data_ov005_02169608, 0x50) = DINO_POOL_WORD(data_ov005_02169208, 0x4);
        DINO_POOL_WORD(data_ov005_02169608, 0x54) = DINO_POOL_WORD(data_ov005_02169388, 0x20);
        DINO_POOL_WORD(data_ov005_02169608, 0x58) = DINO_POOL_WORD(data_ov005_02169188, 0x50);
        DINO_POOL_WORD(data_ov005_02169608, 0x5c) = DINO_POOL_WORD(data_ov005_02169208, 0x18);
        DINO_POOL_WORD(data_ov005_02169608, 0x60) = DINO_POOL_WORD(data_ov005_02169288, 0x68);
        DINO_POOL_WORD(data_ov005_02169608, 0x64) = DINO_POOL_WORD(data_ov005_02169308, 0xc);
        DINO_POOL_WORD(data_ov005_02169608, 0x68) = DINO_POOL_WORD(data_ov005_02169308, 0x18);
        DINO_POOL_WORD(data_ov005_02169608, 0x6c) = DINO_POOL_WORD(data_ov005_02169288, 0x70);
        DINO_POOL_WORD(data_ov005_02169608, 0x70) = DINO_POOL_WORD(data_ov005_02169308, 0x0);
        DINO_POOL_WORD(data_ov005_02169608, 0x74) = DINO_POOL_WORD(data_ov005_02169308, 0x78);
        DINO_POOL_WORD(data_ov005_02169608, 0x78) = DINO_POOL_WORD(data_ov005_02169388, 0x10);
        DINO_POOL_WORD(data_ov005_02169608, 0x7c) = DINO_POOL_WORD(data_ov005_02169208, 0x7c);
        DINO_POOL_WORD(data_ov005_02169688, 0x0) = DINO_POOL_WORD(data_ov005_02169288, 0x8);
        DINO_POOL_WORD(data_ov005_02169688, 0x4) = DINO_POOL_WORD(data_ov005_02169188, 0x4);
        DINO_POOL_WORD(data_ov005_02169688, 0x8) = DINO_POOL_WORD(data_ov005_02169308, 0x5c);
        DINO_POOL_WORD(data_ov005_02169688, 0xc) = DINO_POOL_WORD(data_ov005_02169188, 0x1c);
        DINO_POOL_WORD(data_ov005_02169688, 0x10) = DINO_POOL_WORD(data_ov005_02169288, 0x20);
        DINO_POOL_WORD(data_ov005_02169688, 0x14) = DINO_POOL_WORD(data_ov005_02169208, 0x10);
        DINO_POOL_WORD(data_ov005_02169688, 0x18) = DINO_POOL_WORD(data_ov005_02169188, 0x64);
        DINO_POOL_WORD(data_ov005_02169688, 0x1c) = DINO_POOL_WORD(data_ov005_02169288, 0x28);
        DINO_POOL_WORD(data_ov005_02169688, 0x20) = DINO_POOL_WORD(data_ov005_02169288, 0x3c);
        DINO_POOL_WORD(data_ov005_02169688, 0x24) = DINO_POOL_WORD(data_ov005_02169188, 0x58);
        DINO_POOL_WORD(data_ov005_02169688, 0x28) = DINO_POOL_WORD(data_ov005_02169188, 0x68);
        DINO_POOL_WORD(data_ov005_02169688, 0x2c) = DINO_POOL_WORD(data_ov005_02169288, 0x64);
        DINO_POOL_WORD(data_ov005_02169688, 0x30) = DINO_POOL_WORD(data_ov005_02169208, 0x30);
        DINO_POOL_WORD(data_ov005_02169688, 0x34) = DINO_POOL_WORD(data_ov005_02169288, 0x7c);
        DINO_POOL_WORD(data_ov005_02169688, 0x38) = DINO_POOL_WORD(data_ov005_02169208, 0x40);
        DINO_POOL_WORD(data_ov005_02169488, 0x18) = ((s32 *) &data_ov005_02169308)[0x20 / 4];
        DINO_POOL_WORD(data_ov005_02169488, 0x1c) = ((s32 *) &data_ov005_02169308)[0x20 / 4];
        DINO_POOL_WORD(data_ov005_02169488, 0x20) = DINO_POOL_WORD(data_ov005_02169208, 0x4c);
        DINO_POOL_WORD(data_ov005_02169488, 0x24) = DINO_POOL_WORD(data_ov005_02169188, 0x60);
        DINO_POOL_WORD(data_ov005_02169488, 0x28) = DINO_POOL_WORD(data_ov005_02169208, 0x58);
        DINO_POOL_WORD(data_ov005_02169488, 0x2c) = DINO_POOL_WORD(data_ov005_02169308, 0x4c);
        DINO_POOL_WORD(data_ov005_02169488, 0x30) = DINO_POOL_WORD(data_ov005_02169308, 0x60);
        DINO_POOL_WORD(data_ov005_02169488, 0x34) = DINO_POOL_WORD(data_ov005_02169208, 0x6c);
        DINO_POOL_WORD(data_ov005_02169488, 0x38) = DINO_POOL_WORD(data_ov005_02169188, 0x6c);
        DINO_POOL_WORD(data_ov005_02169488, 0x3c) = DINO_POOL_WORD(data_ov005_02169208, 0x1c);
        DINO_POOL_WORD(data_ov005_02169488, 0x40) = DINO_POOL_WORD(data_ov005_02169208, 0x24);
        DINO_POOL_WORD(data_ov005_02169488, 0x44) = DINO_POOL_WORD(data_ov005_02169208, 0x0);
        DINO_POOL_WORD(data_ov005_02169488, 0x48) = DINO_POOL_WORD(data_ov005_02169208, 0x38);
        DINO_POOL_WORD(data_ov005_02169488, 0x4c) = DINO_POOL_WORD(data_ov005_02169188, 0x7c);
        DINO_POOL_WORD(data_ov005_02169488, 0x50) = DINO_POOL_WORD(data_ov005_02169208, 0x54);
        DINO_POOL_WORD(data_ov005_02169488, 0x54) = DINO_POOL_WORD(data_ov005_02169188, 0x10);
        DINO_POOL_WORD(data_ov005_02169488, 0x58) = DINO_POOL_WORD(data_ov005_02169208, 0x78);
        DINO_POOL_WORD(data_ov005_02169488, 0x5c) = DINO_POOL_WORD(data_ov005_02169188, 0x4c);
        DINO_POOL_WORD(data_ov005_02169488, 0x60) = DINO_POOL_WORD(data_ov005_02169188, 0xc);
        DINO_POOL_WORD(data_ov005_02169488, 0x64) = DINO_POOL_WORD(data_ov005_02169388, 0x24);
        DINO_POOL_WORD(data_ov005_02169488, 0x68) = DINO_POOL_WORD(data_ov005_02169388, 0x14);
        DINO_POOL_WORD(data_ov005_02169488, 0x6c) = DINO_POOL_WORD(data_ov005_02169388, 0x4);
        DINO_POOL_WORD(data_ov005_02169488, 0x70) = DINO_POOL_WORD(data_ov005_02169188, 0x74);
        DINO_POOL_WORD(data_ov005_02169488, 0x74) = DINO_POOL_WORD(data_ov005_02169308, 0x64);
        DINO_POOL_WORD(data_ov005_02169488, 0x78) = DINO_POOL_WORD(data_ov005_02169308, 0x54);
        DINO_POOL_WORD(data_ov005_02169488, 0x7c) = DINO_POOL_WORD(data_ov005_02169308, 0x44);
        DINO_POOL_WORD(data_ov005_02169508, 0x0) = DINO_POOL_WORD(data_ov005_02169308, 0x34);
        DINO_POOL_WORD(data_ov005_02169508, 0x4) = DINO_POOL_WORD(data_ov005_02169308, 0x24);
        DINO_POOL_WORD(data_ov005_02169508, 0x8) = DINO_POOL_WORD(data_ov005_02169208, 0x44);
        DINO_POOL_WORD(data_ov005_02169508, 0xc) = DINO_POOL_WORD(data_ov005_02169308, 0x40);
        DINO_POOL_WORD(data_ov005_02169388, 0x34) = 128;
        DINO_POOL_WORD(data_ov005_02169388, 0x38) = 96;
        DINO_POOL_WORD(data_ov005_02169388, 0x3c) = 128;
        DINO_POOL_WORD(data_ov005_02169388, 0x40) = 96;
        DINO_POOL_WORD(data_ov005_02169388, 0x44) = 128;
        DINO_POOL_WORD(data_ov005_02169388, 0x48) = 96;
        DINO_POOL_WORD(data_ov005_02169388, 0x4c) = 53;
        DINO_POOL_WORD(data_ov005_02169388, 0x50) = 59;
        DINO_POOL_WORD(data_ov005_02169388, 0x54) = 50;
        DINO_POOL_WORD(data_ov005_02169388, 0x58) = 0;
        DINO_POOL_WORD(data_ov005_02169388, 0x5c) = 0;
        DINO_POOL_WORD(data_ov005_02169388, 0x60) = 50;
        DINO_POOL_WORD(data_ov005_02169388, 0x64) = 241;
        DINO_POOL_WORD(data_ov005_02169388, 0x68) = 128;
        DINO_POOL_WORD(data_ov005_02169388, 0x6c) = 26;
        DINO_POOL_WORD(data_ov005_02169388, 0x70) = 149;
    }
} sDinoUiContainerDropMenuPageIncTaskStaticInit;

// (Off at the end of the file, where template members are instantiated: the game calls them out of line.)
#pragma auto_inline off
