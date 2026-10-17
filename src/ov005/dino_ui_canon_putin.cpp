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
