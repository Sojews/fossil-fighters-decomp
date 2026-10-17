#ifndef GAME_WAIT_ACTION_INFO_BAR_TASK_H
#define GAME_WAIT_ACTION_INFO_BAR_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class WaitActionInfoBarTask : public MTask {
public:
    virtual ~WaitActionInfoBarTask();
    virtual void Execute();
};

#endif
