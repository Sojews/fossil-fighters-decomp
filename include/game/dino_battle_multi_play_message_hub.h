#ifndef GAME_DINO_BATTLE_MULTI_PLAY_MESSAGE_HUB_H
#define GAME_DINO_BATTLE_MULTI_PLAY_MESSAGE_HUB_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleMultiPlayMessageHub : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleMultiPlayMessageHub();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void *mUnkA8;   // 0xA8
};

class DinoBattleMultiPlayMessageReceiver : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleMultiPlayMessageReceiver();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    MMessageSubject *mSubject;  // 0xA8
};

#endif
