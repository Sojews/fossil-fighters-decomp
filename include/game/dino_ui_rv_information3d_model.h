#ifndef GAME_DINO_UI_RV_INFORMATION3D_MODEL_H
#define GAME_DINO_UI_RV_INFORMATION3D_MODEL_H

#include "m/task.h"
#include "m/fx32.h"

class M3DModel;
class M3DScene;

class DinoUiRvInformation3DModel : public MProcess {
public:
    virtual ~DinoUiRvInformation3DModel();
    virtual void Idle();

    void SetUnk84(u32 value);

    u32 unk74;
    M3DScene *mScene;   // 0x78
    u32 unk7C;
    u32 mUnk80;         // 0x80
    u32 mUnk84;         // 0x84
    u8 mUnk88;          // 0x88
    u8 unk89[0x108 - 0x89];
    M3DModel *mModel;   // 0x108
    u8 unk10C[0x1B4 - 0x10C];
    MFx32 mAngle;   // 0x1B4: 0 to 100
};

#endif
