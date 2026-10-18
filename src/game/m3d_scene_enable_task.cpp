// M3DSceneEnableTask.
// Only the destructors are decompiled so far.

#include "m/3d_scene_enable_task.h"
#include "m/3d_scene.h"

M3DSceneEnableTask::~M3DSceneEnableTask() {
}

void M3DSceneEnableTask::Execute() {
    mScene->mEnabled = mEnable ? TRUE : FALSE;
    Exit();
}
