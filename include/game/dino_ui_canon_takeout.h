#ifndef GAME_DINO_UI_CANON_TAKEOUT_H
#define GAME_DINO_UI_CANON_TAKEOUT_H

#include "m/task.h"

class DinoUiCanonTakeout : public MProcess {
public:
    virtual ~DinoUiCanonTakeout();
    virtual void Idle();
};

#endif
