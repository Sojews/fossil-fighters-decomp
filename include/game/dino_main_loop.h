#ifndef GAME_DINO_MAIN_LOOP_H
#define GAME_DINO_MAIN_LOOP_H

#include "m/task.h"

class DinoMainLoop : public MProcess {
public:
    virtual ~DinoMainLoop();
    virtual void Idle();

    void (DinoMainLoop::*mState)(); // 0x74
};

#endif
