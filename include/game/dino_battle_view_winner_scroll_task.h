#ifndef GAME_DINO_BATTLE_VIEW_WINNER_SCROLL_TASK_H
#define GAME_DINO_BATTLE_VIEW_WINNER_SCROLL_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewWinnerScrollTask : public MProcess {
public:
    virtual ~DinoBattleViewWinnerScrollTask();
    virtual void Idle();

    u32 mUnk74;     // 0x74
    u8 mUnk78;      // 0x78
};

#endif
