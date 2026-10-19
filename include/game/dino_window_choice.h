// A choice shown in a window: two strings.
#ifndef GAME_DINO_WINDOW_CHOICE_H
#define GAME_DINO_WINDOW_CHOICE_H

#include <string>
#include <vector>
#include "types.h"
#include "m/geometry.h"

struct DinoWindowChoice {
    DinoWindowChoice() {}
    DinoWindowChoice(u32 id, const std::string &text, const std::string &text2);

    u32 unk00;
    std::string unk04;  // 0x04
    std::string unk10;  // 0x10
};

// An entry of a window's menu.
struct DinoCommonWindowMenuEntry {
    u32 id;             // 0x00
    MRect rect;         // 0x04
    std::string text;   // 0x14
};

#endif
