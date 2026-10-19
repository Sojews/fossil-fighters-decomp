#ifndef GAME_DINO_UI_RV_INFORMATION3D_MODEL_H
#define GAME_DINO_UI_RV_INFORMATION3D_MODEL_H

#include "m/task.h"
#include "m/fx32.h"

class M3DModel;

class DinoUiRvInformation3DModel : public MProcess {
public:
    virtual ~DinoUiRvInformation3DModel();
    virtual void Idle();

    u8 unk74[0x108 - 0x74];
    M3DModel *mModel;   // 0x108
    u8 unk10C[0x1B4 - 0x10C];
    MFx32 mAngle;   // 0x1B4: 0 to 100
};

#endif
