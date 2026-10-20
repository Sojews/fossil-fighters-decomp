// The battle scene, its auto demo and the debug battle select.
#ifndef GAME_DINO_BATTLE_H
#define GAME_DINO_BATTLE_H

#include <string>
#include "m/task.h"
#include "game/dino_battle_controller.h"

// One side of a battle (the game keeps two).
struct DinoBattleSide {
    std::string mName;  // 0x00
    struct Creature {
        u32 mId;        // 0x00: the low byte is the creature
        u32 unk04[4];   // 0x04
        u32 unk14;      // 0x14
    } mCreatures[3];    // 0x0C
    u32 unk54;          // 0x54: (the horider level, in wireless battles)
    u32 unk58;          // 0x58
    u32 unk5C[3];       // 0x5C
    struct {
        u32 unk68;      // 0x68
        u32 unk6C;      // 0x6C
    };                  // (copied as a pair)
    u32 mLevels[3];     // 0x70
    u32 unk7C;          // 0x7C
    u32 unk80;          // 0x80
    u32 unk84;          // 0x84
    u8 unk88;           // 0x88
    u8 unk89;           // 0x89
};

// What a battle is set up with.
struct DinoBattleParams {
    u32 unk00;                  // 0x00
    u32 unk04;                  // 0x04
    // (Copied as pairs.)
    struct {
        u32 unk08;              // 0x08: what drives each side: 0 the player, 1 nobody, 2 the CPU
        u32 unk0C;              // 0x0C
    };
    struct {
        u32 unk10;              // 0x10
        u32 unk14;              // 0x14
    };
    u32 unk18;                  // 0x18
    u8 unk1C;                   // 0x1C
    u32 mTutorialId;            // 0x20
    const char *mLiveName;      // 0x24: the commentary's name (none without commentary)
    u32 mScene;                 // 0x28: the sound scene
    u32 unk2C;                  // 0x2C
    DinoBattleSide sides[2];    // 0x30
    u32 unk148;                 // 0x148
    u32 unk14C;                 // 0x14C
    u32 unk150;                 // 0x150
    u32 unk154;                 // 0x154
    u8 unk158;                  // 0x158
    u32 unk15C;                 // 0x15C
    u32 unk160;                 // 0x160
    u8 unk164;                  // 0x164
};

class MMultiPlay;
class DinoBattleModel;
class DinoBattleView;
class DinoBattleTutorial;

class DinoBattle : public MProcess {
public:
    DinoBattle(const DinoBattleParams &params, u32 unk1D4, u32 unk1D8, u8 *unk1DC, u8 *unk1E0);
    virtual ~DinoBattle();
    virtual void Idle();

    u32 GetUnk1F4(int i);
    u32 GetUnk1FC();
    MMessageObserver *GetUnk200();
    void SetUnk1DC(u8 value);
    void SetUnk1E0(u8 value);
    MMultiPlay *GetMultiPlay() { return (MMultiPlay *)mParams.unk04; }
    void StateSetupDisplays();
    void State0215a540();
    void StateRun();
    void StateSoftReset();

    // (unk00: the mode; unk04: the MMultiPlay of a wireless battle; unk08/unk0C: what drives each side, 0 the
    // player, 1 nobody, 2 the CPU; unk1C: post the game-begin signal at the start)
    DinoBattleParams mParams;   // 0x74
    u8 *mUnk1DC;                // 0x1DC: set when the battle ends (if given)
    u8 *mUnk1E0;                // 0x1E0
    u32 mUnk1E4[2];             // 0x1E4
    void (DinoBattle::*mState)();   // 0x1EC
    DinoBattleController *mControllers[2];  // 0x1F4
    DinoBattleModel *mModel;    // 0x1FC
    DinoBattleView *mView;      // 0x200
    u32 mUnk204;                // 0x204
    DinoBattleTutorial *mTutorial;  // 0x208
};

class MInput;

class DinoBattleAutoDemo : public MProcess {
public:
    DinoBattleAutoDemo(MTask *unk7C, u8 *result);
    virtual ~DinoBattleAutoDemo();
    virtual void Idle();

    void StateSetup();
    void StateWatch();
    void StateEnd();
    void StateFadeOut();

    void (DinoBattleAutoDemo::*mState)(); // 0x74
    MTask *mUnk7C;      // 0x7C: the battle
    MInput *mInput;     // 0x80
    u8 *mResult;        // 0x84
};

class DinoBattleDebugSelect : public MProcess {
public:
    DinoBattleDebugSelect(const DinoBattleParams &params, u8 *result);
    virtual ~DinoBattleDebugSelect();
    virtual void Idle();

    void StateCreatureSelect();
    void StateLevelSelect();
    void StateStart();

    u8 mCancelled;              // 0x74: set by the selects
    DinoBattleParams mParams;   // 0x78
    u8 mUnk1E0;                 // 0x1E0: the battle's result
    void (DinoBattleDebugSelect::*mState)(); // 0x1E4
};

#endif
