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

    u8 unk74[0x14];
    void (B3DStatusViewTask::*mState)(); // 0x88
};

class LoadWinnerLoad : public MProcess {
public:
    virtual ~LoadWinnerLoad();
    virtual void Idle();
};

#endif
