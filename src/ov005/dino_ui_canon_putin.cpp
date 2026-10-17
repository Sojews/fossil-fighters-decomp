// DinoUiCanonPutin, DinoUiCanonTakeout.
// Only the destructors are decompiled so far.

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

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiCanonTakeoutMenu {};
class DinoUiCanonPutinMenu {};
template class MCallTask0<DinoUiCanonTakeoutMenu, DinoUiCanonTakeoutMenu>;
template MTask *MCreateCallTask(DinoUiCanonTakeoutMenu *obj, void (DinoUiCanonTakeoutMenu::*func)());
template class MCallTask0<DinoUiCanonPutinMenu, DinoUiCanonPutinMenu>;
template MTask *MCreateCallTask(DinoUiCanonPutinMenu *obj, void (DinoUiCanonPutinMenu::*func)());
