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

// How a camera move goes (copied into the move).
struct DinoBattleView3DMoveParams {
    u32 unk0;
    u32 unk4;
    s16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
};

class DinoBattleView3DMove : public MProcess {
public:
    DinoBattleView3DMove(const DinoBattleView3DMoveParams *params, u32 unk74, Dino3DLoadCsv *model,
                         Dino3DLoadCsv *camera, const void *unk164, u8 unk160);
    virtual ~DinoBattleView3DMove();
    virtual void Idle();

    void Start();
    void StateIdle();
    void Update();
    void StateEnd();

    u32 mUnk74;     // 0x74
    u8 unk78[0xB0 - 0x78];
    void (DinoBattleView3DMove::*mState)();  // 0xB0
    Dino3DLoadCsv *mModel;  // 0xB8
    u32 mUnkBC;     // 0xBC
    Dino3DLoadCsv *mCamera; // 0xC0
    u32 unkC4;
    u8 mUnkC8;      // 0xC8
    u8 unkC9[3];
    int mUnkCC;     // 0xCC
    int mUnkD0;     // 0xD0
    VecFx32 mStep;  // 0xD4
    u8 unkE0[0xEC - 0xE0];
    u32 mUnkEC;     // 0xEC
    u32 mUnkF0;     // 0xF0
    s16 mUnkF4;     // 0xF4
    u16 mUnkF6;     // 0xF6
    u16 mUnkF8;     // 0xF8
    u16 mUnkFA;     // 0xFA
    u16 mUnkFC;     // 0xFC
    u8 unkFE[2];
    VecFx32 mTarget;    // 0x100
    VecFx32 mDelta;     // 0x10C: per frame
    u8 unk118[0x158 - 0x118];
    int mUnk158;    // 0x158
    u8 unk15C[0x160 - 0x15C];
    u8 mUnk160;     // 0x160
    u8 unk161[3];
    const void *mUnk164;    // 0x164
};

#endif
