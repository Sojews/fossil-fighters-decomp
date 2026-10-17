#ifndef GAME_CREATURE_MOTION_H
#define GAME_CREATURE_MOTION_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class CreatureMotion : public MMotion {
public:
    virtual ~CreatureMotion();
    virtual void Execute();
};

class NopTask : public MTask {
public:
    virtual ~NopTask();
    virtual void Execute();
};

class CreateCreatureMotionTask : public MTask {
public:
    virtual ~CreateCreatureMotionTask();
    virtual void Execute();
};

class EffectMotionTask : public MProcess {
public:
    virtual ~EffectMotionTask();
    virtual void Idle();
};

class SetCretureIconTask : public MProcess {
public:
    virtual ~SetCretureIconTask();
    virtual void Idle();
};

class SetCretureIconPriorityTask : public MTask {
public:
    virtual ~SetCretureIconPriorityTask();
    virtual void Execute();
};

class UpdateLifeBarTask : public MTask {
public:
    virtual ~UpdateLifeBarTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
};

class UpdateLifePointTask : public MProcess {
public:
    virtual ~UpdateLifePointTask();
    virtual void Idle();
};

class SetColorMaskTask : public MTask {
public:
    virtual ~SetColorMaskTask();
    virtual void Execute();
};

class ShuffleMoveTask : public MTask {
public:
    virtual ~ShuffleMoveTask();
    virtual void Execute();
};

#endif
