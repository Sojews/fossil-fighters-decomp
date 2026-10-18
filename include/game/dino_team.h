#ifndef GAME_DINO_TEAM_H
#define GAME_DINO_TEAM_H

#include "game/dino_backup.h"

// TRUE if the two teams' members differ.
BOOL DinoTeamDiffers(const DinoTeam *a, const DinoTeam *b);
// TRUE if the team's first three members are all set and share a type, which gives the team a technique.
BOOL DinoTeamHasTech(const DinoTeam *team);

#endif
