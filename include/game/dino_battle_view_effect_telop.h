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

    void (DinoBattleViewEffectTelop::*mState)(); // 0x74
    u32 mUnk7C;                                 // 0x7C
    std::string mUnk80;                         // 0x80
    u32 mUnk8C;                                 // 0x8C
    u32 mUnk90;                                 // 0x90
    u32 unk94;
    u32 mUnk98;                                 // 0x98
    u8 mUnk9C;                                  // 0x9C
};

class SetBGModeTask : public MTask {
public:
    virtual ~SetBGModeTask();
    virtual void Execute();

    MBg *mBg;           // 0x10
    int mMode;          // 0x14
};

class SkipTelopTask : public MProcess {
public:
    virtual ~SkipTelopTask();
    virtual void Idle();
};

class SkipWinTelopTask : public SkipTelopTask {
public:
    virtual ~SkipWinTelopTask();
};

class SkipLoseTelopTask : public SkipTelopTask {
public:
    virtual ~SkipLoseTelopTask();
};

class NopWaitTask : public MTask {
public:
    virtual ~NopWaitTask();
    virtual void Execute();
};

#endif
