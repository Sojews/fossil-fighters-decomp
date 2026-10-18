#ifndef GAME_DINO_UI_PARTS_TEAM_INFO_H
#define GAME_DINO_UI_PARTS_TEAM_INFO_H

#include "m/task.h"

class DinoUiPartsTeamInfo : public MProcess {
public:
    DinoUiPartsTeamInfo();
    virtual ~DinoUiPartsTeamInfo();
    virtual void Idle();

    // Shows the saved team's members.
    void SetTeam(u32 team);

    u8 unk74[0xE0 - 0x74];
};

#endif
