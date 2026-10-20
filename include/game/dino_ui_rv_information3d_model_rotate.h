#ifndef GAME_DINO_UI_RV_INFORMATION3D_MODEL_ROTATE_H
#define GAME_DINO_UI_RV_INFORMATION3D_MODEL_ROTATE_H

#include "m/task.h"
#include "nitro/fx.h"

class DinoUiRvInformation3DModel;

class MAnmModelBase;

class DinoUiRvInformation3DModelRotate : public MTask {
public:
    DinoUiRvInformation3DModelRotate(MAnmModelBase *model, fx32 pivotY) {
        mOwner = model;
        mPivotY = pivotY;
        mRotZ = 0;
        mRotY = -0x5A000;
        mRotX = 0;
    }
    virtual ~DinoUiRvInformation3DModelRotate();
    virtual void Execute();

    MtxFx44 mMtx;                           // 0x10: given to the model
    fx32 mRotZ;                             // 0x50: degrees
    fx32 mRotY;                             // 0x54
    fx32 mRotX;                             // 0x58
    MAnmModelBase *mOwner;                  // 0x5C: the model turned
    fx32 mPivotY;                           // 0x60: the height it turns around
};

#endif
