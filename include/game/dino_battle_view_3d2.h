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

    // A hit's definition: up to three motions played one after another (the last can end with a knockout or a
    // finish), each with a hit mark and extra marks.
    struct Defs {
        class MSmartOam *mOam;  // 0x00
        MTaskSet *mTasks;       // 0x04: where the marks go
        u32 mType;              // 0x08: the hit mark (0-2)
        int mMarks[4];          // 0x0C: by step, the number of extra marks
        int mAnimes[16];        // 0x1C: by step (four each), each extra mark's anime
        int mOffsets[16];       // 0x5C: and its x offset
        u32 mCount;             // 0x9C: steps
        u32 mUnkA0;             // 0xA0: (the last step loads a file)
        u32 mUnkA4;             // 0xA4: (the last step loads another file and plays SE 0x2DF)
        const char *mPaths[3];  // 0xA8
        u32 unkB4;
        u32 mUnkB8;             // 0xB8: the model name's arguments
        u32 mUnkBC[3];          // 0xBC
        u32 unkC8;
        int mSes[4];            // 0xCC: by step, played (and stopped at the next)
        int mSes2[4];           // 0xDC: by step
    };

    DinoBattleView3D *mModel;   // 0x74
    void *mOwner;               // 0x78: (a CameraWipe::Owner; 0x144: set while the hit plays)
    Dino3DLoadCsv *mCsvs[3];    // 0x7C
    class DinoBattleView3DCamera *mCamera;  // 0x88
    Defs *mDefs;                // 0x8C
    class MMotion *mMotions[5]; // 0x90: the hit mark, then the extra marks
    u32 mMotionCount;           // 0xA4
    u32 mIndex;                 // 0xA8: the step
    int mHold;                  // 0xAC: frames the last motion is held (fading out), or 0
    u32 mSide;                  // 0xB0
    int mStepFrames;            // 0xB4
    u32 unkB8[5];
    int mFrame;                 // 0xCC
    u32 mStep;                  // 0xD0
    int mMarkDelay;             // 0xD4
    u32 mShown;                 // 0xD8: the extra marks shown so far (from 1)
    u32 unkDC[0x10];
    char mName[64];             // 0x11C: the camera
    u8 mFade;                   // 0x15C: fade out (else wipe)
    u8 mShowMarks;              // 0x15D
    class MInputHub *mInputHub; // 0x160
    char mNames[3][64];         // 0x164
    const char *mFiles[3];      // 0x224
};

class DinoBattleView3DAction : public MProcess {
public:
    virtual ~DinoBattleView3DAction();
    virtual void Idle();

    // (On the background thread.)
    void LoadCsv();

    DinoBattleView3D *mModel;   // 0x74
    void *mUnk78;               // 0x78: (a CameraWipe::Owner; 0x144: set while the action plays)
    Dino3DLoadCsv *mCsv;        // 0x7C
    class DinoBattleView3DCamera *mCamera;  // 0x80
    void *mUnk84;               // 0x84
    u32 mUnk88;                 // 0x88
    u32 mUnk8C;                 // 0x8C
    u32 unk90[2];
    u16 mUnk98;                 // 0x98
    int mFrame;                 // 0x9C
    u32 mStep;                  // 0xA0
    u8 unkA4[0xE4 - 0xA4];
    u8 mUnkE4;                  // 0xE4: fade the 3D scene out (else wipe)
    class MInputHub *mInputHub; // 0xE8
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

    // (The target is the 3D view, the camera a DinoBattleView3DCamera.)
    class DinoBattleView3D *GetView() { return (class DinoBattleView3D *)mTarget; }
    class DinoBattleView3DCamera *GetCamera() { return (class DinoBattleView3DCamera *)mCamera; }

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
    void *mCamera;      // 0x148: a DinoBattleView3DCamera
    u32 mDuration;      // 0x14C
    int mUnk150[3];     // 0x150: each model's unkB0 before the group was loaded
};

#endif
