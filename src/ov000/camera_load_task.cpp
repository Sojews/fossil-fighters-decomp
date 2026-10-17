// CameraLoadTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/camera_load_task.h"
#include "types.h"
#include "game/dino_battle_view_3d_camera.h"

CameraLoadTask::~CameraLoadTask() {
}

NextCameraTask::~NextCameraTask() {
}

ChangeSetTask::~ChangeSetTask() {
}

CameraPauseTask::~CameraPauseTask() {
}

CameraSetDefParentTask::~CameraSetDefParentTask() {
}

void DinoBattleView3DCamera::Idle() {
    (this->*mState)();
}
