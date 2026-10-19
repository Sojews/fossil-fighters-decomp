#ifndef GAME_DINO_CLEANING_KASEKI_ROCK_XRAY_BLEND_MANAGER_H
#define GAME_DINO_CLEANING_KASEKI_ROCK_XRAY_BLEND_MANAGER_H

#include "m/task.h"

class DinoCleaningImageView;

class DinoCleaningKasekiRockXrayBlendManager : public MProcess {
public:
    virtual ~DinoCleaningKasekiRockXrayBlendManager();
    virtual void Idle();

    void StateFade();
    void StateDone();

    void (DinoCleaningKasekiRockXrayBlendManager::*mState)(); // 0x74
    DinoCleaningImageView *mImage;                              // 0x7C: the x-ray image
    int mTimer;                                                 // 0x80
    u8 mHidden;                                                 // 0x84: no blending while it's set
    u32 unk88;
    MTaskSet *mTasks;                                           // 0x8C
};

#endif
