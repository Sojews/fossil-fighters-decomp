#ifndef GAME_BATTLE_SELECT_H
#define GAME_BATTLE_SELECT_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/seed_generator.h"
#include <vector>

// A debug process that plays battles automatically.
class DebugAutoBattle : public MProcess {
public:
    DebugAutoBattle();
    virtual ~DebugAutoBattle();
    virtual void Idle();

    MRandom *mRandom;           // 0x74
    std::vector<u32> mUnk78;    // 0x78
};

class BattleSelect : public MProcess {
public:
    virtual ~BattleSelect();
    virtual void Idle();
};

class UseCreatureSelect : public MProcess {
public:
    UseCreatureSelect(void *params, u32 unk7C, u32 unk80, u8 *cancelled);
    virtual ~UseCreatureSelect();
    virtual void Idle();

    class MIndicator *CreateIndicator(class MSmartOam *oam, class MFont *font, MPoint size, MPoint pos);
    void CreateTexts(class MSmartOam *oam, class MFont *font);
    void ShowCreature(u32 creature);

    void (UseCreatureSelect::*mState)(); // 0x74
    u32 mUnk7C;             // 0x7C
    u32 mUnk80;             // 0x80
    u32 unk84;
    u32 mUnk88;             // 0x88
    u8 unk8C[0xFC - 0x8C];
    class MIndicator *mTitle;       // 0xFC
    u32 unk100;
    class MIndicator *mNames[5];    // 0x104
    class MIndicator *mValues[5];   // 0x118
    void *mParams;          // 0x12C
    u8 *mCancelled;         // 0x130
};

class PlayerLevelSelect : public MProcess {
public:
    PlayerLevelSelect(u32 *level, BOOL unk7C, u32 unk80);
    virtual ~PlayerLevelSelect();
    virtual void Idle();

    void (PlayerLevelSelect::*mState)(); // 0x74
    BOOL mUnk7C;            // 0x7C
    u32 mUnk80;             // 0x80
    u32 unk84[6];
    u32 mUnk9C;             // 0x9C
    u32 unkA0[2];
    u32 mUnkA8;             // 0xA8
    u32 mUnkAC;             // 0xAC
    u32 unkB0[8];
    u32 *mLevel;            // 0xD0
};

class IncValTask : public MTask {
public:
    virtual ~IncValTask();
    virtual void Execute();

    int *mValue;    // 0x10
};

class TimeCountTask : public MProcess {
public:
    virtual ~TimeCountTask();
    virtual void Idle();
};

#endif
