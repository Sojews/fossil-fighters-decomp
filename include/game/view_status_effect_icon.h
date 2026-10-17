#ifndef GAME_VIEW_STATUS_EFFECT_ICON_H
#define GAME_VIEW_STATUS_EFFECT_ICON_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewStatusEffectIcon : public MProcess {
public:
    virtual ~DinoBattleViewStatusEffectIcon();
    virtual void Idle();
};

class ViewStatusEffectIcon : public MProcess {
public:
    virtual ~ViewStatusEffectIcon();
    virtual void Idle();

    class PlayEraseMotionTask : public MTask {
    public:
        virtual ~PlayEraseMotionTask();
        virtual void Execute();
    };

    class SetPriorityTask : public MTask {
    public:
        virtual ~SetPriorityTask();
        virtual void Execute();
    };
};

class ViewStatusEffectIconWaitStop : public MTask {
public:
    virtual ~ViewStatusEffectIconWaitStop();
    virtual void Execute();
};

class ViewStatusEffectIconLoadMotionTask : public MTask {
public:
    virtual ~ViewStatusEffectIconLoadMotionTask();
    virtual void Execute();
};

class ViewStatusEffectIconShowTask : public MTask {
public:
    virtual ~ViewStatusEffectIconShowTask();
    virtual void Execute();
};

#endif
