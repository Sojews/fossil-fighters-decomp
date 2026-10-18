// The revival information screen (ov003).
#ifndef GAME_DINO_UI_RV_INFORMATION_H
#define GAME_DINO_UI_RV_INFORMATION_H

#include "m/task.h"

class DinoUiRvInformation3DModel;

class DinoUiRvInformation : public MProcess {
public:
    virtual ~DinoUiRvInformation();
    virtual void Idle();

    void SetUnk1B1(u8 value);
    void SetUnk1AC(u32 value);
    MTask *CreateWaitModelTask();   // waits until the 3D model's tasks finish

    u8 unk74[0x174 - 0x74];
    DinoUiRvInformation3DModel *mModel; // 0x174
    u8 unk178[0x1AC - 0x178];
    u32 mUnk1AC;                        // 0x1AC
    u8 unk1B0;
    u8 mUnk1B1;                         // 0x1B1
};

#endif
