#ifndef GAME_BLACK_IN_TASK_H
#define GAME_BLACK_IN_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

extern "C" u32 data_0208a0b4, data_0208a0b8;

class BlackInTask : public MProcess {
public:
    BlackInTask() : MProcess(NULL) {
        mUnk74 = data_0208a0b8;
    }
    virtual ~BlackInTask();
    virtual void Idle();

    u32 mUnk74;     // 0x74
};

class BlackOutTask : public MProcess {
public:
    virtual ~BlackOutTask();
    virtual void Idle();

    u32 mUnk74;     // 0x74
};

class MValChange : public MTask {
public:
    virtual ~MValChange();
    virtual void Execute();

    int *mDst;      // 0x10
    int mValue;     // 0x14
};

#endif
