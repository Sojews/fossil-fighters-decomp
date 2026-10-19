// DinoUiCleaningLeave.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_ui_cleaning_leave_menu.h"
#include "game/dino_ui_cleaning_leave.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_ui3_parts_cursor.h"
#include "game/dino_sound.h"

DinoUiCleaningLeave::~DinoUiCleaningLeave() {
}

void DinoUiCleaningLeave::Idle() {
}

DinoUiCleaningLeaveMenu::~DinoUiCleaningLeaveMenu() {
}

DinoUiCleaningLeave::DinoUiCleaningLeave(std::vector<void *> *chosen) : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiCleaningLeaveMenu(chosen));
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

template MTask *MCreateCallTask(DinoUiCleaningLeaveMenu *obj, void (DinoUiCleaningLeaveMenu::*func)());

extern const u32 data_0208a628; // text: the back button's help

DinoUiCleaningLeaveMenu::DinoUiCleaningLeaveMenu(std::vector<void *> *chosen) {
    mChosen = chosen;
    mChosen->clear();
    mUnk101 = 0x40;
    mUnk104 = data_0208a628;
}

extern const u32 data_0208a9a4, data_0208a9a0, data_0208a99c, data_0208a998, data_0208a994;

// Asks whether to leave the marked fossils.
BOOL DinoUiCleaningLeaveMenu::TryDecide() {
    if (mCount == 0) {
        DinoSoundPlaySe(0xA4, 0);
        return FALSE;
    }
    mCursor->Hide();
    mConfirmed = FALSE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208a9a4), &mConfirmed);
    dialog->SetLabels(gDinoResource->GetText(data_0208a9a0), gDinoResource->GetText(data_0208a99c));
    dialog->SetLabels2(gDinoResource->GetText(data_0208a998), gDinoResource->GetText(data_0208a994));
    dialog->SetUnkBC(1);
    MTask *finish = MCreateCallTask(this, &DinoUiCleaningLeaveMenu::FinishDecide);
    MTask *show = mCursor->CreateShowTransfer();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(dialog);
    serial->AddTail(show);
    serial->AddTail(finish);
    return TRUE;
}

// After the yes/no: hands back the marked entries and leaves.
void DinoUiCleaningLeaveMenu::FinishDecide() {
    if (mConfirmed == TRUE) {
        for (int i = 0; i < 0x40; i++) {
            if (mFlags[i]) {
                mFlags[i] = FALSE;
                mChosen->push_back((void *)i);
            }
        }
        mCount = 0;
        ResetDecideButton();
        OnBackDecided();
    }
}

void DinoUiCleaningLeaveMenu::GetEntry(u32 index, DinoBackupEntry *entry) {
    gDinoBackup->GetUnk98(index, entry);
}

BOOL DinoUiCleaningLeaveMenu::CanMark() {
    return TRUE;
}

BOOL DinoUiCleaningLeaveMenu::IsMarkable(DinoBackupEntry *entry) {
    if (entry->active) {
        return FALSE;
    }
    if (entry->unk10_0 == 1) {
        return FALSE;
    }
    if (entry->type == 1) {
        return TRUE;
    }
    DinoKasekiRockState rock;
    gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
    if (!rock.unk10) {
        return FALSE;
    }
    u32 id = entry->id;
    const u8 *data = (const u8 *)mDefs.Get();
    const u8 *def = data + ((const u32 *)(data + *(const u32 *)(data + 8)))[id];
    if (def[4] == 0) {
        return TRUE;
    }
    return FALSE;
}

MTask *DinoUiCleaningLeaveMenu::CreateUnk2C() {
    return new MTimer(1);
}
