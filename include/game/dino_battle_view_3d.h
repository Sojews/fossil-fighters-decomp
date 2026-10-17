#ifndef GAME_DINO_BATTLE_VIEW_3D_H
#define GAME_DINO_BATTLE_VIEW_3D_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleView3D : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleView3D();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u8 unkA8[0x18];
    void *mUnkC0;                           // 0xC0
    void (DinoBattleView3D::*mState)();     // 0xC4
};

#endif
