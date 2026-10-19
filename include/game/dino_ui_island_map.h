#ifndef GAME_DINO_UI_ISLAND_MAP_H
#define GAME_DINO_UI_ISLAND_MAP_H

#include "m/task.h"

// The island map: picks a spot to go to.
class DinoUiIslandMap : public MProcess {
public:
    DinoUiIslandMap(s32 *result, u32 unlocked);
    virtual ~DinoUiIslandMap();
    virtual void Idle();

    u32 mUnlocked;      // 0x74
    s32 *mResult;       // 0x78
};

#endif
