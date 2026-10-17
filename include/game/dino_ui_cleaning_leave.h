#ifndef GAME_DINO_UI_CLEANING_LEAVE_H
#define GAME_DINO_UI_CLEANING_LEAVE_H

#include "m/task.h"

class DinoUiCleaningLeave : public MProcess {
public:
    virtual ~DinoUiCleaningLeave();
    virtual void Idle();
};

#endif
