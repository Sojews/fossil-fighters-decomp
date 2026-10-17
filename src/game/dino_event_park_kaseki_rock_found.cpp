// DinoEventParkKasekiRockFound.
// Only the destructors are decompiled so far.

#include "game/dino_event_park_kaseki_rock_found.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/polygon_char.h"

DinoEventParkKasekiRockFound::~DinoEventParkKasekiRockFound() {
}

void DinoEventParkKasekiRockFound::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoFieldHole {};
class DinoEventLoop {};
template class MCallTask0<DinoFieldHole, DinoFieldHole>;
template MTask *MCreateCallTask(DinoFieldHole *obj, void (DinoFieldHole::*func)());
template class MCallTask1<DinoEventLoop, DinoEventLoop, bool>;
template MTask *MCreateCallTask(DinoEventLoop *obj, void (DinoEventLoop::*func)(bool), bool a0);
template class MCallTask2<MPolygonChar, MPolygonChar, const char *, const char *>;
template MTask *MCreateCallTask(MPolygonChar *obj, void (MPolygonChar::*func)(const char *, const char *), const char * a0, const char * a1);
