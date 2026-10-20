#ifndef GAME_B3D_STATUS_VIEW_TASK_H
#define GAME_B3D_STATUS_VIEW_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class B3DStatusViewTask : public MProcess {
public:
    virtual ~B3DStatusViewTask();
    virtual void Idle();

    void StateAdvance();
    void StateCheck();
    void StateShow();
    void StateFade();
    void StateNext();

    class DinoBattleView3D *mView;      // 0x74
    u32 unk78;
    u32 mUnk7C;                         // 0x7C
    u32 mUnk80;                         // 0x80: -1 when done
    u32 mUnk84;                         // 0x84
    void (B3DStatusViewTask::*mState)(); // 0x88
};

class LoadWinnerLoad : public MProcess {
public:
    virtual ~LoadWinnerLoad();
    virtual void Idle();

    void LoadModel(int i);

    class DinoBattleView3D *mView;  // 0x74
    int mSlot;                      // 0x78
    int mWinner;                    // 0x7C: the model of the creature that poses (-1 for none)
    u32 mStep;                      // 0x80
    int mUnk84;                     // 0x84: the slot model to look for (tried 0-4)
    int mTimer;                     // 0x88
};

#endif
