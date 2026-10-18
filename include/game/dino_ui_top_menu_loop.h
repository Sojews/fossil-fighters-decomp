#ifndef GAME_DINO_UI_TOP_MENU_LOOP_H
#define GAME_DINO_UI_TOP_MENU_LOOP_H

#include "m/task.h"

class DinoUiTopMenuLoop : public MProcess {
public:
    DinoUiTopMenuLoop(int *result);
    virtual ~DinoUiTopMenuLoop();
    virtual void Idle();

    void StateStart();
    void StateTopMenu();
    void StateTopMenuEnd();
    void StateMultiplay();
    void StateMultiplayEnd();
    void StateExchange();

    void (DinoUiTopMenuLoop::*mState)(); // 0x74
    int *mResult;                        // 0x7C: 2 or 3 to start in multiplay (and what it ends with)
    u32 unk80;
    int mChoice;                         // 0x84: the top menu's
    int mMode;                           // 0x88: the multiplay menu's
};

#endif
