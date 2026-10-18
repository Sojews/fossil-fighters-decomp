#ifndef GAME_DINO_UI_ATM_H
#define GAME_DINO_UI_ATM_H

#include "m/task.h"

class DinoUiPartsTeamInfo;

class DinoUiATM : public MProcess {
public:
    DinoUiATM(u8 arg);
    virtual ~DinoUiATM();
    virtual void Idle();

    void StateStart();
    void StateSetup();
    void StateMenu();
    void StateResult();
    void StateNamed();

    void (DinoUiATM::*mState)();    // 0x74
    u8 mUnk7C;                      // 0x7C
    u32 mResult;                    // 0x80: the menu's
    MTaskSet *mTasks;               // 0x84
    DinoUiPartsTeamInfo *mTeamInfo; // 0x88
};

#endif
