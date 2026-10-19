#ifndef GAME_DINO_UI_RV_INFORMATION3D_MODEL_H
#define GAME_DINO_UI_RV_INFORMATION3D_MODEL_H

#include "m/task.h"
#include "m/fx32.h"

class DinoUiRvInformation3DModel : public MProcess {
public:
    virtual ~DinoUiRvInformation3DModel();
    virtual void Idle();

    u8 unk74[0x1B4 - 0x74];
    MFx32 mAngle;   // 0x1B4: 0 to 100
};

#endif
