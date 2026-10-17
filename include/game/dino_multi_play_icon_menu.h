#ifndef GAME_DINO_MULTI_PLAY_ICON_MENU_H
#define GAME_DINO_MULTI_PLAY_ICON_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "m/multi_play.h"

// Shows the wireless icon on one screen.
class DinoMultiPlayIconHook : public MProcess, public MDisplayHook {
public:
    DinoMultiPlayIconHook(MDisplay *display, MPoint pos) : MProcess(display), MDisplayHook(display) {
        mPos = pos;
        mUnk7C = 0;
    }
    virtual ~DinoMultiPlayIconHook();
    virtual void Idle();
    virtual void Unk08();
    virtual void Unk0C();

    MMultiPlayIcon *mUnk7C; // 0x7C
    MPoint mPos;    // 0x80
};

#endif
