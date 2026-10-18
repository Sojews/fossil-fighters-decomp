// A model drawn by an M3DScene.
#ifndef M_3D_MODEL_H
#define M_3D_MODEL_H

#include "types.h"
#include "nitro/fx.h"

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
    void SetUnk2F4(u16 value);
    BOOL SetFlag100(BOOL on);
    const MtxFx44 *GetMatrix();
    BOOL GetJointMatrix(int joint, MtxFx44 *mtx, u8 arg);
    int FindJoint(const char *name);

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
    u8 unk54[0x70];
    const MtxFx44 *mUnkC4;      // 0xC4: a matrix to apply
    u32 mTexKey;                // 0xC8
    u32 mPlttKey;               // 0xCC
    u8 mHasTexture;             // 0xD0
    u32 unkD4;
    const void *mModelRes;      // 0xD8: the model resource (its joint names)
    u32 mUnkDC;                 // 0xDC
    u8 unkE0[0x2E4 - 0xE0];
    M3DGrid *mGrid;             // 0x2E4: the ground, if the model has one
    u32 unk2E8[3];
    u16 mUnk2F4;                // 0x2F4
    u8 unk2F6[0x2FC - 0x2F6];
    u8 *mModelData;             // 0x2FC
    u8 unk300[0x328 - 0x300];
    u8 mOwnsModelData;          // 0x328
    u8 unk329[0x32C - 0x329];
};

#endif
