// The revival information screen (ov003).
#ifndef GAME_DINO_UI_RV_INFORMATION_H
#define GAME_DINO_UI_RV_INFORMATION_H

#include "m/picture.h"
#include "m/resource.h"
#include "m/task.h"

class DinoUiRvInformation3DModel;

// A creature's details as the information screen shows them (passed by value).
struct DinoCreatureInfo {
    u32 unk00[6];
};

// (The data's layout isn't known yet.)
struct DinoUiRvInformationData;

class DinoUiRvInformation : public MProcess {
public:
    virtual ~DinoUiRvInformation();
    virtual void Idle();

    void SetUnk1B1(u8 value);
    void SetUnk1AC(u32 value);
    MTask *CreateWaitModelTask();   // waits until the 3D model's tasks finish

    MPictureFile *mPictures[10];        // 0x74
    u8 unk9C[0x174 - 0x9C];
    DinoUiRvInformation3DModel *mModel; // 0x174
    u8 unk178[0x194 - 0x178];
    u8 mHasTenPictures;                 // 0x194: else 7
    u8 unk195[0x19C - 0x195];
    MResource<DinoUiRvInformationData> mUnk19C; // 0x19C
    u32 mUnk1AC;                        // 0x1AC
    u8 unk1B0;
    u8 mUnk1B1;                         // 0x1B1
};

#endif
