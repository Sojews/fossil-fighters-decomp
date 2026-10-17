#ifndef GAME_SET_VAR_TASK_H
#define GAME_SET_VAR_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/console_filter.h"

class SetVarTask : public MTask {
public:
    virtual ~SetVarTask();
    virtual void Execute();

    int *mDst;      // 0x10
    int mValue;     // 0x14
};

class DinoBattleLiveConsoleFilter : public MConsoleFilter {
public:
    virtual ~DinoBattleLiveConsoleFilter();
};

#endif
