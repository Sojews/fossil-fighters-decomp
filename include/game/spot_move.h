#ifndef GAME_SPOT_MOVE_H
#define GAME_SPOT_MOVE_H

#include "m/task.h"

class SpotMove : public MProcess {
public:
    virtual ~SpotMove();
    virtual void Idle();
};

#endif
