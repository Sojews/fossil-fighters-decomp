// DinoFieldCharaAngleTask, DinoFieldCharaMoveToRegionTask, DinoFieldCharaMoveToCoordTask, DinoFieldCharaMoveOffsetTask, DinoFieldCharaMoveVectorTask, DinoFieldCharaWaitMoveTask, DinoFieldCharaFocusTask.
// Only the destructors are decompiled so far.

#include "game/dino_field_spot.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "game/dino_field_chara_angle_task.h"
#include "game/dino_field_chara_move_to_region_task.h"
#include "game/dino_field_chara_move_to_coord_task.h"
#include "game/dino_field_chara_move_offset_task.h"
#include "game/dino_field_chara_move_vector_task.h"
#include "game/dino_field_chara_wait_move_task.h"
#include "game/dino_field_chara_focus_task.h"
#include "game/dino_field_capture_fade_in_task.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/polygon_char.h"

DinoFieldCharaAngleTask::~DinoFieldCharaAngleTask() {
}

DinoFieldCharaMoveToRegionTask::~DinoFieldCharaMoveToRegionTask() {
}

DinoFieldCharaMoveToCoordTask::~DinoFieldCharaMoveToCoordTask() {
}

DinoFieldCharaMoveOffsetTask::~DinoFieldCharaMoveOffsetTask() {
}

DinoFieldCharaMoveVectorTask::~DinoFieldCharaMoveVectorTask() {
}

DinoFieldCharaWaitMoveTask::~DinoFieldCharaWaitMoveTask() {
}

DinoFieldCharaFocusTask::~DinoFieldCharaFocusTask() {
}

DinoFieldSpotOut::~DinoFieldSpotOut() {
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
}

DinoFieldSpotIn::~DinoFieldSpotIn() {
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<MPolygonChar, MPolygonChar>;
template MTask *MCreateCallTask(MPolygonChar *obj, void (MPolygonChar::*func)());
template class MBgCallTask0<DinoFieldCaptureFadeInTask>;
template MTask *MCreateBgCallTask(DinoFieldCaptureFadeInTask *obj, void (DinoFieldCaptureFadeInTask::*func)(), int thread);
template class MCallTask1<MBg, MBg, bg_mode>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(bg_mode), bg_mode a0);
