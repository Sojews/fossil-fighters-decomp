#ifndef GAME_CREATE_PLAYER_UPDATE_ABILITY_ICON_TASK_H
#define GAME_CREATE_PLAYER_UPDATE_ABILITY_ICON_TASK_H

#include "m/fx32.h"

#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class CreatePlayerUpdateAbilityIconTask : public MTask {
public:
    virtual ~CreatePlayerUpdateAbilityIconTask();
    virtual void Execute();

    u8 mUnk10;          // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
};

class DinoBattleModelPlayer;

class UpdateAllCreatureTask : public MProcess {
public:
    UpdateAllCreatureTask(DinoBattleModelPlayer *player);
    virtual ~UpdateAllCreatureTask();
    virtual void Idle();

    DinoBattleModelPlayer *mPlayer; // 0x74
};

class MoveCreatureFromEscape : public MProcess {
public:
    MoveCreatureFromEscape(DinoBattleModelPlayer *player);
    virtual ~MoveCreatureFromEscape();
    virtual void Idle();

    DinoBattleModelPlayer *mPlayer; // 0x74
};

class DinoBattleModelCreature;
class MRandom;

// One side of the battle in the battle model.
class DinoBattleModelPlayer : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleModelPlayer();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u32 GetUnkB8();
    u32 GetUnk128();
    u32 GetUnk14C();
    u8 GetUnkC8();
    void SetUnkC8(u8 value);
    u32 GetUnk124();
    void SetUnk124(u32 value);
    int GetCreatureCount();
    DinoBattleModelCreature *GetCreature(int index);
    DinoBattleModelCreature *FindCreature(u32 id);
    MTask *CreateMoveToHeatAreaAnyCreature();
    MTask *CreateAddKasekiPointCreatureDie();
    MTask *CreateUpdateAllCreatureTask();
    u32 GetUnkFC();
    MRandom *GetRandom();
    u32 GetUnkAC();
    MFx32 RandomFraction();
    void SetUnk12C(u8 value);
    u32 NextUnkId();
    DinoBattleModelCreature *FindCreatureByB4(u32 id);
    BOOL AnyCreatureUnkD244();
    void CollectCreaturesUnkD244(u32 *ids, int *count);
    BOOL IsUnk2163978();
    void CollectCreaturesB8In12(u32 *ids, int *count);
    BOOL AllCreaturesShareDefFlag();

    u32 mUnkA8;                         // 0xA8
    u32 mUnkAC;                         // 0xAC
    u32 unkB0[2];
    u32 mUnkB8;                         // 0xB8
    u32 mUnkBC;                         // 0xBC
    u32 unkC0[2];
    u8 mUnkC8;                          // 0xC8
    u8 unkC9[0xFC - 0xC9];
    u32 mUnkFC;                         // 0xFC
    u32 unk100;
    DinoBattleModelCreature **mCreatures;   // 0x104
    int mCreatureCount;                 // 0x108
    u32 unk10C;
    MRandom *mRandom;                   // 0x110
    u32 unk114[4];
    u32 mUnk124;                        // 0x124
    u32 mUnk128;                        // 0x128
    u8 mUnk12C;                         // 0x12C
    u8 mUnk12D;                         // 0x12D
    u8 unk12E[2];
    u32 mUnk130[7];                     // 0x130
    u32 mUnk14C;                        // 0x14C
    u32 unk150;
    u32 mKasekiPoint;                   // 0x154
};

// A vivosaur's definition data as the battle reads it (not identified yet).
struct DinoBattleCreatureDef {
    u8 unk00[0xD];
    u8 mUnk0D;          // 0x0D
    u8 unk0E[0x20 - 0x0E];
    u8 mUnk20;          // 0x20
    u8 mUnk21;          // 0x21
    u8 unk22;
    u8 mUnk23;          // 0x23
    u32 mUnk24;         // 0x24: flags
    u8 unk28[0x61 - 0x28];
    u8 mUnk61;          // 0x61
    u8 unk62[0x68 - 0x62];
    s32 mUnk68;         // 0x68: offset of a byte table
};

struct DinoBattleCreatureStats {
    u32 v[6];
};

// A creature's status effects.
struct DinoBattleCreatureStatus {
    u32 flags;          // 0x00
    u8 unk04;
    u8 unk05;           // 0x05
    u8 unk06[0x20 - 0x06];
    u32 unk20;          // 0x20
    u8 unk24[0x40 - 0x24];
};

class DinoBattleModelCreature : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleModelCreature();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u32 GetUnkB0();
    u32 GetUnkB4();
    u32 GetUnkB8();
    u32 GetUnkBC();
    void SetUnkB8(u32 value);
    u32 GetUnkC0();
    void SetUnkC0(u32 value);
    void ClearUnkC4();
    DinoBattleCreatureStats GetStats();
    u32 GetUnkE4();
    u32 GetUnkE8();
    u8 GetUnk174();
    u32 GetUnkEC();
    u32 GetUnkF0();
    u32 GetUnkF4();
    u32 GetUnkF8();
    u32 GetUnkFC();
    u8 GetDefUnk20(BOOL second);
    u8 GetDefUnk61();
    DinoBattleCreatureDef *GetDef();
    void *GetUnk12C();
    u8 GetDefTable(int index);
    u32 GetUnk108();
    void SetUnk108(u32 value);
    void ClearStatus8();
    void ResetStatus();
    BOOL IsUnkD244();
    BOOL IsUnkD274();
    u32 GetStatusUnk20();
    void RollUnk10C();
    MFx32 RandomFraction();
    BOOL IsUnkCE50();
    int GetUnkCE7C(u32 attack);
    int CompareUnkECA0(DinoBattleModelCreature *other, u32 attack);
    int CompareUnkED0C(DinoBattleModelCreature *other);
    BOOL GetUnkD298();
    BOOL RollDefUnk23();
    int GetB8Category();

    class AddLifePointTask : public MTask {
    public:
        virtual ~AddLifePointTask();
        virtual void Execute();
    };

    u32 mUnkA8;                     // 0xA8
    DinoBattleModelPlayer *mPlayer; // 0xAC
    u32 mUnkB0;                     // 0xB0
    u32 mUnkB4;                     // 0xB4
    u32 mUnkB8;                     // 0xB8
    u32 mUnkBC;                     // 0xBC
    u32 mUnkC0;                     // 0xC0
    u32 mUnkC4;                     // 0xC4
    DinoBattleCreatureStats mStats; // 0xC8
    DinoBattleCreatureDef *mDef;    // 0xE0
    u32 mUnkE4;                     // 0xE4
    u32 mUnkE8;                     // 0xE8
    u32 mUnkEC;                     // 0xEC
    u32 mUnkF0;                     // 0xF0
    u32 mUnkF4;                     // 0xF4
    u32 mUnkF8;                     // 0xF8
    u32 mUnkFC;                     // 0xFC
    u32 mUnk100;                    // 0x100
    u32 unk104;
    u32 mUnk108;                    // 0x108
    u32 mUnk10C;                    // 0x10C
    u32 unk110[4];
    std::vector<void *> mUnk120;    // 0x120
    DinoBattleCreatureStatus mStatus;   // 0x12C
    u8 unk16C[0x174 - 0x16C];
    u8 mUnk174;                     // 0x174
    u32 mUnk178;                    // 0x178
};

class ModelCretureAddLifeTask : public MTask {
public:
    virtual ~ModelCretureAddLifeTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class ResetStatusEffectSleepTask : public MTask {
public:
    virtual ~ResetStatusEffectSleepTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
};

class AddLifePointTask : public MTask {
public:
    virtual ~AddLifePointTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u8 mUnk18;          // 0x18
};

class ExecSpecialAttackTask : public MProcess {
public:
    virtual ~ExecSpecialAttackTask();
    virtual void Idle();
};

class SendModelCreatureAbilityResultTask : public MTask {
public:
    virtual ~SendModelCreatureAbilityResultTask();
    virtual void Execute();
};

class SetLifePointTask : public MTask {
public:
    virtual ~SetLifePointTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class PlayerUpdateTask : public MTask {
public:
    virtual ~PlayerUpdateTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class DinoCreatureUpdateStateIconTask : public MTask {
public:
    virtual ~DinoCreatureUpdateStateIconTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
};

class UpdateAbilityIconTask : public MTask {
public:
    virtual ~UpdateAbilityIconTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u8 mUnk18;          // 0x18
};

class SetKasekiPointTask : public MTask {
public:
    virtual ~SetKasekiPointTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class ExecCreatureEndTask : public MProcess {
public:
    virtual ~ExecCreatureEndTask();
    virtual void Idle();

    u32 mUnk74;     // 0x74
    u32 mUnk78;     // 0x78
};

class MoveToHeatAreaAnyCreature : public MProcess {
public:
    MoveToHeatAreaAnyCreature(DinoBattleModelPlayer *player) : MProcess(NULL) { mPlayer = player; }
    virtual ~MoveToHeatAreaAnyCreature();
    virtual void Idle();

    DinoBattleModelPlayer *mPlayer; // 0x74
};

class AddKasekiPointCreatureDie : public MProcess {
public:
    AddKasekiPointCreatureDie(DinoBattleModelPlayer *player) : MProcess(NULL) { mPlayer = player; }
    virtual ~AddKasekiPointCreatureDie();
    virtual void Idle();

    DinoBattleModelPlayer *mPlayer; // 0x74
};

#endif
