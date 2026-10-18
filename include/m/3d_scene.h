// A 3D scene: the camera and the models drawn on the 3D layer.
#ifndef M_3D_SCENE_H
#define M_3D_SCENE_H

#include <list>
#include "m/task.h"
#include "nitro/fx.h"

class M3d;
class M3DHeap;
class M3DModel;

class M3DScene : public MTask {
public:
    virtual ~M3DScene();
    virtual void Execute();
    virtual void Unk0C();

    void SetCameraPos(const VecFx32 *pos);
    void SetCameraTarget(const VecFx32 *target);
    void SetCameraUp(const VecFx32 *up);
    void SetUnk7C(BOOL value);
    void QueueE8(const void *src, u32 size);
    u32 AllocTex(const void *src, u32 size);
    u32 AllocPltt(const void *src, u32 size);
    void *QueueD0(const void *src, u32 size, u32 arg);
    void FreeTex(u32 key);
    void FreePltt(u32 key);
    MTask *CreateWaitTransferCompleteTask();
    void SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog);
    void SetUnk14C(u32 a, u32 b);
    void SetUnk164(u32 a, u32 b);
    void SetUnk80(u32 a, u32 b);
    MTask *CreateEnableTask(u8 enable);
    void LoadPltt(u32 key, const void *data);
    void AddModel(M3DModel *model);
    void RemoveModel(M3DModel *model);

    u32 mUnk10;                 // 0x10
    u32 mUnk14;                 // 0x14
    fx32 mUnk18;                // 0x18
    u8 mDirty;                  // 0x1C
    u8 mCameraDirty;            // 0x1D
    u8 unk1E[0x2A];
    std::list<M3DModel *> mModels; // 0x48
    u32 unk54;
    VecFx32 mCameraPos;         // 0x58
    VecFx32 mCameraTarget;      // 0x64
    VecFx32 mCameraUp;          // 0x70
    u8 mUnk7C;                  // 0x7C
    u32 mUnk80;                 // 0x80
    u32 mUnk84;                 // 0x84
    M3d *mM3d;                  // 0x88
    MtxFx43 mCamera;            // 0x8C: the view matrix
    u8 unkBC[0x144 - 0xBC];
    u8 mEnabled;                // 0x144
    u32 mUnk148;                // 0x148
    u32 mUnk14C;                // 0x14C
    u32 mUnk150;                // 0x150
    u32 mUnk154;                // 0x154
    u32 mUnk158;                // 0x158
    u32 unk15C[2];
    u32 mUnk164;                // 0x164
    u32 mUnk168;                // 0x168
    M3DHeap *mHeap;             // 0x16C
    OSMutex mMutex;             // 0x170
};

#endif
