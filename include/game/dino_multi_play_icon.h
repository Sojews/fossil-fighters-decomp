#ifndef GAME_DINO_MULTI_PLAY_ICON_H
#define GAME_DINO_MULTI_PLAY_ICON_H

#include "m/task.h"

class DinoMultiPlayIcon : public MProcess {
public:
    virtual ~DinoMultiPlayIcon();
    virtual void Idle();
};

#endif
