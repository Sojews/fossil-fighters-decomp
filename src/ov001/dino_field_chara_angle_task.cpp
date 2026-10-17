// DinoFieldCharaAngleTask, DinoFieldCharaMoveToRegionTask, DinoFieldCharaMoveToCoordTask, DinoFieldCharaMoveOffsetTask, DinoFieldCharaMoveVectorTask, DinoFieldCharaWaitMoveTask, DinoFieldCharaFocusTask.
// Only the destructors are decompiled so far.

#include "game/dino_sound.h"
#include "game/dino_field_point.h"
#include "game/dino_field.h"
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

void DinoFieldCharaFocusTask::Transfer() {
    mField->GetCamera()->SetFocus(mChara);
}

void DinoFieldCharaAngleTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        chara->SetAngle(mAngle, mA, mB);
    }
}

void DinoFieldCharaWaitMoveTask::Idle() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (!chara) {
        Exit();
    } else {
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *wait = chara->CreateWaitMoveTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(exit);
    }
}

// NONMATCHING: stack slots of the point temporaries only.
void DinoFieldCharaMoveOffsetTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        MFx32Point offset = mOffset;
        MFx32Point coord = chara->GetModel()->pos + offset;
        if (!mC) {
            chara->SetDirection(MFx32Atan2(offset.y, offset.x));
        }
        chara->MoveTo(coord, mA, mB, mC);
    }
}

// NONMATCHING: stack slots of the point temporaries only.
void DinoFieldCharaMoveToCoordTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        MFx32Point coord = mCoord;
        MFx32Point delta = coord - chara->GetModel()->pos;
        if (!mC) {
            chara->SetDirection(MFx32Atan2(delta.y, delta.x));
        }
        chara->MoveTo(coord, mA, mB, mC);
    }
}

void DinoField::FadeInLowerPict(u32 pict, MPoint pos, u32 frames, long unk) {
    mLowerPos = pos;
    FadeInPict(gLcd->GetBottomDisplay(), &mLowerPictId, &mLowerPict, pict, pos, frames, unk);
}

void DinoField::FadeOutLowerPict(u32 pict, long frames) {
    FadeOutPict(gLcd->GetBottomDisplay(), &mLowerPict, pict, frames);
}

void DinoField::SlideInLowerPict(u32 pict, MPoint pos, u32 a, u32 b, long c) {
    mLowerPos = pos;
    SlideInPict(gLcd->GetBottomDisplay(), &mLowerPictId, &mLowerPict, pict, pos, a, b, c);
}

void DinoField::SlideOutLowerPict(u32 pict, u32 a, long b) {
    SlideOutPict(gLcd->GetBottomDisplay(), &mLowerPict, pict, a, b);
}

void DinoField::ZoomInLowerPict(u32 pict, MPoint pos, MFx32 scale, u32 a, long b) {
    mLowerPos = pos;
    ZoomInPict(gLcd->GetBottomDisplay(), &mLowerPictId, &mLowerPict, pict, pos, scale, a, b);
}

void DinoField::ZoomOutLowerPict(MFx32 scale, u32 a, long b) {
    ZoomOutPict(gLcd->GetBottomDisplay(), &mLowerPict, scale, a, b);
}

void DinoField::FadeInUpperPict(u32 pict, MPoint pos, u32 frames, long unk) {
    mUpperPos = pos;
    FadeInPict(gLcd->GetTopDisplay(), &mUpperPictId, &mUpperPict, pict, pos, frames, unk);
}

void DinoField::FadeOutUpperPict(u32 pict, long frames) {
    FadeOutPict(gLcd->GetTopDisplay(), &mUpperPict, pict, frames);
}

void DinoField::SlideInUpperPict(u32 pict, MPoint pos, u32 a, u32 b, long c) {
    mUpperPos = pos;
    SlideInPict(gLcd->GetTopDisplay(), &mUpperPictId, &mUpperPict, pict, pos, a, b, c);
}

void DinoField::SlideOutUpperPict(u32 pict, u32 a, long b) {
    SlideOutPict(gLcd->GetTopDisplay(), &mUpperPict, pict, a, b);
}

void DinoField::ZoomInUpperPict(u32 pict, MPoint pos, MFx32 scale, u32 a, long b) {
    mUpperPos = pos;
    ZoomInPict(gLcd->GetTopDisplay(), &mUpperPictId, &mUpperPict, pict, pos, scale, a, b);
}

void DinoField::ZoomOutUpperPict(MFx32 scale, u32 a, long b) {
    ZoomOutPict(gLcd->GetTopDisplay(), &mUpperPict, scale, a, b);
}

MTask *DinoField::CreateMoneyShowTask() {
    if (mMoney) {
        return new MTimer(0);
    }
    mMoney = new DinoFieldMoney();
    mParallel.AddTail(mMoney);
    return mMoney->CreateShowTask();
}

MTask *DinoField::CreateMoneyHideTask() {
    if (!mMoney) {
        return new MTimer(0);
    }
    MTask *task = mMoney->CreateHideTask();
    mMoney = NULL;
    return task;
}

MTask *DinoField::CreateDonateShowTask() {
    if (mDonate) {
        return new MTimer(0);
    }
    mDonate = new DinoFieldDonate();
    mParallel.AddTail(mDonate);
    return mDonate->CreateShowTask();
}

MTask *DinoField::CreateDonateHideTask() {
    if (!mDonate) {
        return new MTimer(0);
    }
    MTask *task = mDonate->CreateHideTask();
    mDonate = NULL;
    return task;
}

MTask *DinoField::CreateFadeWaitTask() {
    return MTaskSetWaitEmptyTask::Create(mFadeTasks);
}

void DinoField::PlayBgm(u32 bgm, u32 arg) {
    if (mBgmChanging == 0) {
        mBgm = bgm;
        mBgmScene = DinoSoundGetScene();
        DinoSoundPlayBgm(bgm, arg);
    }
}

void DinoField::StopBgm(u32 fadeFrames) {
    mBgm = 0;
    mBgmChanging = FALSE;
    DinoSoundStopBgm(-1, fadeFrames);
}

void DinoField::PlayBgmFade(u32 bgm, u32 arg) {
    mBgm = bgm;
    mBgmScene = DinoSoundGetScene();
    mBgmChanging = TRUE;
    DinoSoundPlayBgm(bgm, arg);
}

u32 DinoField::GetBgm() {
    return mBgm;
}

BOOL DinoField::IsBgmChanging() {
    if (mBgmChanging) {
        return TRUE;
    }
    return FALSE;
}

void DinoField::Quake(u32 a, u32 b, u32 c) {
    mQuake = TRUE;
    mQuakeArgs[0] = a;
    mQuakeArgs[1] = b;
    mQuakeArgs[2] = c;
    mQuakeTime = 0;
}

MTask *DinoField::CreatePictWaitTask() {
    return MTaskSetWaitEmptyTask::Create(mPictTasks);
}
