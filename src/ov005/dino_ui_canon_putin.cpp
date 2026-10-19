// DinoUiCanonPutin, DinoUiCanonTakeout.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_ui_canon_putin_menu.h"
#include "game/dino_ui_canon_putin.h"
#include "game/dino_ui_canon_takeout.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_message_box.h"

DinoUiCanonPutin::~DinoUiCanonPutin() {
}

DinoUiCanonTakeout::~DinoUiCanonTakeout() {
}

void DinoUiCanonTakeout::Idle() {
}

void DinoUiCanonPutin::Idle() {
}

DinoUiCanonPutinMenu::~DinoUiCanonPutinMenu() {
}

DinoUiCanonTakeoutMenu::~DinoUiCanonTakeoutMenu() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiCanonTakeoutMenu, DinoUiCanonTakeoutMenu>;
template MTask *MCreateCallTask(DinoUiCanonTakeoutMenu *obj, void (DinoUiCanonTakeoutMenu::*func)());
template class MCallTask0<DinoUiCanonPutinMenu, DinoUiCanonPutinMenu>;
template MTask *MCreateCallTask(DinoUiCanonPutinMenu *obj, void (DinoUiCanonPutinMenu::*func)());

DinoUiCanonTakeout::DinoUiCanonTakeout() : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiCanonTakeoutMenu());
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

// "Put which ... in?" (a text id).
extern "C" const u32 data_0208a574;

DinoUiCanonPutinMenu::DinoUiCanonPutinMenu() {
    mUnk101 = 0x40;
    mUnk104 = data_0208a574;
}

DinoUiCanonPutin::DinoUiCanonPutin() : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiCanonPutinMenu());
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

extern const u32 data_0208a98c; // text shown when nothing can be taken out
extern const u32 data_0208a990; // text shown when nothing can be put in

void DinoUiCanonPutinMenu::GetEntry(u32 index, DinoBackupEntry *entry) {
    gDinoBackup->GetUnk98(index, entry);
}

BOOL DinoUiCanonPutinMenu::CanMark() {
    if (gDinoBackup->CountFreeUnk14C() > mCount) {
        return TRUE;
    }
    return FALSE;
}

BOOL DinoUiCanonPutinMenu::IsMarkable(DinoBackupEntry *entry) {
    if (entry->unk10_0 == 1) {
        return FALSE;
    }
    DinoKasekiRockState rock;
    gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
    if (!rock.unk10 && !entry->unk10_3) {
        return FALSE;
    }
    return TRUE;
}

MTask *DinoUiCanonPutinMenu::CreateUnk2C() {
    return new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208a990), 4);
}

void DinoUiCanonTakeoutMenu::GetEntry(u32 index, DinoBackupEntry *entry) {
    gDinoBackup->GetUnk14C(index, entry);
}

BOOL DinoUiCanonTakeoutMenu::CanMark() {
    if (gDinoBackup->CountFreeUnk98() > mCount) {
        return TRUE;
    }
    return FALSE;
}

BOOL DinoUiCanonTakeoutMenu::IsMarkable(DinoBackupEntry *entry) {
    return TRUE;
}

MTask *DinoUiCanonTakeoutMenu::CreateUnk2C() {
    return new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208a98c), 4);
}
