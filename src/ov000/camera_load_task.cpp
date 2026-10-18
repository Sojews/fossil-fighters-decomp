// CameraLoadTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/camera_load_task.h"
#include "game/dino_3d_load_csv.h"
#include "m/3d_scene.h"
#include "m/file.h"
#include "types.h"
#include "game/dino_battle_view_3d_camera.h"

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
    mCamera->SetLight(mLight);
    Exit();
}

void CameraLoadTask::Execute() {
    mCamera->Load(mName, mLight);
    Exit();
}

extern "C" void func_ov000_0218a924(void *camera);
extern "C" void func_ov000_0218a93c(void *camera);

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
    camera->mUnk7C->SetParent(camera->mParent);
    Exit();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x3C];
    void (DinoBattleView3DCamera::*Stopped_3C)();
    u8 unk44[0x4C - 0x44];
    void (DinoBattleView3DCamera::*Run_4C)();
    u8 unk54[0x5C - 0x54];
    void (DinoBattleView3DCamera::*Run_5C)();
    void (DinoBattleView3DCamera::*Run_64)();
} data_ov000_02196cd0;
extern "C" u8 data_ov000_021903dc[];

DinoBattleView3DCamera::DinoBattleView3DCamera(M3DScene *scene, Dino3DLoadCsv *camera, u32 unk88)
    : MProcess(NULL), mCamera(camera), mScene(scene) {
    mState = data_ov000_02196cd0.Run_4C;
    mUnk9C = data_ov000_021903dc;
    mUnk88 = unk88;
    Reset();
    mLight = -1;
    unk80 = 0;
}

void DinoBattleView3DCamera::Start(u32 unk88, const void *unk9C, int light) {
    mState = data_ov000_02196cd0.Run_5C;
    mUnk88 = unk88;
    mUnk9C = unk9C;
    if (light >= 0) {
        mScene->SetUnk7C(light != 0 ? TRUE : FALSE);
        mLight = light;
    }
    Reset();
}

void DinoBattleView3DCamera::Stop() {
    mState = data_ov000_02196cd0.Stopped_3C;
    mCamera->Push18();
}

void DinoBattleView3DCamera::Resume() {
    mState = data_ov000_02196cd0.Run_64;
    mScene->SetUnk7C(mLight != 0 ? TRUE : FALSE);
    mCamera->Push17();
}

void DinoBattleView3DCamera::Reset() {
    mLoaded = FALSE;
    mUnkA1 = 0;
    mUnkA2 = 1;
    mUnk8C = 0;
    mUnk90 = 0;
    mUnk94 = -1;
    mCamera->Push18();
}

void DinoBattleView3DCamera::SetLight(int light) {
    mLoaded = FALSE;
    if (light >= 0) {
        mScene->SetUnk7C(light != 0 ? TRUE : FALSE);
    }
    mCamera->Push18();
}

void DinoBattleView3DCamera::Load(const char *path, int light) {
    if (MFileExists(path)) {
        mLoaded = TRUE;
        if (light >= 0) {
            mScene->SetUnk7C(light != 0 ? TRUE : FALSE);
        }
        mCamera->Load(path, 0);
    }
    mUnkA1 = 0;
    mUnkA2 = 1;
    mCamera->Push17();
}
