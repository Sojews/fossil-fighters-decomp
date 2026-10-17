// CameraLoadTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/camera_load_task.h"
#include "types.h"
#include "game/dino_battle_view_3d_camera.h"

extern "C" void func_ov000_0218a970(void *, u32);

extern "C" void func_ov000_0218a8ac(void *, u32, u32, u32);

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

void ChangeSetTask::Execute() {
    func_ov000_0218a8ac((void *)mUnk10, mUnk14, mUnk1C, mUnk18);
    Exit();
}

void NextCameraTask::Execute() {
    func_ov000_0218a970((void *)mUnk10, mUnk14);
    Exit();
}

extern "C" void func_ov000_0218a998(u32 arg0, char *name, u32 arg2);

void CameraLoadTask::Execute() {
    func_ov000_0218a998(mUnk50, mName, mUnk54);
    Exit();
}

extern "C" void func_ov000_0218a924(void *camera);
extern "C" void func_ov000_0218a93c(void *camera);
extern "C" void func_0202d264(u32 arg0, u32 parent);

void CameraPauseTask::Execute() {
    if (mPause) {
        func_ov000_0218a924(mCamera);
    } else {
        func_ov000_0218a93c(mCamera);
    }
    Exit();
}

void CameraSetDefParentTask::Execute() {
    Camera *camera = mCamera;
    camera->mParent = mParent;
    func_0202d264(camera->mUnk7C, camera->mParent);
    Exit();
}
