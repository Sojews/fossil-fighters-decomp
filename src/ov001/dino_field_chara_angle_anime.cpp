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
