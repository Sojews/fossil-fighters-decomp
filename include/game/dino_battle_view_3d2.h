#ifndef GAME_DINO_BATTLE_VIEW_3D2_H
#define GAME_DINO_BATTLE_VIEW_3D2_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class Dino3DLoadCsv;

class DinoBattleView3D;

class DinoBattleView3DDamage : public MProcess {
public:
    virtual ~DinoBattleView3DDamage();
    virtual void Idle();

    DinoBattleView3D *mModel;   // 0x74
};

class DinoBattleView3DAction : public MProcess {
public:
    virtual ~DinoBattleView3DAction();
    virtual void Idle();

    DinoBattleView3D *mModel;   // 0x74
};

class DinoBattleView3DMask : public MTask {
public:
    virtual ~DinoBattleView3DMask();
    virtual void Execute();

    struct View {
        u8 unk00[0xD0];
        Dino3DLoadCsv *mModels[3];  // 0xD0
    } *mView;               // 0x10
    u32 mMask;              // 0x14
};

class DinoBattleView3DGroupFade : public MProcess {
public:
    virtual ~DinoBattleView3DGroupFade();
    virtual void Idle();

    struct Target {
        u8 unk00[0x13C];
        u32 mUnk13C;    // 0x13C
    } *mTarget;         // 0x74
    u32 mSaved;         // 0x78
};

#endif
