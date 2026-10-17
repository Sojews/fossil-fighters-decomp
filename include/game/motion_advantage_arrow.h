#ifndef GAME_MOTION_ADVANTAGE_ARROW_H
#define GAME_MOTION_ADVANTAGE_ARROW_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MotionAdvantageArrow : public MMotion {
public:
    virtual ~MotionAdvantageArrow();
    virtual void Unk44(int arg);

    u8 mUnk7C;  // 0x7C
};

class DinoBattleViewAdvantageArrow : public MProcess {
public:
    virtual ~DinoBattleViewAdvantageArrow();
    virtual void Idle();
};

class DinoBattleViewTutorialMotion : public MProcess {
public:
    virtual ~DinoBattleViewTutorialMotion();
    virtual void Idle();
};

class DinoBattleViewLCDFader : public MProcess {
public:
    virtual ~DinoBattleViewLCDFader();
    virtual void Idle();
};

class TutorialWindowCloseTask : public MTask {
public:
    virtual ~TutorialWindowCloseTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
};

class WaitUpdateSupportEffectAnimeTask : public MTask {
public:
    virtual ~WaitUpdateSupportEffectAnimeTask();
    virtual void Execute();
};

class DinoBattleViewTutorialRect : public MProcess {
public:
    virtual ~DinoBattleViewTutorialRect();
    virtual void Idle();
};

class SetTutorialMotionArrorTask : public MTask {
public:
    virtual ~SetTutorialMotionArrorTask();
    virtual void Execute();
};

class SetTutorialMotionRectTask : public MTask {
public:
    virtual ~SetTutorialMotionRectTask();
    virtual void Execute();
};

#endif
