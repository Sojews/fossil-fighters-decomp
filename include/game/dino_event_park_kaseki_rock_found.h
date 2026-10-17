#ifndef GAME_DINO_EVENT_PARK_KASEKI_ROCK_FOUND_H
#define GAME_DINO_EVENT_PARK_KASEKI_ROCK_FOUND_H

#include "m/task.h"

class DinoEventParkKasekiRockFound : public MProcess {
public:
    virtual ~DinoEventParkKasekiRockFound();
    virtual void Idle();
};

#endif
