#ifndef M_BLEND_ALPHA_RATIO_ANIME_H
#define M_BLEND_ALPHA_RATIO_ANIME_H

#include "m/point_anime.h"
#include "m/blend.h"

// Steps a display's alpha blend weights (x: EVA, y: EVB).
class MBlendAlphaRatioAnime : public MPointAnime {
public:
    MBlendAlphaRatioAnime(MBlend *blend, int evaFrom, int evaTo, int evbFrom, int evbTo, u32 duration, int ease)
        : MPointAnime(MPoint(evaFrom, evbFrom), MPoint(evaTo, evbTo)) {
        mBlend = blend;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MBlendAlphaRatioAnime();
    virtual void Apply(int step);

    MBlend *mBlend; // 0x50
};

#endif
