#ifndef M_BLEND_FADE_RATIO_ANIME_H
#define M_BLEND_FADE_RATIO_ANIME_H

#include "m/value_anime.h"
#include "m/blend.h"

// Steps a display's brightness fade.
class MBlendFadeRatioAnime : public MValueAnime {
public:
    MBlendFadeRatioAnime(MBlend *blend, int from, int to, u32 duration, int ease) : MValueAnime(from, to) {
        mBlend = blend;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MBlendFadeRatioAnime();
    virtual void Apply(int step);

    MBlend *mBlend; // 0x40
};

#endif
