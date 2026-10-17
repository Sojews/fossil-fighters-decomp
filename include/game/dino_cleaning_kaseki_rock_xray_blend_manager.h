#ifndef GAME_DINO_CLEANING_KASEKI_ROCK_XRAY_BLEND_MANAGER_H
#define GAME_DINO_CLEANING_KASEKI_ROCK_XRAY_BLEND_MANAGER_H

#include "m/task.h"

class DinoCleaningKasekiRockXrayBlendManager : public MProcess {
public:
    virtual ~DinoCleaningKasekiRockXrayBlendManager();
    virtual void Idle();

    void (DinoCleaningKasekiRockXrayBlendManager::*mState)(); // 0x74
};

#endif
