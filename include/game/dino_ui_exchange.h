#ifndef GAME_DINO_UI_EXCHANGE_H
#define GAME_DINO_UI_EXCHANGE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiExchange : public MProcess, public MMessageObserver {
public:
    virtual ~DinoUiExchange();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);
};

#endif
