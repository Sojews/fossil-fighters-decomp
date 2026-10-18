// M3DScene.
// Only the small functions are decompiled so far.

#include "m/3d_scene.h"
#include "m/3d.h"
#include "m/3d_scene_enable_task.h"

extern "C" void G3X_SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog);

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

void M3DScene::SetUnk7C(u8 value) {
    mUnk7C = value;
}

void M3DScene::QueueE8(const void *src, u32 size) {
    mM3d->QueueE8(src, size);
}

void M3DScene::AllocTex(const void *src, u32 size) {
    mUnk148 = 2;
    mM3d->AllocTex(src, size, TRUE);
}

void M3DScene::AllocPltt(const void *src, u32 size) {
    mUnk148 = 2;
    mM3d->AllocPltt(src, size);
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
