#ifndef GAME_DINO_UI_RV_INFORMATION3D_MODEL_H
#define GAME_DINO_UI_RV_INFORMATION3D_MODEL_H

#include "m/task.h"
#include "m/fx32.h"

class M3DModel;
class M3DScene;

class MInput;

class DinoUiRvInformation3DModel : public MProcess {
public:
    DinoUiRvInformation3DModel();
    virtual ~DinoUiRvInformation3DModel();
    virtual void Idle();

    void SetUnk84(u32 value);
    // Loads the creature's model and its turning (on the background thread).
    void LoadModel();

    MParallelTaskSet *mTasks;   // 0x74
    M3DScene *mScene;   // 0x78
    class MAnmModel *mAnm;  // 0x7C
    u32 mUnk80;         // 0x80
    u32 mUnk84;         // 0x84
    u8 mUnk88;          // 0x88
    u32 mUnk8C;         // 0x8C
    u32 mUnk90;         // 0x90
    u32 mUnk94;         // 0x94
    MInput *mInput;     // 0x98
    u8 mUnk9C;          // 0x9C
    u8 unk9D[0xA8 - 0x9D];
    u32 mUnkA8;         // 0xA8
    u32 mUnkAC;         // 0xAC
    class DinoUiRvInformation3DModelRotate *mRotate;    // 0xB0
    u8 unkB4[0x108 - 0xB4];
    M3DModel *mModel;   // 0x108
    u8 unk10C[0x1B4 - 0x10C];
    MFx32 mAngle;   // 0x1B4: 0 to 100
};

#endif
