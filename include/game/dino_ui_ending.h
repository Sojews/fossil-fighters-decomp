#ifndef GAME_DINO_UI_ENDING_H
#define GAME_DINO_UI_ENDING_H

#include "m/task.h"
#include "m/picture.h"

// The ending sequence.
class DinoUiEnding : public MProcess {
public:
    DinoUiEnding();
    virtual ~DinoUiEnding();
    virtual void Idle();

    void (DinoUiEnding::*mState)(); // 0x74
    u8 unk7C[0x18];
    MPictureFile mPicture;          // 0x94
};

#endif
