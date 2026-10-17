#ifndef GAME_DINO_MULTI_PLAY_ICON_MENU_H
#define GAME_DINO_MULTI_PLAY_ICON_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"

class DinoMultiPlayIconHook : public MProcess, public MDisplayHook {
public:
    virtual ~DinoMultiPlayIconHook();
    virtual void Idle();
    virtual void Unk08();
    virtual void Unk0C();

    u32 mUnk7C;     // 0x7C
};

#endif
