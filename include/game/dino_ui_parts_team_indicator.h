#ifndef GAME_DINO_UI_PARTS_TEAM_INDICATOR_H
#define GAME_DINO_UI_PARTS_TEAM_INDICATOR_H

#include "m/task.h"
#include "m/font.h"
#include "m/view.h"

class DinoUiPartsTeamIndicator : public MProcess {
public:
    virtual ~DinoUiPartsTeamIndicator();
    virtual void Idle();

    MFont *mFont;   // 0x74
    MView *mView;   // 0x78
};

#endif
