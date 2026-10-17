#ifndef GAME_SEND_LOCAL_MESSAGE_TASK_MENU_H
#define GAME_SEND_LOCAL_MESSAGE_TASK_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"

class DinoBattleModelField : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleModelField();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);
};

#endif
