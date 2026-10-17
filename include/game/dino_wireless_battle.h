#ifndef GAME_DINO_WIRELESS_BATTLE_H
#define GAME_DINO_WIRELESS_BATTLE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoWirelessBattle : public MProcess, public MMessageObserver {
public:
    virtual ~DinoWirelessBattle();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u8 unkA8[0x254];
    DinoHelpDisabler mDisabler; // 0x2FC
};

#endif
