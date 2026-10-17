#ifndef GAME_SET_TOTURIAL_FLAG_TASK_H
#define GAME_SET_TOTURIAL_FLAG_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class SetToturialFlagTask : public MTask {
public:
    virtual ~SetToturialFlagTask();
    virtual void Execute();

    struct Tutorial {
        u8 unk00[0xD0];
        u32 mFlags;     // 0xD0
    } *mTutorial;       // 0x10
    u32 mFlag;          // 0x14
};

#endif
