#ifndef GAME_DINO_CLEANING_IMAGE_VIEW_SCALE_ANIME_H
#define GAME_DINO_CLEANING_IMAGE_VIEW_SCALE_ANIME_H

#include "m/anime.h"

class DinoCleaningImageView;

// Zooms the cleaning image.
class DinoCleaningImageViewScaleAnime : public MFx32ValueAnime {
public:
    DinoCleaningImageViewScaleAnime(DinoCleaningImageView *view, MFx32 from, MFx32 to, int duration, int ease)
        : MFx32ValueAnime(from, to) {
        mView = view;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~DinoCleaningImageViewScaleAnime();
    virtual void Apply(int step);

    DinoCleaningImageView *mView;   // 0x3C
};

#endif
