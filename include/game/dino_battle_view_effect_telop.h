#ifndef GAME_DINO_BATTLE_VIEW_EFFECT_TELOP_H
#define GAME_DINO_BATTLE_VIEW_EFFECT_TELOP_H

#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewEffectTelop : public MProcess {
public:
    DinoBattleViewEffectTelop(u32 unk7C, std::string text, u32 unk98, u32 unk8C, u32 unk90, u8 unk9C);
    virtual ~DinoBattleViewEffectTelop();
    virtual void Idle();

    void StateStart();
    void Load();
    void StateFinish();

    void (DinoBattleViewEffectTelop::*mState)(); // 0x74
    u32 mUnk7C;                                 // 0x7C
    std::string mUnk80;                         // 0x80
    u32 mUnk8C;                                 // 0x8C
    u32 mUnk90;                                 // 0x90
    class MExtAffinePicture *mPicture;          // 0x94
    u32 mUnk98;                                 // 0x98
    u8 mUnk9C;                                  // 0x9C: with the player's name
    class MMotion *mName;                       // 0xA0
};

class SetBGModeTask : public MTask {
public:
    SetBGModeTask(MBg *bg, int mode) {
        mBg = bg;
        mMode = mode;
    }
    virtual ~SetBGModeTask();
    virtual void Execute();

    MBg *mBg;           // 0x10
    int mMode;          // 0x14
};

class MInput;

// Waits for a telop's tasks to finish, or for the player to skip it.
class SkipTelopTask : public MProcess {
public:
    virtual ~SkipTelopTask();
    virtual void Idle();
    virtual void OnFinished() = 0;
    virtual void OnSkipped() = 0;

    MTaskSet *mTasks;   // 0x74: the telop's
    MInput *mInput;     // 0x78
    u32 mDelay;         // 0x7C: before it can be skipped
    u8 mUnk80;          // 0x80
    u8 mNoSkip;         // 0x81
};

class SkipWinTelopTask : public SkipTelopTask {
public:
    virtual ~SkipWinTelopTask();
    virtual void OnFinished();
    virtual void OnSkipped();
};

class SkipLoseTelopTask : public SkipTelopTask {
public:
    virtual ~SkipLoseTelopTask();
    virtual void OnFinished();
    virtual void OnSkipped();
};

class NopWaitTask : public MTask {
public:
    virtual ~NopWaitTask();
    virtual void Execute();
};

#endif
