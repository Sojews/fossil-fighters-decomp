#ifndef GAME_DINO_BATTLE2_H
#define GAME_DINO_BATTLE2_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewFocusLCDFrame : public MProcess {
public:
    virtual ~DinoBattleViewFocusLCDFrame();
    virtual void Idle();
};

class DBViewFocusLCDFrameTask : public MTask {
public:
    virtual ~DBViewFocusLCDFrameTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u8 mUnk14;          // 0x14
};

class DinoBattleTalkWindow : public MProcess {
public:
    virtual ~DinoBattleTalkWindow();
    virtual void Idle();

    void SetUnk84(u32 value);

    u32 unk74[4];
    u32 mUnk84;     // 0x84
};

#endif
