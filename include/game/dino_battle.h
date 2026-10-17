// The battle scene, its auto demo and the debug battle select.
#ifndef GAME_DINO_BATTLE_H
#define GAME_DINO_BATTLE_H

#include <string>
#include "m/task.h"

// One side of a battle (the game keeps two).
struct DinoBattleSide {
    std::string mName;  // 0x00
    u8 unk0C[0x80];
};

class DinoBattle : public MProcess {
public:
    virtual ~DinoBattle();
    virtual void Idle();

    u32 mUnk74[12];             // 0x74
    DinoBattleSide mSides[2];   // 0xA4
    u32 mUnk1BC[12];            // 0x1BC
    void (DinoBattle::*mState)();   // 0x1EC
    u32 mUnk1F4[5];             // 0x1F4
    MTask *mUnk208;             // 0x208
};

class DinoBattleAutoDemo : public MProcess {
public:
    virtual ~DinoBattleAutoDemo();
    virtual void Idle();

    void (DinoBattleAutoDemo::*mState)(); // 0x74
    MTask *mUnk7C;      // 0x7C
};

class DinoBattleDebugSelect : public MProcess {
public:
    virtual ~DinoBattleDebugSelect();
    virtual void Idle();

    u32 mUnk74[13];             // 0x74
    DinoBattleSide mSides[2];   // 0xA8
    u32 mUnk1C0[9];             // 0x1C0
    void (DinoBattleDebugSelect::*mState)(); // 0x1E4
};

#endif
