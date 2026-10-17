#ifndef GAME_DINO_ATM_SAMPLE_H
#define GAME_DINO_ATM_SAMPLE_H

#include "m/task.h"

class DinoATMSample : public MProcess {
public:
    virtual ~DinoATMSample();
    virtual void Idle();
};

#endif
