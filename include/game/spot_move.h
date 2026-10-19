#ifndef GAME_SPOT_MOVE_H
#define GAME_SPOT_MOVE_H

#include "m/task.h"
#include "m/geometry.h"

class MRasterWindow;
class MMotion;
class MValueAnime;
class MPointAnime;
class DrawFont;

// Shines the spotlight on a place on the island map, then brings its name in.
class SpotMove : public MProcess {
public:
    SpotMove(MRasterWindow *window, MPoint center, u32 frames, MMotion *name, MMotion *mark, DrawFont *font,
             BOOL flip)
        : MProcess(NULL) {
        mWindow = window;
        mCenter = center;
        mFrames = frames;
        mFrame = 0;
        mName = name;
        mMark = mark;
        mFont = font;
        mFlip = flip;
    }
    virtual ~SpotMove();
    virtual void Idle();

    MRasterWindow *mWindow;  // 0x74
    MPoint mCenter;          // 0x78
    u32 mFrames;             // 0x80: the scroll's length
    u32 mFrame;              // 0x84
    MMotion *mName;          // 0x88
    MMotion *mMark;          // 0x8C
    MValueAnime *mRadius;    // 0x90
    MPointAnime *mNameMove;  // 0x94
    MPointAnime *mMarkMove;  // 0x98
    DrawFont *mFont;         // 0x9C
    BOOL mFlip;              // 0xA0: the name comes in from the other side
};

#endif
