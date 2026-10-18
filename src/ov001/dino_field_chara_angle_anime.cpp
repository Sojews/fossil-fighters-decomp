// DinoFieldCharaAngleAnime, DinoFieldCharaWaitStopTask, DinoFieldCharaAngleToTask, DinoFieldCharaMoveToTask, DinoFieldCharaShowManpuTask, DinoFieldCharaHideManpuTask, DinoFieldCharaModelMotionIdTask.
// Only the destructors are decompiled so far.

#include "game/dino_field.h"
#include "game/dino_field_chara_angle_anime.h"
#include "game/dino_field_chara_coord_anime.h"
#include "game/dino_field_chara_wait_stop_task.h"
#include "game/dino_field_chara_angle_to_task.h"
#include "game/dino_field_chara_move_to_task.h"
#include "game/dino_field_chara_show_manpu_task.h"
#include "game/dino_field_chara_hide_manpu_task.h"
#include "game/dino_field_chara_model_motion_id_task.h"

DinoFieldCharaAngleAnime::~DinoFieldCharaAngleAnime() {
}

DinoFieldCharaWaitStopTask::~DinoFieldCharaWaitStopTask() {
}

void DinoFieldCharaAngleAnime::Apply(int step) {
    int last = mSteps - 1;
    mChara->SetDirection((mFrom * (last - step) + mTo * step) / last);
}

DinoFieldCharaAngleToTask::~DinoFieldCharaAngleToTask() {
}

DinoFieldCharaMoveToTask::~DinoFieldCharaMoveToTask() {
}

DinoFieldCharaShowManpuTask::~DinoFieldCharaShowManpuTask() {
}

DinoFieldCharaHideManpuTask::~DinoFieldCharaHideManpuTask() {
}

DinoFieldCharaModelMotionIdTask::~DinoFieldCharaModelMotionIdTask() {
}

DinoFieldCharaCoordAnime::~DinoFieldCharaCoordAnime() {
    if (!mUnk4C) {
        mChara->SetMotion(1, 1, MFx32(1));
    }
}

DinoFieldCharaModel *DinoFieldChara::GetModel() {
    return &mModel;
}

// The chara file's header.
struct DinoFieldCharaData {
    u8 unk00[0x18];
    MFx32 mHeight;  // 0x18
};

MFx32 DinoFieldChara::GetHeight() {
    if (IsUnkA1Motion()) {
        MFx32 zero;
        zero.v = 0;
        return zero;
    }
    return ((DinoFieldCharaData *) mFile.GetData())->mHeight;
}

BOOL DinoFieldChara::IsUnkA1Motion() {
    return mModel.unk05[0] && (mMotion == 2 || mMotion == 3);
}

inline DinoFieldCharaAngleAnime::DinoFieldCharaAngleAnime(DinoFieldChara *chara, u32 angle, u32 frames, int ease) {
    mChara = chara;
    mSteps = 256;
    SetDuration(frames);
    mEase = ease;
    u16 from = mChara->GetModel()->angle;
    u16 diff = angle - from;
    mFrom = from;
    if (diff < 0x8000) {
        mTo = from + diff;
    } else {
        mTo = from - (0x10000 - diff);
    }
}

void DinoFieldChara::SetAngle(u32 angle, u32 frames, u32 ease) {
    mAngleTasks->Clear();
    mAngleTasks->AddTail(new DinoFieldCharaAngleAnime(this, angle, frames, ease));
}

void DinoFieldChara::SetTask(DinoCharaTask task) {
    mTask = task;
    switch (task.unk4) {
    case 0: {
        DinoCharaState state;
        gDinoBackup->GetChara(mModel.id, &state);
        mHomeX = state.x;
        mHomeY = state.y;
        break;
    }
    case 2:
        mUnkC0 = 1;
        break;
    case 3:
        mUnkC0 = 1;
        mUnkC4 = 0;
        break;
    }
    mModel.unk10 = 0;
}

void DinoFieldChara::Read(const void *buf) {
    MSerializer::Read(buf);
    if (mModel.id) {
        gDinoBackup->GetCharaTask(mModel.id, &mTask);
    }
}

MTask *DinoFieldChara::CreateWaitMoveTask() {
    MTask *move;
    MTask *angle;
    angle = MTaskSetWaitEmptyTask::Create(mAngleTasks);
    move = MTaskSetWaitEmptyTask::Create(mMoveTasks);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(move);
    set->AddTail(angle);
    return set;
}

void DinoFieldChara::HideManpu() {
    if (mManpu) {
        mTaskSet->Remove(mManpu);
        delete mManpu;
        mManpu = NULL;
    }
}

void DinoFieldChara::UpdateBodyModel() {
    ApplyBodyModel();
    ApplyHeadModel();
}

void DinoFieldChara::UpdateHeadModel() {
    ApplyHeadModel();
}

void DinoFieldChara::SetMotion(u32 motion, u8 loop, MFx32 speed) {
    if (mMotion != motion) {
        ChangeMotion(motion, loop, speed);
    }
}

void DinoFieldChara::StartBehold(u32 target) {
    mBeholdTarget = target;
}

void DinoFieldChara::FinishBehold() {
    mBeholdTarget = 0;
}

void DinoFieldChara::SetFootSe(u32 se) {
    mHasFootSe = TRUE;
    mFootSe = se;
}

void DinoFieldChara::SetDirection(u16 angle) {
    if (mBeholdTarget == 0) {
        mModel.angle = angle;
        ApplyMotion();
    }
}

void DinoFieldCharaAngleToTask::Transfer() {
    mChara->SetAngle(mAngle, mA, mB);
}

void DinoFieldCharaShowManpuTask::Transfer() {
    mChara->ShowManpu(mManpu, mOffset);
}

void DinoFieldCharaHideManpuTask::Transfer() {
    mChara->HideManpu();
}

void DinoFieldCharaMoveToTask::Transfer() {
    mChara->MoveTo(mCoord, mA, mB, mC);
}

void DinoFieldCharaModelMotionIdTask::Transfer() {
    if (mForce) {
        mChara->ChangeMotion(mMotion, mLoop, mSpeed);
    } else {
        mChara->SetMotion(mMotion, mLoop, mSpeed);
    }
}

void DinoFieldCharaWaitStopTask::Execute() {
    if (!mChara->IsMoving()) {
        Exit();
    }
}

void DinoFieldCharaCoordAnime::Apply(int step) {
    MFx32PointAnime::Apply(step);
    if (!mUnk4C) {
        mChara->SetMotion(2, 1, mChara->mField->GetMap()->GetWalkSpeed());
    }
    fx32 x = mValue.x.v;
    fx32 y = mValue.y.v;
    MFx32Point *pos = &mChara->GetModel()->pos;
    pos->x.v = x;
    pos->y.v = y;
    mChara->ApplyMotion();
}

void DinoFieldCharaCoordAnime::Execute() {
    MFx32PointAnime::Execute();
    if (mUnk50 && !mUnk4C) {
        mChara->mUnk310 = mUnk50;
    }
}
