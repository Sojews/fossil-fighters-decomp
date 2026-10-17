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
#include "game/dino_field_lower_spot.h"

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

DinoFieldWhiteOut::~DinoFieldWhiteOut() {
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
}

DinoFieldWhiteIn::~DinoFieldWhiteIn() {
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
}

DinoFieldBlackOut::~DinoFieldBlackOut() {
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
}

DinoFieldBlackIn::~DinoFieldBlackIn() {
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
}

void DinoFieldWhiteOut::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToWhite(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldWhiteIn::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromWhite(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeInFromWhite(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldBlackOut::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldBlackIn::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldSpotOut::Idle() {
    MTask *top;
    MTask *lower;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    lower = new DinoFieldLowerSpotOut(mDuration);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(lower);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldSpotIn::Idle() {
    MTask *top;
    MTask *lower;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    lower = new DinoFieldLowerSpotIn(mDuration);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(lower);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<MPolygonChar, MPolygonChar>;
template MTask *MCreateCallTask(MPolygonChar *obj, void (MPolygonChar::*func)());
template class MBgCallTask0<DinoFieldCaptureFadeInTask>;
template MTask *MCreateBgCallTask(DinoFieldCaptureFadeInTask *obj, void (DinoFieldCaptureFadeInTask::*func)(), int thread);
template class MCallTask1<MBg, MBg, bg_mode>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(bg_mode), bg_mode a0);
