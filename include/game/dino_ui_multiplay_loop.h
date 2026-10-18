#ifndef GAME_DINO_UI_MULTIPLAY_LOOP_H
#define GAME_DINO_UI_MULTIPLAY_LOOP_H

#include "m/task.h"

class DinoUiMultiplayLoop : public MProcess {
public:
    DinoUiMultiplayLoop(int *result);
    virtual ~DinoUiMultiplayLoop();
    virtual void Idle();

    void StateMenu();
    void StateMatch();
    void StateMatchEnd();

    void (DinoUiMultiplayLoop::*mState)(); // 0x74
    int *mResult;       // 0x7C: 2 to start on the wireless match; 1 when it ends that way
    int mChoice;        // 0x80: the menu's
    u32 mMatchResult;   // 0x84
    u8 mUnk88;          // 0x88
};

#endif
