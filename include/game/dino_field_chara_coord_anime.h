// Moves a field character between two coordinates.
#ifndef GAME_DINO_FIELD_CHARA_COORD_ANIME_H
#define GAME_DINO_FIELD_CHARA_COORD_ANIME_H

#include "m/anime.h"
#include "game/dino_field.h"

class DinoFieldCharaCoordAnime : public MFx32PointAnime {
public:
    virtual ~DinoFieldCharaCoordAnime();
    virtual void Execute();

    DinoFieldChara *mChara; // 0x48
    u8 mUnk4C;              // 0x4C
};

#endif
