#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_H

#include "m/task.h"

class DinoUiDebugKasekiRockGet : public MProcess {
public:
    DinoUiDebugKasekiRockGet();
    virtual ~DinoUiDebugKasekiRockGet();
    virtual void Idle();
};

#endif
