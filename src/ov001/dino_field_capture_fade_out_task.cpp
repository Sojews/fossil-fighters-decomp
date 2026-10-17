// DinoFieldCaptureFadeOutTask, DinoFieldCaptureFadeInTask.
// Only the destructors are decompiled so far.

#include "game/dino_field_capture_fade_out_task.h"
#include "game/dino_field_capture_fade_in_task.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/polygon_char.h"

DinoFieldCaptureFadeOutTask::~DinoFieldCaptureFadeOutTask() {
}

DinoFieldCaptureFadeInTask::~DinoFieldCaptureFadeInTask() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<MPolygonChar, MPolygonChar>;
template MTask *MCreateCallTask(MPolygonChar *obj, void (MPolygonChar::*func)());
template class MBgCallTask0<DinoFieldCaptureFadeInTask>;
template MTask *MCreateBgCallTask(DinoFieldCaptureFadeInTask *obj, void (DinoFieldCaptureFadeInTask::*func)(), int thread);
template class MCallTask1<MBg, MBg, bg_mode>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(bg_mode), bg_mode a0);
