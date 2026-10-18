// Moves a field character between two coordinates.
#ifndef GAME_DINO_FIELD_CHARA_COORD_ANIME_H
#define GAME_DINO_FIELD_CHARA_COORD_ANIME_H

#include "m/anime.h"
#include "game/dino_field.h"

class DinoFieldCharaCoordAnime : public MFx32PointAnime {
public:
    DinoFieldCharaCoordAnime(DinoFieldChara *chara, MFx32Point to, u32 frames, int ease, u8 silent, u32 se);
    virtual ~DinoFieldCharaCoordAnime();
    virtual void Execute();
    virtual void Apply(int step);

    DinoFieldChara *mChara; // 0x48
    u8 mUnk4C;              // 0x4C
    u32 mUnk50;             // 0x50
    u32 mUnk54;             // 0x54
};

#endif
