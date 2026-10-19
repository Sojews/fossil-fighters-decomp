// M3DScene.
// NONMATCHING: AddModel (the list insert's two iterator temporaries in swapped stack slots) and RemoveModel
// (the game keeps the mutex offset in a register and has 8 more bytes of temporaries).

#include "m/3d_scene.h"
#include "m/3d.h"
#include "m/3d_scene_enable_task.h"
#include "m/3d_resource.h"
#include "m/3d_model.h"
#include "m/heap.h"
#include <algorithm>
#include "m/bg.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_3d_load_csv.h"

extern "C" void G3X_SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog);

M3DScene::M3DScene() : mUnk10(0), mUnk14(1), mUnk18(0x190000), mDirty(TRUE) {
    OS_InitMutex(&mMutex);
    mHeap = new M3DHeap();
    Dino3DInitSoundNames();
    Init();
}

// NONMATCHING: the game copies heapSize out of its register before heap.
M3DScene::M3DScene(u32 heap, u32 heapSize) : mUnk10(0), mUnk14(0), mUnk18(0x190000), mDirty(TRUE) {
    OS_InitMutex(&mMutex);
    mHeap = new M3DHeap(heap, heapSize);
    Dino3DInitSoundNames();
    Init();
}

M3DScene::~M3DScene() {
    Dino3DDestroySoundNames();
    delete mHeap;
    gLcd->mMain->mBg->Delete3d(mM3d);
}

void M3DScene::SetCameraPos(const VecFx32 *pos) {
    mCameraPos = *pos;
    mCameraDirty = TRUE;
    mDirty = TRUE;
}

void M3DScene::SetCameraTarget(const VecFx32 *target) {
    mCameraTarget = *target;
    mCameraDirty = TRUE;
    mDirty = TRUE;
}

void M3DScene::SetCameraUp(const VecFx32 *up) {
    mCameraUp = *up;
    mCameraDirty = TRUE;
    mDirty = TRUE;
}

void M3DScene::SetUnk7C(BOOL value) {
    mUnk7C = value;
}

void M3DScene::QueueE8(const void *src, u32 size) {
    mM3d->QueueE8(src, size);
}

u32 M3DScene::AllocTex(const void *src, u32 size) {
    mUnk148 = 2;
    return mM3d->AllocTex(src, size, TRUE);
}

u32 M3DScene::AllocPltt(const void *src, u32 size) {
    mUnk148 = 2;
    return mM3d->AllocPltt(src, size);
}

void M3DScene::QueueD0(const void *src, u32 size, u32 arg) {
    mM3d->QueueD0(src, size, arg);
}

void M3DScene::FreeTex(u32 key) {
    mM3d->FreeTex(key);
}

void M3DScene::FreePltt(u32 key) {
    mM3d->FreePltt(key);
}

MTask *M3DScene::CreateWaitTransferCompleteTask() {
    return mM3d->CreateWaitTransferCompleteTask();
}

void M3DScene::SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog) {
    G3X_SetClearColor(rgb, alpha, depth, polygonID, fog);
}

void M3DScene::SetUnk14C(u32 a, u32 b) {
    mUnk14C = b + 0xBF;
    mUnk150 = b;
    mUnk154 = a;
    mUnk158 = a + 0xFF;
}

void M3DScene::SetUnk164(u32 a, u32 b) {
    mUnk164 = a;
    mUnk168 = b;
}

void M3DScene::SetUnk80(u32 a, u32 b) {
    mUnk80 = a;
    mUnk84 = b;
}

void M3DScene::Unk0C() {
}

extern "C" void G3i_PerspectiveW_(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, BOOL draw,
                                  MtxFx44 *mtx);
extern "C" void G3X_SetFog(BOOL enable, int mode, int depth, int offset);
extern "C" s16 data_0214c180[6];

void M3DScene::Init() {
    for (int i = 0; i < 4; i++) {
        mLights[i].dir[0] = 0x93D;
        mLights[i].dir[1] = -0x93D;
        mLights[i].dir[2] = -0x93D;
        mLights[i].color = 0x7FFF;
        mLights[i].enabled = i == 0 ? TRUE : FALSE;
    }
    mCameraTarget.z = 0;
    mCameraTarget.y = 0;
    mCameraTarget.x = 0;
    mCameraUp.z = 0;
    mCameraUp.x = 0;
    mCameraPos.y = 0;
    mCameraPos.x = 0;
    mCameraUp.y = 0x1000;
    mCameraPos.z = 0x1000;
    mUnk7C = FALSE;
    mUnk148 = 0;
    mEnabled = TRUE;
    mUnk164 = mUnk168 = 0;
    mM3d = gLcd->mMain->mBg->Create3d();
    mM3d->mBgCntBits.priority = 3;
    mM3d->Show();
    SetClearColor(0, 0, 0x7FFF, 0x3F, FALSE);
    SetUnk14C(0, 0);
    G3X_SetFog(FALSE, 0, 0, 0);
    *(vu16 *) 0x04000060 &= 0xCFFB;
    data_0214c180[0] = -0x1000;
    data_0214c180[1] = -0x2000;
    data_0214c180[2] = -0x1000;
    data_0214c180[3] = 0x2000;
    data_0214c180[4] = 0x4000;
    data_0214c180[5] = 0x2000;
    G3i_PerspectiveW_(0x800, 0xDDB, 0x1555, 0x1000, 0x190000, 0x1000, TRUE, NULL);
    *(vu32 *) 0x0400044C = 0;
    SetFov(0x1555);
    SetUnk80(0, 0);
}

void M3DScene::SetFov(int fovy) {
    G3i_PerspectiveW_(FX_SinCosTable_[((u16) fovy >> 4) * 2], FX_SinCosTable_[((u16) fovy >> 4) * 2 + 1], 0x1555, 0x800, mUnk18,
                      0x1000, FALSE, &mProjection);
    if (mUnk7C) {
        MtxFx44 mirror;
        MTX_Identity44_(&mirror);
        MTX_ScaleApply44(&mirror, &mirror, -0x1000, 0x1000, 0x1000);
        MTX_Concat44(&mProjection, &mirror, &mProjection);
    }
    if (mUnk164 | mUnk168) {
        MtxFx44 offset;
        MTX_Identity44_(&offset);
        MTX_TransApply44(&offset, &offset, mUnk164, mUnk168, 0);
        MTX_Concat44(&mProjection, &offset, &mProjection);
    }
}

MTask *M3DScene::CreateEnableTask(u8 enable) {
    return new M3DSceneEnableTask(this, enable);
}

void M3DScene::AddModel(M3DModel *model) {
    MScopedLock lock(&mMutex, TRUE);
    mModels.push_back(model);
    model->mScene = this;
}

void M3DScene::RemoveModel(M3DModel *model) {
    OS_LockMutex(&mMutex);
    std::list<M3DModel *>::iterator it = std::find(mModels.begin(), mModels.end(), model);
    if (it != mModels.end()) {
        mModels.erase(it);
    }
    OS_UnlockMutex(&mMutex);
}
