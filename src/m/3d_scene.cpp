// M3DScene.
// Only the small functions are decompiled so far.
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

void *M3DScene::QueueD0(const void *src, u32 size, u32 arg) {
    return mM3d->QueueD0(src, size, arg);
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
