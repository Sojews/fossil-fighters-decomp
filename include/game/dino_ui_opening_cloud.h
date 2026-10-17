#ifndef GAME_DINO_UI_OPENING_CLOUD_H
#define GAME_DINO_UI_OPENING_CLOUD_H

#include "m/view.h"
#include "m/task.h"

class DinoUiOpeningCloud : public MProcess {
public:
    virtual ~DinoUiOpeningCloud();
    virtual void Idle();

    MView *mCloud;  // 0x74
};

#endif
