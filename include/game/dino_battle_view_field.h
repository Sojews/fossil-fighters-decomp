#ifndef GAME_DINO_BATTLE_VIEW_FIELD_H
#define GAME_DINO_BATTLE_VIEW_FIELD_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleView;
class MSmartOam;

class DinoBattleViewField : public MProcess, public MMessageObserver {
public:
    DinoBattleViewField(DinoBattleView *view, MSmartOam *oam, u32 tutorial);
    virtual ~DinoBattleViewField();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u8 unkA8[0x16C - 0xA8];     // (laid out in DinoBattleViewUnkE0, dino_battle_view_class.h)
};

#endif
