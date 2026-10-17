// DinoMain, DinoMainLoop.
// Only the destructors are decompiled so far.

#include "game/dino_main.h"
#include "game/dino_main_loop.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoMain::~DinoMain() {
}

DinoMainLoop::~DinoMainLoop() {
}

void DinoMainLoop::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoMainLoop>;
template MTask *MCreateBgCallTask(DinoMainLoop *obj, void (DinoMainLoop::*func)(), int thread);

DinoMain::DinoMain() : MProcess(NULL) {
}
