#ifndef GAME_DINO_MAIN_H
#define GAME_DINO_MAIN_H

#include "m/task.h"

class DinoMain : public MProcess {
public:
    virtual ~DinoMain();
    virtual void Idle();
};

#endif
