// The battle scene, its auto demo and the debug battle select.
#ifndef GAME_DINO_BATTLE_H
#define GAME_DINO_BATTLE_H

#include <string>
#include "m/task.h"
#include "game/dino_battle_controller.h"

// One side of a battle (the game keeps two).
struct DinoBattleSide {
    std::string mName;  // 0x00
    u8 unk0C[0x80];
};

// What a battle is set up with.
struct DinoBattleParams {
    u32 unk00;                  // 0x00
    u32 unk04;                  // 0x04
    u32 unk08;                  // 0x08
    u32 unk0C;                  // 0x0C
    u32 unk10;                  // 0x10
    u32 unk14;                  // 0x14
    u32 unk18;                  // 0x18
    u8 unk1C;                   // 0x1C
    u32 unk20[4];               // 0x20
    DinoBattleSide sides[2];    // 0x30
    u32 unk148[8];              // 0x148
};

class DinoBattle : public MProcess {
public:
    virtual ~DinoBattle();
    virtual void Idle();

    u32 GetUnk1F4(int i);
    u32 GetUnk1FC();
    MMessageObserver *GetUnk200();
    void SetUnk1DC(u8 value);
    void SetUnk1E0(u8 value);

    u32 mUnk74[12];             // 0x74
    DinoBattleSide mSides[2];   // 0xA4
    u32 mUnk1BC[8];             // 0x1BC
    u8 *mUnk1DC;                // 0x1DC: set when the battle ends (if given)
    u8 *mUnk1E0;                // 0x1E0
    u32 mUnk1E4[2];             // 0x1E4
    void (DinoBattle::*mState)();   // 0x1EC
    u32 mUnk1F4[2];             // 0x1F4
    u32 mUnk1FC;                // 0x1FC
    DinoBattleController *mUnk200;  // 0x200
    u32 mUnk204;                // 0x204
    MTask *mUnk208;             // 0x208
};

class DinoBattleAutoDemo : public MProcess {
public:
    DinoBattleAutoDemo(MTask *unk7C, u8 *result);
    virtual ~DinoBattleAutoDemo();
    virtual void Idle();

    void (DinoBattleAutoDemo::*mState)(); // 0x74
    MTask *mUnk7C;      // 0x7C
    u32 unk80;
    u8 *mResult;        // 0x84
};

class DinoBattleDebugSelect : public MProcess {
public:
    DinoBattleDebugSelect(const DinoBattleParams &params, u8 *result);
    virtual ~DinoBattleDebugSelect();
    virtual void Idle();

    void StateLevelSelect();

    u8 mCancelled;              // 0x74: set by the selects
    DinoBattleParams mParams;   // 0x78
    u32 mUnk1E0;                // 0x1E0
    void (DinoBattleDebugSelect::*mState)(); // 0x1E4
};

#endif
