// DinoUiContainerDropMenuPageIncTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

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

// Where the kaseki buttons go: two rows of four.
extern struct {
    u8 unk00[0x4C];
    int x;          // 0x4C: the first button
    int y;          // 0x50
    int columnX;    // 0x54: to the next in a row
    int columnY;    // 0x58
    int rowX;       // 0x5C: to the next row
    int rowY;       // 0x60
} data_ov005_02169388;

void DinoUiContainerDropMenu::CreateItems() {
    mItems.clear();
    for (int i = 0; i < 8; i++) {
        if (mIcons.size() > i + mPage * 8) {
            MMenuItem *item = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02169388.x + data_ov005_02169388.columnX * (i % 4) + data_ov005_02169388.rowX * (i / 4),
                       data_ov005_02169388.y + data_ov005_02169388.columnY * (i % 4) + data_ov005_02169388.rowY * (i / 4)),
                mIcons[mPage * 8 + i], 0);
            AddItem(item);
            mItems.push_back(item);
        }
    }
    DinoUi2LinkKasekiGrid(this, mItems, mUnkB8);
}
