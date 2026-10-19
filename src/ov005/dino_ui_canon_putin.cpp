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

extern "C" const u32 data_0208a560;

DinoUiCanonTakeoutMenu::DinoUiCanonTakeoutMenu() {
    mUnk101 = 8;
    mUnk104 = data_0208a560;
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

void DinoUiCanonPutinMenu::PutIn() {
    if (mUnk158 == 1) {
        for (int i = 0; i < 64; i++) {
            if (mFlags[i]) {
                mFlags[i] = 0;
                DinoBackupEntry entry;
                gDinoBackup->GetUnk98(i, &entry);
                if (!gDinoBackup->IsUnk14CFull()) {
                    gDinoBackup->SetUnk14C(gDinoBackup->FindFreeUnk14C(), &entry);
                    MI_CpuFill8(&entry, 0, sizeof(entry));
                    gDinoBackup->SetUnk98(i, &entry);
                }
            }
        }
        mCount = 0;
        gDinoBackup->CompactUnk98();
        if (mLastPage > 0) {
            if (gDinoBackup->CountUsedUnk98() <= 8) {
                HidePageButtons();
                mPage = 0;
                mFraction->SetNumerator(mPage + 1);
            } else {
                int last = (gDinoBackup->CountUsedUnk98() - 1) / 8;
                if (mPage > last) {
                    mPage = last;
                    mFraction->SetNumerator(mPage + 1);
                }
            }
        }
        RefreshEntries();
        ClearItems();
        RefreshPage2();
        ResetDecideButton();
    }
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


void DinoUiCanonTakeoutMenu::TakeOut() {
    if (mUnk158 == 1) {
        for (int i = 0; i < 8; i++) {
            if (mFlags[i]) {
                mFlags[i] = 0;
                DinoBackupEntry entry;
                gDinoBackup->GetUnk14C(i, &entry);
                if (!gDinoBackup->IsUnk98Full()) {
                    gDinoBackup->SetUnk98(gDinoBackup->FindFreeUnk98(), &entry);
                    MI_CpuFill8(&entry, 0, sizeof(entry));
                    gDinoBackup->SetUnk14C(i, &entry);
                }
            }
        }
        mCount = 0;
        gDinoBackup->CompactUnk14C();
        RefreshEntries();
        ClearItems();
        RefreshPage2();
        ResetDecideButton();
    }
}

MTask *DinoUiCanonTakeoutMenu::CreateUnk2C() {
    return new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208a98c), 4);
}
