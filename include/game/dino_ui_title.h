#ifndef GAME_DINO_UI_TITLE_H
#define GAME_DINO_UI_TITLE_H

#include "m/task.h"

class DinoUiTitle : public MProcess {
public:
    DinoUiTitle(s32 *result);
    virtual ~DinoUiTitle();
    virtual void Idle();

    void (DinoUiTitle::*mState)(); // 0x74
    s32 *mResult;                  // 0x7C: what the player chose
    u8 unk80[0x28];
};

#endif
