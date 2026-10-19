#ifndef GAME_DINO_BATTLE_VIEW_KP_UP_DOWN_H
#define GAME_DINO_BATTLE_VIEW_KP_UP_DOWN_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class KPUpDownMotion;

// The kaseki point gains and losses shown by each side.
class DinoBattleViewKPUpDown : public MProcess {
public:
    DinoBattleViewKPUpDown();
    virtual ~DinoBattleViewKPUpDown();
    virtual void Idle();

    KPUpDownMotion *mMotions[2];    // 0x74: by side
};

class WaitPopNumberTask : public MTask {
public:
    WaitPopNumberTask() {}
    virtual ~WaitPopNumberTask();
    virtual void Execute();
};

class KPUpDownMotion : public MProcess {
public:
    KPUpDownMotion(MPoint pos) : MProcess(NULL), mPos(pos) {
        mTasks = new MParallelTaskSet(FALSE);
        mParallel.AddTail(mTasks);
    }
    virtual ~KPUpDownMotion();
    virtual void Idle();

    MParallelTaskSet *mTasks;   // 0x74
    u8 unk78[0x88 - 0x78];
    MPoint mPos;                // 0x88
};

#endif
