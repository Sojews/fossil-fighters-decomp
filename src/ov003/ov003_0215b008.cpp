// Helpers between the field menu fraction and the revive status: team checks and text tags.
// Only some functions are decompiled so far.

#include <string>
#include "game/dino_team.h"
#include "game/dino_resource.h"

// TRUE if the team's first three members are all set and share a type, which gives the team a technique.
BOOL DinoTeamHasTech(const DinoTeam *team) {
    for (int i = 0; i < 3; i++) {
        if (team->slots[i] == 0) {
            return FALSE;
        }
    }
    u32 types = gDinoResource->GetCreatureDef(team->slots[0])->types;
    for (int i = 1; i < 3; i++) {
        types &= gDinoResource->GetCreatureDef(team->slots[i])->types;
    }
    return types != 0 ? TRUE : FALSE;
}

// TRUE if the two teams' members differ.
BOOL DinoTeamDiffers(const DinoTeam *a, const DinoTeam *b) {
    for (int i = 0; i < 5; i++) {
        if (a->slots[i] != b->slots[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// The text with every tag replaced by the value (unless the value has the tag itself).
// NONMATCHING: ours keeps an unreachable epilogue after the loop.
std::string DinoReplaceTag(std::string text, std::string tag, std::string value) {
    if (value.find(tag, 0) != std::string::npos) {
        return text;
    }
    for (;;) {
        std::string::size_type pos = text.find(tag, 0);
        if (pos == std::string::npos) {
            return text;
        }
        text = text.replace(pos, tag.size(), value);
    }
}
