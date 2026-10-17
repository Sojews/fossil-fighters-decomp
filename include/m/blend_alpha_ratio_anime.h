#ifndef M_BLEND_ALPHA_RATIO_ANIME_H
#define M_BLEND_ALPHA_RATIO_ANIME_H

#include "m/point_anime.h"
#include "m/blend.h"

// Steps a display's alpha blend weights (x: EVA, y: EVB).
class MBlendAlphaRatioAnime : public MPointAnime {
public:
    MBlendAlphaRatioAnime(MBlend *blend, MPoint from, MPoint to, u32 duration, int ease) : MPointAnime(from, to) {
        mBlend = blend;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MBlendAlphaRatioAnime();
    virtual void Apply(int step);

    MBlend *mBlend; // 0x50
};

#endif
