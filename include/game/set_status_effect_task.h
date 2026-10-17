#ifndef GAME_SET_STATUS_EFFECT_TASK_H
#define GAME_SET_STATUS_EFFECT_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class SetStatusEffectTask : public MTask {
public:
    virtual ~SetStatusEffectTask();
    virtual void Execute();
};

class DinoCreatureUpdateTask : public MTask {
public:
    virtual ~DinoCreatureUpdateTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u8 mUnk14;          // 0x14
    u32 mUnk18;         // 0x18
};

class DamagedUpdate : public MTask {
public:
    virtual ~DamagedUpdate();
    virtual void Execute();
};

#endif
