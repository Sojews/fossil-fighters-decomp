// A model drawn by an M3DScene.
#ifndef M_3D_MODEL_H
#define M_3D_MODEL_H

#include "types.h"
#include "nitro/fx.h"
#include "m/geometry.h"

class M3DScene;
class M3DGrid;

class M3DModel {
public:
    M3DModel();
    virtual ~M3DModel();
    virtual void Unk08(void *model, void *anim, BOOL arg);
    virtual void SetModelData(void *data, BOOL arg);
    virtual void SetAnimData(void *data);
    virtual int GetFrameCount();
    virtual BOOL IsAnimDone();

    fx32 GetGroundHeight(fx32 x, fx32 z);
    BOOL IsOnGround(fx32 x, fx32 z, fx32 *height);
    void SetPolygonId(int id);
    void SetUnk1C(u32 value);
    void SetScale(const VecFx32 *scale);
    void SetRotation(VecFx32 *rotation);
    void SetTranslation(const VecFx32 *trans);
    void SetUnk10(u8 value);
    u8 GetUnk10();
    void SetTexture(u32 unkDC, u32 texKey, u32 plttKey);
    void ReleaseTexture();
    void ReleaseModelData();
    void ReleaseGrid();
    void LoadGrid(const void *data);
    void SetUnk2F4(u16 value);
    BOOL SetFlag100(BOOL on);
    const MtxFx44 *GetMatrix();
    BOOL GetJointMatrix(int joint, MtxFx44 *mtx, u8 arg);
    int FindJoint(const char *name);
    BOOL GetJointScreenPos(int joint, MPoint *pos);

    M3DScene *mScene;           // 0x04
    u32 mFlags;                 // 0x08
    u32 mUnkC;                  // 0x0C
    u8 mUnk10;                  // 0x10
    u32 mUnk14;                 // 0x14
    int mPolygonId;             // 0x18
    u32 mUnk1C;                 // 0x1C
    u32 mUnk20;                 // 0x20
    u16 mColors[4];             // 0x24
    u8 mUnk2C;                  // 0x2C
    u8 mUnk2D;                  // 0x2D
    u8 mUnk2E;                  // 0x2E
    u8 mMatrixValid;            // 0x2F
    VecFx32 mTrans;             // 0x30
    VecFx32 mRotation;          // 0x3C: angles, 16 bits each
    VecFx32 mScale;             // 0x48
    MtxFx44 mMatrix;            // 0x54: translation * rotation (x, y, z) * scale
    u8 unk94[0xC4 - 0x94];
    const MtxFx44 *mUnkC4;      // 0xC4: a matrix to apply
    u32 mTexKey;                // 0xC8
    u32 mPlttKey;               // 0xCC
    u8 mHasTexture;             // 0xD0
    const void *mAnimRes;       // 0xD4: the joint animation resource
    const void *mModelRes;      // 0xD8: the model resource (its joint names)
    u32 mUnkDC;                 // 0xDC
    const MtxFx43 *mJointFrames[0x80]; // 0xE0: each animated joint's matrices, one per frame
    u32 unk2E0;
    M3DGrid *mGrid;             // 0x2E4: the ground, if the model has one
    u32 mGridUnk;               // 0x2E8
    u32 mFrame;                 // 0x2EC: the animation frame, in 1/16ths
    u32 mUnk2F0;                // 0x2F0
    u16 mUnk2F4;                // 0x2F4
    u32 mUnk2F8;                // 0x2F8
    u8 *mModelData;             // 0x2FC
    u32 mUnk300;                // 0x300
    u8 unk304[0x318 - 0x304];
    VecFx32 mUnk318;            // 0x318
    u32 unk324;
    u8 mOwnsModelData;          // 0x328
    u8 mUnk329;                 // 0x329
    u8 mUnk32A;                 // 0x32A
    u8 unk32B;
};

#endif
