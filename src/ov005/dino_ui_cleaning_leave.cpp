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

DinoUiCleaningLeave::~DinoUiCleaningLeave() {
}

void DinoUiCleaningLeave::Idle() {
}

DinoUiCleaningLeaveMenu::~DinoUiCleaningLeaveMenu() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiCleaningLeaveMenu, DinoUiCleaningLeaveMenu>;
template MTask *MCreateCallTask(DinoUiCleaningLeaveMenu *obj, void (DinoUiCleaningLeaveMenu::*func)());

DinoUiCleaningLeave::DinoUiCleaningLeave(u32 arg) : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiCleaningLeaveMenu(arg));
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

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
    const u8 *data = (const u8 *)mFile.GetData();
    const u8 *def = data + ((const u32 *)(data + *(const u32 *)(data + 8)))[id];
    if (def[4] == 0) {
        return TRUE;
    }
    return FALSE;
}

MTask *DinoUiCleaningLeaveMenu::CreateUnk2C() {
    return new MTimer(1);
}
