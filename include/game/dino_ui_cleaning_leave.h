#ifndef GAME_DINO_UI_CLEANING_LEAVE_H
#define GAME_DINO_UI_CLEANING_LEAVE_H

#include <vector>

#include "m/task.h"

class DinoUiCleaningLeave : public MProcess {
public:
    DinoUiCleaningLeave(std::vector<void *> *chosen);
    virtual ~DinoUiCleaningLeave();
    virtual void Idle();
};

#endif
