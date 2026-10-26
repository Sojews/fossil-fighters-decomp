#ifndef GAME_DINO_UI_RV_INFORMATION3D_MODEL_H
#define GAME_DINO_UI_RV_INFORMATION3D_MODEL_H

#include "m/task.h"
#include "m/fx32.h"
#include "m/geometry.h"

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
    fx32 mUnk8C;        // 0x8C: the model's turn, degrees (Z)
    fx32 mUnk90;        // 0x90: (Y), 0 to 360
    fx32 mUnk94;        // 0x94: (X), -60 to 60
    MInput *mInput;     // 0x98
    u8 mDragging;       // 0x9C: a drag started on the model
    u8 unk9D[0xA0 - 0x9D];
    MPoint mTouch;      // 0xA0: the drag's last touch
    fx32 mUnkA8;        // 0xA8: the Y turn's speed
    fx32 mUnkAC;        // 0xAC: the X turn's speed
    class DinoUiRvInformation3DModelRotate *mRotate;    // 0xB0
    u8 unkB4[0xF4 - 0xB4];
};

#endif
