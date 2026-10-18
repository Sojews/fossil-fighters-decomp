// M3DSceneEnableTask.
// Also some of M3DModel's and M3DRyusen's virtual functions.

#include "m/3d_scene_enable_task.h"
#include "m/3d_scene.h"
#include "m/3d_ryusen.h"

M3DSceneEnableTask::~M3DSceneEnableTask() {
}

void M3DSceneEnableTask::Execute() {
    mScene->mEnabled = mEnable ? TRUE : FALSE;
    Exit();
}

void M3DModel::Unk08(void *model, void *anim, BOOL arg) {
    SetModelData(model, arg);
    SetAnimData(anim);
}

BOOL M3DRyusen::IsAnimDone() {
    return FALSE;
}

int M3DRyusen::GetFrameCount() {
    return 0xFFFF;
}

BOOL M3DModel::IsAnimDone() {
    BOOL done = TRUE;
    if (mFlags & 1) {
        if (((const u32 *)mModelRes)[8] - 1 > mFrame >> 4) {
            done = FALSE;
        }
    } else {
        done = FALSE;
    }
    return done;
}

int M3DModel::GetFrameCount() {
    if (mFlags & 1) {
        return ((const u32 *)mModelRes)[8];
    }
    return 0;
}
