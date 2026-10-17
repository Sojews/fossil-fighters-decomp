#ifndef M_WIN_CONTROL_SHAPE_ANIME_H
#define M_WIN_CONTROL_SHAPE_ANIME_H

#include "m/rect_anime.h"
#include "m/window.h"

// Moves a window's shape from one rectangle to another.
class MWinControlShapeAnime : public MRectAnime {
public:
    MWinControlShapeAnime(MWinControl *win, MRect from, MRect to, u32 duration, int ease) : MRectAnime(from, to) {
        mWin = win;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MWinControlShapeAnime();
    virtual void Apply(int step);

    MWinControl *mWin;  // 0x70
};

#endif
