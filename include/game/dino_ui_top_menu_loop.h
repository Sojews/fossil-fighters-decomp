#ifndef GAME_DINO_UI_TOP_MENU_LOOP_H
#define GAME_DINO_UI_TOP_MENU_LOOP_H

#include "m/task.h"

class DinoUiTopMenuLoop : public MProcess {
public:
    DinoUiTopMenuLoop(u32 arg);
    virtual ~DinoUiTopMenuLoop();
    virtual void Idle();

    void (DinoUiTopMenuLoop::*mState)(); // 0x74
    u32 mUnk7C;                          // 0x7C
    u8 unk80[0xC];
};

#endif
