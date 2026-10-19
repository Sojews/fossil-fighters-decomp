#ifndef GAME_DINO_BATTLE_VIEW_3D_MOVE_H
#define GAME_DINO_BATTLE_VIEW_3D_MOVE_H

#include "m/menu_item.h"
#include "nitro/fx.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class Dino3DLoadCsv;

class DinoBattleView3DMove : public MProcess {
public:
    virtual ~DinoBattleView3DMove();
    virtual void Idle();

    void Start();
    void StateIdle();
    void Update();
    void StateEnd();

    u8 unk74[0xB0 - 0x74];
    void (DinoBattleView3DMove::*mState)();  // 0xB0
    u8 unkB8[0xC0 - 0xB8];
    Dino3DLoadCsv *mCamera; // 0xC0
    u32 unkC4;
    u8 mUnkC8;      // 0xC8
    u8 unkC9[3];
    int mUnkCC;     // 0xCC
    int mUnkD0;     // 0xD0
    VecFx32 mStep;  // 0xD4
    u8 unkE0[0xFA - 0xE0];
    u16 mUnkFA;     // 0xFA
    u8 unkFC[0x158 - 0xFC];
    int mUnk158;    // 0x158
    u8 unk15C[0x160 - 0x15C];
    u8 mUnk160;     // 0x160
};

#endif
