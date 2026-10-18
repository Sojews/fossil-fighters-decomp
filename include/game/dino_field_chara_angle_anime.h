#ifndef GAME_DINO_FIELD_CHARA_ANGLE_ANIME_H
#define GAME_DINO_FIELD_CHARA_ANGLE_ANIME_H

#include "m/anime.h"

class DinoFieldChara;

// Turns a field character from one direction to another.
class DinoFieldCharaAngleAnime : public MAnime {
public:
    DinoFieldCharaAngleAnime(DinoFieldChara *chara, u32 angle, u32 frames, int ease);
    virtual ~DinoFieldCharaAngleAnime();
    virtual void Apply(int step);

    DinoFieldChara *mChara; // 0x30
    u32 mFrom;              // 0x34
    u32 mTo;                // 0x38
};

#endif
