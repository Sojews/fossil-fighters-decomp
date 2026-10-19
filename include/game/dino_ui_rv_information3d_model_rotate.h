#ifndef GAME_DINO_UI_RV_INFORMATION3D_MODEL_ROTATE_H
#define GAME_DINO_UI_RV_INFORMATION3D_MODEL_ROTATE_H

#include "m/task.h"
#include "nitro/fx.h"

class DinoUiRvInformation3DModel;

class DinoUiRvInformation3DModelRotate : public MTask {
public:
    virtual ~DinoUiRvInformation3DModelRotate();
    virtual void Execute();

    MtxFx44 mMtx;                           // 0x10: given to the model
    fx32 mRotZ;                             // 0x50: degrees
    fx32 mRotY;                             // 0x54
    fx32 mRotX;                             // 0x58
    DinoUiRvInformation3DModel *mOwner;     // 0x5C
    fx32 mPivotY;                           // 0x60: the height it turns around
};

#endif
