#ifndef GAME_DINO_UI_ENDING_H
#define GAME_DINO_UI_ENDING_H

#include "m/task.h"
#include "m/picture.h"
#include "m/bg.h"

// The ending sequence.
class DinoUiEnding : public MProcess {
public:
    DinoUiEnding();
    virtual ~DinoUiEnding();
    virtual void Idle();

    void StateStart();
    void StateEnd();

    // A picture on each screen.
    struct Screen {
        MPalUnit *palette;
        MIndexBitmap *bitmap;
        MIndexBitmap *bitmap2;
    };

    void (DinoUiEnding::*mState)(); // 0x74
    Screen mScreens[2];             // 0x7C
    MPictureFile mPicture;          // 0x94
};

#endif
