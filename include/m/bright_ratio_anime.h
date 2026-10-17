#ifndef M_BRIGHT_RATIO_ANIME_H
#define M_BRIGHT_RATIO_ANIME_H

#include "m/value_anime.h"

class MBright;

// Steps the brightness factor.
class MBrightRatioAnime : public MValueAnime {
public:
    MBrightRatioAnime(MBright *bright, int from, int to, int duration, int ease) : MValueAnime(from, to) {
        mBright = bright;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MBrightRatioAnime();
    virtual void Apply(int step);

    MBright *mBright;   // 0x40
};

#endif
