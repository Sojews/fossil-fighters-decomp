#ifndef GAME_SEND_LOCAL_MESSAGE_TASK_MENU_H
#define GAME_SEND_LOCAL_MESSAGE_TASK_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"

class DinoBattleModel;
class DinoCommonLive;

class DinoBattle;

class DinoBattleModelField : public MProcess, public MMessageObserver {
public:
    DinoBattleModelField();
    virtual ~DinoBattleModelField();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    // Takes the battle's controllers, model and live commentary.
    void SetBattle(DinoBattle *battle);

    MMessageObserver *mControllers[2];  // 0xA8
    DinoBattleModel *mModel;    // 0xB0
    MMessageObserver *mUnkB4;   // 0xB4
    DinoBattle *mBattle;        // 0xB8
    DinoCommonLive *mLive;      // 0xBC: the commentary
    u32 unkC0;
    u32 mUnkC4;                 // 0xC4
    u32 unkC8[2];
    u32 mUnkD0;                 // 0xD0
};

#endif
