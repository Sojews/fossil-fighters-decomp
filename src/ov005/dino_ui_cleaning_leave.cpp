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
