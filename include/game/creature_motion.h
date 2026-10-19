#ifndef GAME_CREATURE_MOTION_H
#define GAME_CREATURE_MOTION_H

#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/fx32.h"

class DinoBattleViewCreature;

class DinoBattleViewLifebar;

class CreatureMotion : public MMotion {
public:
    CreatureMotion(const char *path, MSmartOam *oam, int arg);
    virtual ~CreatureMotion();
    virtual void Execute();
    virtual MRect GetFrame();
    virtual void Unk44(int arg);

    void SetArg(int arg, BOOL flag);

    int mArg;           // 0x7C
    u32 mUnk80;         // 0x80
    u32 mUnk84;         // 0x84
    u16 mUnk88;         // 0x88
    MSmartOam *mOam;    // 0x8C
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

    CreatureMotion **mSlot;         // 0x10
    DinoBattleViewCreature *mView;  // 0x14
    std::string mPath;              // 0x18
    MSmartOam *mOam;                // 0x24
    int mArg;                       // 0x28
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

    MMotion **mMotion;  // 0x10
    int mPriority;      // 0x14
    int mAnchor;        // 0x18
};

class UpdateLifeBarTask : public MTask {
public:
    virtual ~UpdateLifeBarTask();
    virtual void Execute();

    DinoBattleViewLifebar *mLifebar;    // 0x10
    int mLife;                          // 0x14
    int mMaxLife;                       // 0x18
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

    struct Target {
        u8 unk00[0x144];
        u32 mColorMask; // 0x144
    } *mTarget;         // 0x10
    u32 mColorMask;     // 0x14
};

// Moves a creature's motion from one point to another along a curve.
class ShuffleMoveTask : public MTask {
public:
    ShuffleMoveTask(int arg, MMotion *motion, MPoint from, MPoint to)
        : mArg(arg), mMotion(motion), mFrom(from), mTo(to), mDuration(120), mFrame(0), mAngle(0), mUnk34(0) {
        mAngle = FX_Atan2Idx(from.y << FX32_SHIFT, from.x << FX32_SHIFT);
    }
    virtual ~ShuffleMoveTask();
    virtual void Execute();

    int mArg;           // 0x10
    MMotion *mMotion;   // 0x14
    MPoint mFrom;       // 0x18
    MPoint mTo;         // 0x20
    int mDuration;      // 0x28
    int mFrame;         // 0x2C
    u16 mAngle;         // 0x30
    int mUnk34;         // 0x34
};

#endif
