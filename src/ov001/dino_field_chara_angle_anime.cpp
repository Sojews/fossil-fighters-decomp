// DinoFieldCharaAngleAnime, DinoFieldCharaWaitStopTask, DinoFieldCharaAngleToTask, DinoFieldCharaMoveToTask, DinoFieldCharaShowManpuTask, DinoFieldCharaHideManpuTask, DinoFieldCharaModelMotionIdTask.
// Only the destructors are decompiled so far.

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
