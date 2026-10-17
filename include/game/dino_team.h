#ifndef GAME_DINO_TEAM_H
#define GAME_DINO_TEAM_H

#include "types.h"

// A team as the menus see it.
struct DinoTeam {
    u8 unk00[0x22];
    u8 members[5];  // 0x22
};

// TRUE if the two teams' members differ.
BOOL DinoTeamDiffers(const DinoTeam *a, const DinoTeam *b);

#endif
