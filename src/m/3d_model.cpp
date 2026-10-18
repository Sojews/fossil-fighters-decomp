// M3DModel.
// Only the small functions are decompiled so far.

#include "m/3d_model.h"
#include <string.h>
#include "m/3d_resource.h"
#include "m/3d_scene.h"
#include "nitro/mi.h"

extern VecFx32 data_0214c180; // set up with the scene

M3DModel::M3DModel() {
    mUnkC = 1;
    mUnk10 = 0;
    mUnk14 = 0xF;
    mPolygonId = 0;
    mUnk1C = 0x1F;
    mUnk20 = 0;
    mColors[0] = 0x5EF7;
    mColors[1] = 0x2108;
    mColors[2] = 0x7FFF;
    mColors[3] = 0;
    mUnk2C = 0;
    mUnk2D = 0;
    mMatrixValid = FALSE;
    mTrans.x = mTrans.y = mTrans.z = 0;
    mRotation.x = mRotation.y = mRotation.z = 0;
    mScale.x = mScale.y = mScale.z = FX32_ONE;
    mUnk2EC = 0;
    mUnk2F0 = 1;
    mUnk2F4 = 0x7FFF;
    mUnk2F8 = 0;
    mHasTexture = FALSE;
    mUnkC4 = NULL;
    mUnk32A = 0;
    mModelData = NULL;
    mUnk300 = 0;
    mFlags = 0;
    mGrid = NULL;
    mUnk2E = 0;
    MI_CpuCopy32(&data_0214c180, &mUnk318, sizeof(VecFx32));
    mOwnsModelData = FALSE;
}

void M3DModel::SetPolygonId(int id) {
    mPolygonId = id & 0x3F;
}

void M3DModel::SetUnk1C(u32 value) {
    mUnk1C = value;
}

void M3DModel::SetScale(const VecFx32 *scale) {
    fx32 x = scale->x;
    if (mScale.x != x || mScale.y != scale->y || mScale.z != scale->z) {
        mScale.x = x;
        mScale.y = scale->y;
        mScale.z = scale->z;
        mMatrixValid = FALSE;
    }
}

void M3DModel::SetRotation(VecFx32 *rotation) {
    rotation->x = (u16)rotation->x;
    rotation->y = (u16)rotation->y;
    rotation->z = (u16)rotation->z;
    fx32 x = rotation->x;
    if (mRotation.x != x || mRotation.y != rotation->y || mRotation.z != rotation->z) {
        mRotation.x = x;
        mRotation.y = rotation->y;
        mRotation.z = rotation->z;
        mMatrixValid = FALSE;
    }
}

void M3DModel::SetTranslation(const VecFx32 *trans) {
    fx32 x = trans->x;
    if (mTrans.x != x || mTrans.y != trans->y || mTrans.z != trans->z) {
        mTrans.x = x;
        mTrans.y = trans->y;
        mTrans.z = trans->z;
        mMatrixValid = FALSE;
    }
}

void M3DModel::SetUnk10(u8 value) {
    mUnk10 = value;
}

u8 M3DModel::GetUnk10() {
    return mUnk10;
}

void M3DModel::SetTexture(u32 unkDC, u32 texKey, u32 plttKey) {
    ReleaseTexture();
    mUnkDC = unkDC;
    mTexKey = texKey;
    mPlttKey = plttKey;
}

void M3DModel::ReleaseTexture() {
    if (mHasTexture) {
        mScene->FreeTex(mTexKey);
        mScene->FreePltt(mPlttKey);
        mHasTexture = FALSE;
    }
}

void M3DModel::SetUnk2F4(u16 value) {
    mUnk2F4 = value;
}

BOOL M3DModel::SetFlag100(BOOL on) {
    BOOL old = (mFlags & 0x100) ? TRUE : FALSE;
    mFlags &= ~0x100;
    if (on) {
        mFlags |= 0x100;
    }
    return old;
}

fx32 M3DModel::GetGroundHeight(fx32 x, fx32 z) {
    if (mGrid == NULL) {
        return 0;
    }
    return mGrid->GetHeight(x, z);
}

BOOL M3DModel::IsOnGround(fx32 x, fx32 z, fx32 *height) {
    if (mGrid == NULL) {
        return FALSE;
    }
    return mGrid->IsOnGround(x, z, height);
}

M3DModel::~M3DModel() {
    ReleaseModelData();
    ReleaseTexture();
    ReleaseGrid();
}

void M3DModel::ReleaseModelData() {
    if (mOwnsModelData) {
        delete[] mModelData;
        mModelData = NULL;
        mOwnsModelData = FALSE;
    }
}

void M3DModel::ReleaseGrid() {
    if (mGrid != NULL) {
        delete mGrid;
    }
    mGrid = NULL;
}

// The index of the joint with the name, or -1.
int M3DModel::FindJoint(const char *name) {
    int joint = -1;
    const u32 *res = (const u32 *) mModelRes;
    if (res[3] != 0) {
        int i;
        u32 offset = res[1] + res[2];
        const char *entry = (const char *) res + (offset + 4);
        int count = *(const int *) ((const u8 *) res + offset);
        for (i = 0; i < count; i++) {
            if (strncmp((char *) entry, (char *) name, 16) == 0) {
                joint = i;
                break;
            }
            entry += 0x40;
        }
    }
    return joint;
}

const MtxFx44 *M3DModel::GetMatrix() {
    if (!mMatrixValid) {
        mMatrixValid = TRUE;
        MTX_Identity44(&mMatrix);
        MTX_TransApply44(&mMatrix, &mMatrix, mTrans.x, mTrans.y, mTrans.z);
        MtxFx44 rot;
        MTX_RotX44(&rot, FX_SinIdx(mRotation.x), FX_CosIdx(mRotation.x));
        MTX_Concat44(&rot, &mMatrix, &mMatrix);
        MTX_RotY44(&rot, FX_SinIdx(mRotation.y), FX_CosIdx(mRotation.y));
        MTX_Concat44(&rot, &mMatrix, &mMatrix);
        MTX_RotZ44(&rot, FX_SinIdx(mRotation.z), FX_CosIdx(mRotation.z));
        MTX_Concat44(&rot, &mMatrix, &mMatrix);
        MTX_ScaleApply44(&mMatrix, &mMatrix, mScale.x, mScale.y, mScale.z);
    }
    return &mMatrix;
}

// The joint's matrix in the current frame (with withRotation false, only its translation), or the identity for
// joint -1.
BOOL M3DModel::GetJointMatrix(int joint, MtxFx44 *mtx, u8 withRotation) {
    if (joint == -1) {
        MTX_Identity44(mtx);
        return TRUE;
    }
    const u32 *model = (const u32 *) mModelRes;
    if (model[3] != 0) {
        const u32 *anim = (const u32 *) mAnimRes;
        if (anim[5] != 0) {
            const MtxFx43 *frames = (const MtxFx43 *) ((const u8 *) anim + (anim[4] + (anim[3] + (anim[1] + anim[2])) + 8));
            int count = *(const int *) ((const u8 *) model + (model[1] + model[2]));
            u32 frameCount = anim[6];
            if (joint >= count) {
                return FALSE;
            }
            frames += count * ((mUnk2EC >> 4) % frameCount);
            if (withRotation) {
                MTX_Copy43To44(&frames[joint], mtx);
            } else {
                MTX_Identity44(mtx);
                mtx->_30 = frames[joint]._30;
                mtx->_31 = frames[joint]._31;
                mtx->_32 = frames[joint]._32;
            }
            return TRUE;
        }
    }
    return FALSE;
}
