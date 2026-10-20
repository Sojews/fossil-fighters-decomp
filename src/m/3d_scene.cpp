// M3DScene.
// NONMATCHING: AddModel (the list insert's two iterator temporaries in swapped stack slots) and RemoveModel
// (the game keeps the mutex offset in a register and has 8 more bytes of temporaries).

#pragma ipa file
#include "m/3d_scene.h"
#include "m/3d.h"
#include "m/3d_scene_enable_task.h"
#include "m/3d_resource.h"
#include "m/3d_model.h"
#include "m/heap.h"
#include "nitro/g3.h"
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

extern "C" {
BOOL MI_IsDmaBusy(u32 dma);
extern u32 GXi_DmaId;
s32 G3X_GetMtxStackLevelPV(s32 *level);
void G2_SetBG0Offset_(u32 offset);
void G3_LoadMtx44(const MtxFx44 *m);
void G3i_LookAt_(const VecFx32 *camPos, const VecFx32 *camUp, const VecFx32 *target, BOOL draw, MtxFx43 *mtx);
}

#define REG_DISP3DCNT (*(volatile u16 *) 0x04000060)
#define REG_G3_MTX_IDENTITY (*(volatile u32 *) 0x04000454)
#define REG_G3_TEXPLTT_BASE (*(volatile u32 *) 0x040004AC)
#define REG_G3_LIGHT_VECTOR (*(volatile u32 *) 0x040004C8)
#define REG_G3_LIGHT_COLOR (*(volatile u32 *) 0x040004CC)
#define REG_G3_SWAP_BUFFERS (*(volatile u32 *) 0x04000540)
#define REG_G3_VIEWPORT (*(volatile u32 *) 0x04000580)
#define REG_G3X_GXSTAT (*(volatile u32 *) 0x04000600)

// Draws a frame (unless the scene is off, waiting, or the texture DMA is busy): resets the geometry engine,
// sets the viewport, projection, camera and lights, draws the models and swaps the buffers.
void M3DScene::Execute() {
    if (!mEnabled) {
        return;
    }
    if (mUnk148 != 0) {
        mUnk148--;
        return;
    }
    mFrames++;
    if (MI_IsDmaBusy(GXi_DmaId)) {
        return;
    }
    while (REG_G3X_GXSTAT & 0x08000000) {
    }
    REG_G3X_GXSTAT |= 0x8000;
    REG_DISP3DCNT |= 0x2000;
    REG_DISP3DCNT |= 0x1000;
    s32 level;
    while (G3X_GetMtxStackLevelPV(&level)) {
    }
    G3_MtxMode(3);
    REG_G3_MTX_IDENTITY = 0;
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_PopMtx(level);
    REG_G3_POLYGON_ATTR = 0x1F3080;
    REG_G3_TEXIMAGE_PARAM = 0;
    REG_G3_TEXPLTT_BASE = 0;
    REG_G3_VIEWPORT = mUnk154 | (mUnk150 << 8) | (mUnk158 << 16) | (mUnk14C << 24);
    G2_SetBG0Offset_(mUnk80);
    G3_MtxMode(0);
    G3_LoadMtx44(&mProjection);
    if (mCameraDirty) {
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
        G3i_LookAt_(&mCameraPos, &mCameraUp, &mCameraTarget, TRUE, &mCamera);
        mCameraDirty = FALSE;
        mCameraRot._00 = mCamera._00;
        mCameraRot._01 = mCamera._10;
        mCameraRot._02 = mCamera._20;
        mCameraRot._10 = mCamera._01;
        mCameraRot._11 = mCamera._11;
        mCameraRot._12 = mCamera._21;
        mCameraRot._20 = mCamera._02;
        mCameraRot._21 = mCamera._12;
        mCameraRot._22 = mCamera._22;
    }
    if (mDirty) {
        mLightMask = 0;
        for (int i = 0; i < 4; i++) {
            if (mLights[i].enabled) {
                u32 id = (u32) i << 30;
                REG_G3_LIGHT_VECTOR = id | (((mLights[i].dir[0] >> 3) & 0x3FF) | (((mLights[i].dir[1] >> 3) & 0x3FF) << 10) |
                                            (((mLights[i].dir[2] >> 3) & 0x3FF) << 20));
                REG_G3_LIGHT_COLOR = id | mLights[i].color;
                mLightMask |= 1 << i;
            }
        }
        mDirty = FALSE;
    }
    G3_Begin(0);
    G3_End();
    {
        MScopedLock lock(&mMutex, TRUE);
        for (std::list<M3DModel *>::iterator it = mModels.begin(); it != mModels.end(); it++) {
            M3DModel *model = *it;
            if (model->mUnk10) {
                if (mUnk10 == 1) {
                    model->mUnkD1 = FALSE;
                } else {
                    model->mUnkD1 = TRUE;
                }
                (*it)->Draw(mLightMask);
            }
        }
    }
    Unk0C();
    REG_G3_SWAP_BUFFERS = mUnk10 | (mUnk14 << 1);
    mPrevFrames = mFrames;
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
