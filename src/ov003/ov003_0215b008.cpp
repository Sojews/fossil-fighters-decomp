// Helpers between the field menu fraction and the revive status: team checks and text tags.
// Only some functions are decompiled so far.

#include <string>
#include "game/dino_team.h"
#include "game/dino_resource.h"
#include "m/blend.h"
#include "m/display.h"
#include "m/task.h"

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

#pragma auto_inline off

// Dims the screen behind a menu.
extern "C" void func_ov003_0215b16c() {
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->Reset();
    blend->SetMode(3);
    blend->SetBrightness(8);
    blend->SetAlpha(16, 0);
    blend->SetTarget1(0x10);
    blend->SetTarget2(0x3F);
}

// (Emits the out-of-line copies of inline functions the game emits in this file.)
// (Taking its address emits it, with its own calls inlined.)
const DinoKasekiDefs *(MResource<DinoKasekiDefs>::*DinoKasekiDefsGetAnchor())() {
    return &MResource<DinoKasekiDefs>::Get;
}
#pragma dont_inline on
const DinoKasekiDef *DinoKasekiDefsAnchor(const DinoKasekiDefs *defs, u32 id) {
    return defs->GetDef(id);
}
#pragma dont_inline reset
