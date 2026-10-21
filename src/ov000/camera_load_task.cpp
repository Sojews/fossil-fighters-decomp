// CameraLoadTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/camera_load_task.h"
#include "game/dino_3d_load_csv.h"
#include "m/3d_scene.h"
#include "m/file.h"
#include "types.h"
#include "m/seed_generator.h"
#include <string>
#include "game/dino_battle_view_3d_camera.h"
#include "game/dino_static_init.h"

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
    u8 unk00[0x2C];
    void (DinoBattleView3DCamera::*Run_2C)();
    u8 unk34[0x3C - 0x34];
    void (DinoBattleView3DCamera::*Stopped_3C)();
    u8 unk44[0x4C - 0x44];
    void (DinoBattleView3DCamera::*Run_4C)();
    u8 unk54[0x5C - 0x54];
    void (DinoBattleView3DCamera::*Run_5C)();
    void (DinoBattleView3DCamera::*Run_64)();
} data_ov000_02196cd0;
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

// mUnk88: by set, the set's camera files (laid out like std::vector<std::vector<std::string> >).
struct CameraSet {
    std::string *files;
    u32 count;
    u32 capacity;
};
struct CameraSets {
    CameraSet *sets;
    u32 count;
};

static inline CameraSets &GetSets(DinoBattleView3DCamera *camera) {
    return *(CameraSets *)camera->mUnk88;
}

extern "C" MRandom data_ov000_0219791c;

// Runs the cameras of the current set in turn, a random one of each, never the same one twice running when a set is
// replayed.
extern "C" void func_ov000_0218a9f8(DinoBattleView3DCamera *camera) {
    camera->mState = data_ov000_02196cd0.Run_2C;
    if (camera->mCamera->GetUnk78State() != 1 || camera->mLoaded) {
        return;
    }
    if (GetSets(camera).count <= camera->mUnk8C) {
        camera->mUnk8C = GetSets(camera).count - 1;
    }
    if ((int)camera->mUnk8C >= 0) {
        if (camera->mUnk8C == camera->mUnk94) {
            if (GetSets(camera).sets[camera->mUnk8C].count > 1) {
                u32 pick = data_ov000_0219791c.Next(GetSets(camera).sets[camera->mUnk8C].count);
                while (pick == camera->mUnk90) {
                    pick = data_ov000_0219791c.Next(GetSets(camera).sets[camera->mUnk8C].count);
                }
                camera->mUnk90 = pick;
            } else {
                camera->mUnk90 = 0;
            }
            camera->mUnkA1 = TRUE;
        } else {
            camera->mUnk90 = data_ov000_0219791c.Next(GetSets(camera).sets[camera->mUnk8C].count);
        }
        std::string path(GetSets(camera).sets[camera->mUnk8C].files[camera->mUnk90]);
        if (camera->mUnkA2 || !camera->mUnkA1) {
            camera->mCamera->Load(path.c_str(), 0);
        }
        camera->mCamera->PushVec0((const VecFx32 *)camera->mUnk9C);
        camera->mCamera->SetParent(camera->mParent);
        camera->mUnk94 = camera->mUnk8C;
        camera->mUnk8C++;
    }
}

#pragma auto_inline off

// The battle 3D's camera tasks.
extern "C" MTask *func_ov000_0218ab8c(DinoBattleView3DCamera *camera, const char *name, int light) {
    return new CameraLoadTask(camera, name, light);
}

extern "C" MTask *func_ov000_0218abd0(DinoBattleView3DCamera *camera, int light) {
    return new NextCameraTask(camera, light);
}

extern "C" MTask *func_ov000_0218ac04(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    return new ChangeSetTask(arg0, arg1, arg2, arg3);
}

extern "C" MTask *func_ov000_0218ac40(void *camera, u8 pause) {
    return new CameraPauseTask(camera, pause);
}

extern "C" MTask *func_ov000_0218ac74(CameraSetDefParentTask::Camera *camera, Dino3DLoadCsv *parent) {
    return new CameraSetDefParentTask(camera, parent);
}

// (Empty; nothing in the game calls it.)
extern "C" void func_ov000_0218ab68() {
}

// The file's static initialization (see dino_static_init.h).
static struct CameraLoadTaskStaticInit {
    CameraLoadTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_02196cd0, 0x4) = 128;
        DINO_POOL_WORD(data_ov000_02196cd0, 0x8) = 176;
        DINO_POOL_WORD(data_ov000_02196cd0, 0x1c) = 8;
        DINO_POOL_WORD(data_ov000_02196cd0, 0x20) = 150;
        DINO_POOL_WORD(data_ov000_02196cd0, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_02196cd0, 0x10) = 112;
    }
} sCameraLoadTaskStaticInit;
// The cameras' random numbers (constructed after the pooled constants).
extern "C" {
MRandom data_ov000_0219791c;
}
