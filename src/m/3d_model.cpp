// M3DModel.
// Only the small functions are decompiled so far.

#include "m/3d_model.h"
#include <string.h>
#include "m/3d_resource.h"
#include "m/3d_scene.h"

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
