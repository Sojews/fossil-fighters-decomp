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
    u32 unk78;
    Dino3DLoadCsv *mCsvs[3];    // 0x7C
    u32 unk88;
    struct Defs {
        u8 unk00[0xA8];
        const char *mPaths[3];  // 0xA8
    } *mDefs;                   // 0x8C
    u8 unk90[0x164 - 0x90];
    char mNames[3][64];         // 0x164
    const char *mFiles[3];      // 0x224
};

class DinoBattleView3DAction : public MProcess {
public:
    virtual ~DinoBattleView3DAction();
    virtual void Idle();

    DinoBattleView3D *mModel;   // 0x74
    void *mUnk78;               // 0x78
    Dino3DLoadCsv *mCsv;        // 0x7C
    u32 unk80;
    void *mUnk84;               // 0x84
    u32 mUnk88;                 // 0x88
    u32 mUnk8C;                 // 0x8C
    u32 unk90[2];
    u16 mUnk98;                 // 0x98
};

class DinoBattleView3DMask : public MTask {
public:
    struct View;
    DinoBattleView3DMask(View *view, u32 mask) {
        mView = view;
        mMask = mask;
    }
    virtual ~DinoBattleView3DMask();
    virtual void Execute();

    struct View {
        u8 unk00[0xD0];
        Dino3DLoadCsv *mModels[3];  // 0xD0
    } *mView;               // 0x10
    u32 mMask;              // 0x14
};

extern "C" u32 data_0208a0b4, data_0208a0b8;

class DinoBattleView3DGroupFade : public MProcess {
public:
    struct Target;
    DinoBattleView3DGroupFade(void *camera, Target *target, u32 saved, u32 unk7C, int slot) : MProcess(NULL) {
        mTarget = target;
        mSaved = saved;
        mUnk7C = unk7C;
        mSlot = slot;
        mUnk84 = 0;
        mCamera = camera;
        u32 duration = data_0208a0b8;
        mDuration = duration;
        if (duration == (u32)-1) {
            mDuration = data_0208a0b4;
        }
    }
    virtual ~DinoBattleView3DGroupFade();
    virtual void Idle();

    struct Target {
        u8 unk00[0xD0];
        Dino3DLoadCsv *mModels[3];  // 0xD0
        u8 unkDC[0x13C - 0xDC];
        u32 mUnk13C;    // 0x13C
    } *mTarget;         // 0x74
    u32 mSaved;         // 0x78
    u32 mUnk7C;         // 0x7C
    int mSlot;          // 0x80
    u32 mUnk84;         // 0x84
    char mNames[3][64]; // 0x88
    void *mCamera;      // 0x148
    u32 mDuration;      // 0x14C
    u32 unk150[3];
};

#endif
