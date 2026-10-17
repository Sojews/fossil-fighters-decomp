#ifndef GAME_DINO_UI_ENDING_H
#define GAME_DINO_UI_ENDING_H

#include "m/task.h"

// The ending sequence.
class DinoUiEnding : public MProcess {
public:
    DinoUiEnding();
    virtual ~DinoUiEnding();
    virtual void Idle();

    void (DinoUiEnding::*mState)(); // 0x74
    u8 unk7C[0x18];
    u8 unk94[0x34];                 // 0x94: an MPictureFile
};

#endif
