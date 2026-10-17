#ifndef GAME_DINO_UI_ISLAND_MAP_H
#define GAME_DINO_UI_ISLAND_MAP_H

#include "m/task.h"

class DinoUiIslandMap : public MProcess {
public:
    virtual ~DinoUiIslandMap();
    virtual void Idle();
};

#endif
