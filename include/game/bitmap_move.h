#ifndef GAME_BITMAP_MOVE_H
#define GAME_BITMAP_MOVE_H

#include "m/screen.h"
#include "m/point_anime.h"

class BitmapMove : public MPointAnime {
public:
    BitmapMove(MScreen *screen, MPoint from, MPoint to, u32 frames) : MPointAnime(from, to) {
        mScreen = screen;
        SetDuration(frames);
        mEase = -3;
    }
    virtual ~BitmapMove();
    virtual void Apply(int step);

    MScreen *mScreen;   // 0x50: scrolled by the anime
};

#endif
