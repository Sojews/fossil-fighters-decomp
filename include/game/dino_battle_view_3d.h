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

    void GetUnk19C(u32 *a, u32 *b);
    void SetUnk19C(u32 value);
    u32 GetUnkB0();

    u32 unkA8[2];
    u32 mUnkB0;                             // 0xB0
    u32 unkB4[3];
    void *mUnkC0;                           // 0xC0
    void (DinoBattleView3D::*mState)();     // 0xC4
    u8 unkCC[0x19C - 0xCC];
    u32 mUnk19C;                            // 0x19C
    u32 mUnk1A0;                            // 0x1A0
};

#endif
