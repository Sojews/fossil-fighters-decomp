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
#include "m/fx32.h"

class MConsole;
class MMotion;

class MSmartOam;

class DinoBattleViewFocusLCDFrame : public MProcess {
public:
    DinoBattleViewFocusLCDFrame(u32 unk74);
    virtual ~DinoBattleViewFocusLCDFrame();
    virtual void Idle();

    u32 mUnk74;                 // 0x74
    MSmartOam *mOam;            // 0x78: on the main screen, shifted a screen up
    MParallelTaskSet *mTasks;   // 0x7C
    u8 unk80[0xA0 - 0x80];
};

class DBViewFocusLCDFrameTask : public MTask {
public:
    DBViewFocusLCDFrameTask(u32 a, u8 b);
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

    u32 mFace;      // 0x74: the face shown
    int mSide;      // 0x78: the side talking (0 left, 1 right)
    u32 unk7C;
    int mTimer;     // 0x80: frames until the console goes on
    u32 mUnk84;     // 0x84: extra waits before the window counts as done
    u8 unk88[0xC0 - 0x88];
    MTaskSet *mTasksC0;     // 0xC0
    MTaskSet *mTasksC4;     // 0xC4
    MView *mFrame;          // 0xC8: the window frame
    MMotion *mFaceMotion;   // 0xCC
    MConsole *mConsole;     // 0xD0
    u32 unkD4[3];
    MFx32 mRate;            // 0xE0: the console's typing rate
    u8 unkE4[0x124 - 0xE4];
    int mState;             // 0x124: 2 while closing
};

#endif
