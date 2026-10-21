// M3DModel.
// Not decompiled yet: Draw. (Unk08, IsAnimDone and GetFrameCount are in m3d_scene_enable_task.cpp.)
// NONMATCHING: LoadGrid (the game indexes the chunk lengths as cmds[i + 3] and cmds[i + 2] from scratch; ours folds
// them into one address).

#include "m/3d_model.h"
#include <string.h>
#include "m/3d_resource.h"
#include "m/3d_scene.h"
#include "nitro/g3.h"
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
    mFrame = 0;
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
    MI_CpuCopy32(&data_0214c180, mBox, sizeof(mBox));
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
            frames += count * ((mFrame >> 4) % frameCount);
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

// Takes the model resource and reads the tagged blocks after its header, up to 0xFFF. Unless the resource says
// otherwise (flag 0x100), the model data block is copied (when own is set).
// NONMATCHING: the game indexes the blocks as script + (i + n) * 2 where ours folds n into the load, and it
// loads the resource before setting mUnk32A.
void M3DModel::SetModelData(void *data, BOOL own) {
    mModelRes = data;
    mFlags &= ~0x101;
    ReleaseModelData();
    if (((const u32 *)mModelRes)[6] & 0x100) {
        own = FALSE;
    }
    mOwnsModelData = own;
    mUnk32A = 1;
    const u16 *script = (const u16 *)((const u8 *)mModelRes + 0x28);
    u32 i = 0;
    if (script[0] != 0xFFF) {
        do {
            switch (script[i]) {
            case 0x50:
                mUnk314 = script[i + 2];
                mUnk314 |= script[i + 3] << 16;
                i += 4;
                mUnk30C = &script[i];
                i += mUnk314 >> 1;
                mUnk314 >>= 3;
                break;
            case 0x51:
                mUnk310 = script[i + 2];
                mUnk310 |= script[i + 3] << 16;
                i += 4;
                mUnk308 = &script[i];
                i += mUnk310 >> 1;
                mUnk310 >>= 3;
                break;
            case 0x53: {
                mUnk324 = script[i + 1] + 1;
                MI_CpuCopy32(&script[i + 2], mBox, sizeof(mBox));
                s16 size = mBox[3];
                if (size < mBox[4]) {
                    size = mBox[4];
                }
                if (size < mBox[5]) {
                    size = mBox[5];
                }
                i += 8;
                mBox[3] = mBox[4] = mBox[5] = size;
                mBox[0] = mBox[1] = mBox[2] = -size >> 1;
                mFlags |= 0x100;
                break;
            }
            case 0x52:
                mUnk300 = script[i + 2];
                mUnk300 |= script[i + 3] << 16;
                i += 4;
                if (mOwnsModelData) {
                    mModelData = new u8[mUnk300 & ~1];
                    MI_CpuCopy32(&script[i], mModelData, mUnk300);
                } else {
                    mModelData = (u8 *)&script[i];
                }
                i += mUnk300 >> 1;
                break;
            default:
                mUnk32A = 0;
                mUnk2F4 = 0x7820;
                goto done;
            }
        } while (script[i] != 0xFFF);
    }
done:
    mFlags |= 1;
    mUnk329 = 0;
}

void M3DModel::SetAnimData(void *data) {
    mAnimRes = data;
    int i;
    const u32 *anim = (const u32 *) mAnimRes;
    const u8 *p = (const u8 *) anim + (anim[3] + (anim[1] + anim[2]));
    int size = anim[4];
    for (i = 0; i < 0x80; i++) {
        mJointFrames[i] = NULL;
    }
    while (size > 0) {
        int joint = *(const int *) p;
        if (joint >= 0x80) {
            break;
        }
        p += 4;
        mJointFrames[joint] = (const MtxFx43 *) p;
        int len = ((const u32 *) mAnimRes)[6] * sizeof(MtxFx43);
        size -= len + 4;
        p += len;
    }
    mFlags |= 2;
    mUnk329 = 0;
    mFrame = 0;
}

// A 32-bit value stored as two halfwords.
static inline u32 ReadU32(const u16 *p, u32 i) {
    return (p[i + 1] << 16) | p[i];
}

// Makes the ground from the model file's grid chunk.
void M3DModel::LoadGrid(const void *data) {
    mFlags &= ~0x100;
    ReleaseGrid();
    mGrid = new M3DGrid();
    mGridUnk = ((const u32 *) data)[7];
    const u16 *cmds = (const u16 *) ((const u8 *) data + 0x28);
    int i = 0;
    while (cmds[i] != 0xFFF) {
        switch (cmds[i]) {
        case 0x50: {
            u32 len = ReadU32(cmds, i + 2);
            i = i + 4 + (len >> 1);
            break;
        }
        case 0x51: {
            u32 len = ReadU32(cmds, i + 2);
            i = i + 4 + (len >> 1);
            break;
        }
        case 0x52: {
            u32 len = ReadU32(cmds, i + 2);
            i += 4;
            mGrid->Load(&cmds[i]);
            i += len >> 1;
            break;
        }
        case 0x53:
            i += 8;
            break;
        default:
            return;
        }
    }
}

// Where the joint shows on the bottom screen (in pixels); FALSE if that's off it.
BOOL M3DModel::GetJointScreenPos(int joint, MPoint *pos) {
    MtxFx44 mtx;
    GetJointMatrix(joint, &mtx, FALSE);
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_PushMtx();
    G3_LoadMtx43(&mScene->mCamera);
    if (mUnkC4 != NULL) {
        G3_MultMtx44(mUnkC4);
    }
    G3_MultMtx44(GetMatrix());
    G3_PositionTest(mtx._30, mtx._31, mtx._32);
    fx32 w;
    VecFx32 vec;
    while (G3X_GetPositionTestResult(&vec, &w)) {
    }
    fx64c inv = FX_InvFx64c(w);
    vec.x = (FX_Mul32x64c(vec.x, inv) + FX32_ONE) / 2;
    vec.y = (FX_Mul32x64c(vec.y, inv) + FX32_ONE) / 2;
    G3_PopMtx(1);
    pos->x = (vec.x * 255 + (FX32_ONE >> 1)) >> FX32_SHIFT;
    pos->y = 191 - ((vec.y * 192 + (FX32_ONE >> 1)) >> FX32_SHIFT);
    if (vec.x < 0 || vec.y < 0 || vec.x > FX32_ONE || vec.y > FX32_ONE) {
        return FALSE;
    }
    return TRUE;
}

// A morph animation: frames of every vertex's position, each after its time in frames.
struct M3DMorphAnim {
    u8 unk00[0x1C];
    int frameCount;     // 0x1C
    int vertexCount;    // 0x20
    u8 unk24[4];
    s16 frames[1];      // 0x28: per frame, the time (u16) and a pad, then the vertices' x, y, z
};

// The vertices blended between the current animation frame's two keys.
extern "C" s16 data_0214c18c[];

#define REG_CP_DIVCNT (*(volatile u16 *) 0x04000280)
#define REG_CP_DIV_NUMER_L (*(volatile s32 *) 0x04000290)
#define REG_CP_DIV_DENOM (*(volatile u64 *) 0x04000298)
#define REG_CP_DIV_RESULT_L (*(volatile s32 *) 0x040002a0)

// The hardware divider, as the SDK's CP functions use it.
static inline void M3DSetDiv32_32(u32 numer, u32 denom) {
    REG_CP_DIVCNT = 0;
    REG_CP_DIV_NUMER_L = numer;
    REG_CP_DIV_DENOM = denom;
}

static inline s32 M3DGetDivResult32() {
    while (REG_CP_DIVCNT & 0x8000) {
    }
    return REG_CP_DIV_RESULT_L;
}

#pragma section ITCM begin
#pragma thumb off

// The model's vertices at its animation frame: a key's own when the frame is on it, else the two keys around it
// blended (by the CPU and the hardware divider in turn).
// NONMATCHING: register allocation (the game spills the count and the divider's zero high word to the stack).
const s16 *M3DModelGetMorphVertices(M3DModel *model) {
    const M3DMorphAnim *anim = (const M3DMorphAnim *) model->mAnimRes;
    int count = anim->vertexCount;
    if (count == 0) {
        return data_0214c18c;
    }
    int w1 = 0;
    u32 flags = ((const u32 *) model->mModelRes)[5];
    const s16 *prev = anim->frames;
    const s16 *next = prev;
    u32 t1 = 0;
    u32 t0 = 0;
    u32 frame = model->mFrame;
    s16 *out = data_0214c18c;
    int w0;
    int d;
    if (flags & 2) {
        for (; w1 < anim->frameCount; ) {
            t1 = *(const u16 *) next;
            if (t1 > frame >> 4) {
                break;
            }
            w1++;
            prev = next;
            t0 = t1;
            next += anim->vertexCount * 3 + 2;
        }
        int dt = t1 - t0;
        w1 = frame - (t0 << 4);
        d = dt << 4;
        w0 = (dt << 4) - w1;
    }
    prev += 2;
    next += 2;
    if (t0 == t1 || w1 == 0) {
        return prev;
    }
    if (count >= 650) {
        return prev;
    }
    if (count & 1) {
        out[0] = (prev[0] * w0 + next[0] * w1) / d;
        out[1] = (prev[1] * w0 + next[1] * w1) / d;
        out[2] = (prev[2] * w0 + next[2] * w1) / d;
        next += 3;
        prev += 3;
        out += 3;
    }
    int n = (count >> 1) * 3;
    for (int i = 0; i < n; i++) {
        M3DSetDiv32_32(prev[0] * w0 + next[0] * w1, d);
        out[1] = (prev[1] * w0 + next[1] * w1) / d;
        next += 2;
        prev += 2;
        *out = M3DGetDivResult32();
        out += 2;
    }
    return data_0214c18c;
}

// A texture entry of the model's texture table (mUnkDC), 0x1C bytes.
struct M3DTexEntry {
    u8 unk00[0x1C];
};

#define M3D_TEX_PLTT_OFFSET(tex) (*(const u32 *) ((const u8 *) (tex) + 0x20))
#define M3D_TEX_IMAGE_PARAM(tex) (*(const s32 *) ((const u8 *) (tex) + 0x24))

extern "C" void G3_LoadTexMtxTexCoord(const MtxFx44 *m);
extern "C" void G3_MultMtx43(const MtxFx43 *m);

// Patches the model's display list for this frame at each of the tags' offsets: vertex positions, texture and
// palette bases, polygon attributes, matrices.
// NONMATCHING: a few register choices, and the game masks the polygon id before the mirror test.
void M3DModelPatchDisplayList(M3DModel *model, const u32 *tags, u32 count) {
    const s16 *verts = M3DModelGetMorphVertices(model);
    u32 frame = model->mFrame >> 4;
    const u32 *texWords;
    if (((const u32 *) model->mModelRes)[5] & 0xC) {
        const u8 *anim = (const u8 *) model->mAnimRes;
        texWords = (const u32 *) (anim + (((const u32 *) anim)[1] + ((const u32 *) anim)[2])) +
                   frame * ((const u32 *) anim)[9];
    } else {
        const u8 *anim = (const u8 *) model->mAnimRes;
        texWords = (const u32 *) (anim + (((const u32 *) anim)[1] + ((const u32 *) anim)[2]));
    }
    u32 i = 0;
    if (i >= count) {
        return;
    }
    u32 frameOffset = frame * 0x30;
    do {
        u32 word = tags[i * 2];
        u32 offset = word & 0x0FFFFFFF;
        u32 type = word >> 28;
        u32 arg = tags[i * 2 + 1];
        if (model->mUnk300 >= offset) {
            switch (type) {
            case 0: {
                u16 x = verts[arg];
                const s16 *v = verts + arg;
                u32 at = offset & ~1;
                *(u32 *) (model->mModelData + at) = x | (v[1] << 16);
                *(s16 *) (model->mModelData + at + 4) = v[2];
                break;
            }
            case 1: {
                u32 t = texWords[arg];
                const M3DTexEntry *tex = (const M3DTexEntry *) model->mUnkDC + ((t >> 24) & 0x7F);
                u32 param = (M3D_TEX_IMAGE_PARAM(tex) | ((t << 28) >> 12)) + (model->mTexKey >> 3);
                u32 at = offset & ~1;
                *(u16 *) (model->mModelData + at) = param;
                *(u16 *) (model->mModelData + at + 2) = param >> 16;
                break;
            }
            case 2: {
                u32 t = texWords[arg];
                const M3DTexEntry *tex = (const M3DTexEntry *) model->mUnkDC + ((t >> 24) & 0x7F);
                const M3DTexEntry *pltt = (const M3DTexEntry *) model->mUnkDC + ((t >> 16) & 0x7F);
                int shift = 4 - (((M3D_TEX_IMAGE_PARAM(tex) >> 26) & 7) == 2 ? 1 : 0);
                u32 base = (model->mPlttKey + M3D_TEX_PLTT_OFFSET(pltt)) >> shift;
                u32 at = offset & ~1;
                *(u16 *) (model->mModelData + at) = base;
                *(u16 *) (model->mModelData + at + 2) = base >> 16;
                break;
            }
            case 3: {
                u32 t = texWords[arg];
                int alpha = (int) (((t >> 8) & 0x1F) * model->mUnk1C) >> 12;
                u32 id = model->mPolygonId + (model->mUnkD1 & arg);
                if (alpha <= 0) {
                    t &= ~0xC0;
                    alpha = 0x1F;
                }
                if (model->mUnk2E != 0) {
                    alpha = 0;
                }
                id &= 0x3F;
                if (model->mScene->mUnk7C) {
                    u32 x = (t ^ (t >> 1)) & 0x40;
                    x |= x << 1;
                    t ^= x;
                }
                if (alpha > 0x1F) {
                    alpha = 0x1F;
                }
                u32 attr = (t & 0xF0) | (alpha << 16) | (id << 24) | 0xB000;
                u32 at = offset & ~1;
                *(u16 *) (model->mModelData + at) = attr;
                *(u16 *) (model->mModelData + at + 2) = attr >> 16;
                break;
            }
            case 4: {
                MtxFx44 mtx;
                REG_G3_MTX_MODE = 3;
                MTX_Copy43To44_((const MtxFx43 *) ((const u8 *) model->mJointFrames[arg] + frameOffset), &mtx);
                G3_LoadTexMtxTexCoord(&mtx);
                u32 slot = (arg & 0xF) + 0xF;
                *(volatile u32 *) 0x0400044C = slot;
                *(u16 *) (model->mModelData + (offset & ~1)) = slot;
                REG_G3_MTX_MODE = 2;
                break;
            }
            case 5: {
                const u8 *anim = (const u8 *) model->mAnimRes;
                const u8 *mtx = anim + (((const u32 *) anim)[3] + (((const u32 *) anim)[1] + ((const u32 *) anim)[2]));
                u32 n = arg >> 8;
                while (n--) {
                    mtx += 4 + ((const u32 *) anim)[6] * 0x30;
                }
                MIi_CpuCopy32(mtx + 4 + frameOffset, model->mModelData + (offset & ~1), 0x30);
                break;
            }
            case 8:
                *(u16 *) (model->mModelData + (offset & ~1)) = model->mUnk2F4;
                break;
            case 10:
                if (model->unk2E0 != 0) {
                    REG_G3_MTX_PUSH = 0;
                    G3_MultMtx43((const MtxFx43 *) (model->unk2E0 + arg * 0x30));
                    *(volatile u32 *) 0x0400044C = offset + 5;
                    REG_G3_MTX_POP = 1;
                }
                break;
            }
        }
    } while (++i < count);
}

#pragma thumb reset
#pragma section ITCM end
