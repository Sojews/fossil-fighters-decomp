#ifndef GAME_DINO_UI_PARTS_TEAM_TECH_H
#define GAME_DINO_UI_PARTS_TEAM_TECH_H

#include "m/task.h"

class DinoUiPartsTeamTech : public MProcess {
public:
    virtual ~DinoUiPartsTeamTech();
    virtual void Idle();
};

#endif
