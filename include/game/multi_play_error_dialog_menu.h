#ifndef GAME_MULTI_PLAY_ERROR_DIALOG_MENU_H
#define GAME_MULTI_PLAY_ERROR_DIALOG_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MMultiPlay;

class DinoMpGame : public MProcess {
public:
    DinoMpGame(MMultiPlay *multiPlay);
    virtual ~DinoMpGame();
    virtual void Idle();

    void (DinoMpGame::*mState)(); // 0x74
    void StateFinish();

    MMultiPlay *mMultiPlay;     // 0x7C
    u32 mUnk80;
    u8 mFailed;                 // 0x84: shows the error dialog
    DinoHelpDisabler mDisabler; // 0x88
};

#endif
