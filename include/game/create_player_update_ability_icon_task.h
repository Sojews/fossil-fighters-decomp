#ifndef GAME_CREATE_PLAYER_UPDATE_ABILITY_ICON_TASK_H
#define GAME_CREATE_PLAYER_UPDATE_ABILITY_ICON_TASK_H

#include "m/fx32.h"
#include "game/dino_battle_creature_def.h"

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
    BOOL IsNeeded();

    DinoBattleModelPlayer *mPlayer; // 0x74
};

class MoveCreatureFromEscape : public MProcess {
public:
    MoveCreatureFromEscape(DinoBattleModelPlayer *player);
    virtual ~MoveCreatureFromEscape();
    virtual void Idle();
    BOOL IsNeeded();

    DinoBattleModelPlayer *mPlayer; // 0x74
};

class DinoBattleModelCreature;
struct DinoAttackDef;
class MRandom;

// One side of the battle in the battle model.
class DinoBattle;

struct DinoBattlePlayerEvent {
    DinoBattlePlayerEvent(u32 a, u32 b, u8 c) {
        mUnk0 = a;
        mUnk4 = b;
        mUnk8 = c;
    }

    u32 mUnk0;  // 0x0
    u32 mUnk4;  // 0x4
    u8 mUnk8;   // 0x8
};

// Declared trivial for MSL's containers (as MSL allows), so vectors of it copy with memmove and plain assignment.
namespace Metrowerks {
template <> struct is_trivial<DinoBattlePlayerEvent> {
    static const bool value = true;
};
}

class DinoBattleModelPlayer : public MProcess, public MMessageObserver {
public:
    DinoBattleModelPlayer(u32 side, u32 unkB4);
    virtual ~DinoBattleModelPlayer();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u32 GetUnkB8();
    int GetUnk128();
    u32 GetUnk14C();
    u8 GetUnkC8();
    void SetUnkC8(u8 value);
    u32 GetUnk124();
    void SetUnk124(u32 value);
    int GetCreatureCount();
    DinoBattleModelCreature *GetCreature(int index);
    DinoBattleModelCreature *FindCreature(u32 id);
    MTask *CreateMoveToHeatAreaAnyCreature();
    MTask *CreateUpdateTask(u32 arg);
    MTask *CreateUpdateStateIconTask(u32 creature, u32 arg);
    MTask *CreateSetKasekiPointTask(u32 points);
    void SetKasekiPoints(int points);
    MTask *CreateUpdateAllColorTask(u32 color);
    MTask *CreateUpdateCreaturesTask();
    void AddKasekiPoints(int delta);
    MTask *CreateAddKasekiPointCreatureDie();
    MTask *CreateUpdateAllCreatureTask();
    DinoBattleModelPlayer *GetOpponent();
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
    void SetBattle(DinoBattle *battle);
    void PushEvent(u32 a, u32 b, u8 c);
    DinoBattlePlayerEvent PopEvent(bool remove);
    void RemoveEvents(u32 id);
    void CollectCreaturesNearB8(int pos, u32 *ids, int *count);
    void SendUpdateRequest(u32 arg);
    void SendUpdateAbilityIconRequest(u8 a, u32 b);
    // Sends the view the creature's status icons (none without a creature).
    void SendUpdateStateIcon(DinoBattleModelCreature *creature, u32 reply);
    BOOL IsCpu();
    u8 GetUnk15C();
    void SetUnk15C();
    MTask *CreateSendAbilityResultTask(MMessageObserver *target);
    // Asks the view to wait for the field's effects, waiting for its signal (after a frame).
    MTask *CreateFieldEffectWaitTask();
    MTask *CreateUpdateColorTask(const u32 *ids, u32 count, u32 color);
    // Has the view update the ability icons, and waits for it.
    MTask *CreateUpdateAbilityIconTask(u8 flag);
    MTask *CreateMotionEffectTask(u32 a, u32 b, u32 c, u32 d, u32 e);
    MTask *CreateAbilityTask(u32 a, u32 b, u32 c);
    MTask *CreateLcdFadeTask(BOOL clearIcons);
    DinoBattleModelCreature *GetCreatureInSlot(int slot);
    void ClearEvents();
    void AddAbilitySequence(MTaskSet *set, u32 a, u32 b, u32 attacker, std::vector<void *> &targets, u8 recolor);
    void AddAbilitySequence(MTaskSet *set, u32 a, u32 b, u32 attacker, void *target, u8 recolor);

    std::vector<DinoBattlePlayerEvent> mUnkA8; // 0xA8
    u32 mUnkB4;                         // 0xB4
    u32 mUnkB8;                         // 0xB8: the side
    u32 mUnkBC;                         // 0xBC
    u32 mUnkC0;                         // 0xC0
    u32 unkC4;
    u8 mUnkC8;                          // 0xC8
    MMessageSubject mSubject;           // 0xCC
    MMessageObserver *mUnkF4;           // 0xF4
    DinoBattle *mBattle;                // 0xF8
    DinoBattleModelPlayer *mOpponent;   // 0xFC
    u32 mUnk100;                        // 0x100
    std::vector<DinoBattleModelCreature *> mCreatures; // 0x104
    MRandom *mRandom;                   // 0x110
    std::string mUnk114;                // 0x114
    u32 unk120;
    u32 mUnk124;                        // 0x124
    int mUnk128;                        // 0x128: kaseki points
    u8 mUnk12C;                         // 0x12C
    u8 mUnk12D;                         // 0x12D
    u8 unk12E[2];
    u32 mUnk130[7];                     // 0x130
    u32 mUnk14C;                        // 0x14C
    u32 unk150;
    u32 mKasekiPoint;                   // 0x154
    u32 unk158;
    u8 mUnk15C;                         // 0x15C
    u32 unk160;
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
    u8 unk24[0x2C - 0x24];
    u32 guardChance;    // 0x2C: percentage, with flag 0x100
    u8 unk30[0x40 - 0x30];
};

class DinoBattleModelCreature : public MProcess, public MMessageObserver {
public:
    DinoBattleModelCreature(MMessageObserver *view, DinoBattleModelPlayer *player, u32 id, u32 unkB4, u32 unkB8,
                            DinoBattleCreatureStats stats, s32 unkFC, u32 unk100, u32 unk178);
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
    MMessage *CreateMotionEffectRequest();
    void BeginAction();
    void EndAction();
    void SendUpdate(u8 a, u32 b);
    MTask *CreateUpdateTask(u8 a, u32 b);
    u32 CountAttacks();
    u32 GetAttack(u32 index, int *arg);
    u32 GetSpecialAttack();
    const DinoAttackDef *GetAttackDef(u32 index);
    void AddUnkFC(int delta);
    void SetLife(int life);
    void AddLife(int amount, u8 effect);
    void ClearOthersUnk108();
    void UpdateMarks();
    void ResetUnk108();
    void ChangeUnkB8(u32 value);
    BOOL IsInPinch();
    MFx32 GetUnkECRatio();
    u8 GetUnk174();
    u32 GetUnkEC();
    u32 GetUnkF0();
    u32 GetUnkF4();
    u32 GetUnkF8();
    u32 GetUnkFC();
    u8 GetDefUnk20(BOOL second);
    u8 GetDefUnk61();
    DinoBattleCreatureDef *GetDef();
    DinoBattleCreatureStatus *GetStatus();
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
    MFx32 ReduceByDefense(MFx32 damage, MFx32 defense);
    MFx32 ApplyRatio(MFx32 damage, MFx32 ratio);
    MFx32 ApplyRatio2(MFx32 damage, MFx32 ratio);
    MFx32 CurveRatio(MFx32 a, MFx32 b);
    MFx32 RandomDamageFactor();
    MFx32 CalcDamage(DinoBattleModelCreature *target, MFx32 attack, MFx32 defense, u8 *hit);
    MFx32 ApplyPositionRatio(MFx32 damage, int otherCategory);
    BOOL GetUnkD298();
    BOOL RollDefUnk23();
    int GetB8Category();

    class AddLifePointTask : public MTask {
    public:
        virtual ~AddLifePointTask();
        virtual void Execute();

        u32 mUnk10;     // 0x10
        u32 mUnk14;     // 0x14
        u8 mUnk18;      // 0x18
    };

    MMessageObserver *mView;        // 0xA8
    DinoBattleModelPlayer *mPlayer; // 0xAC
    u32 mUnkB0;                     // 0xB0
    u32 mUnkB4;                     // 0xB4
    u32 mUnkB8;                     // 0xB8
    u32 mUnkBC;                     // 0xBC
    u32 mUnkC0;                     // 0xC0
    u32 mUnkC4;                     // 0xC4
    DinoBattleCreatureStats mStats; // 0xC8
    DinoBattleCreatureDef *mDef;    // 0xE0
    s32 mUnkE4;                     // 0xE4: life
    s32 mUnkE8;                     // 0xE8: maximum life
    u32 mUnkEC;                     // 0xEC
    u32 mUnkF0;                     // 0xF0
    u32 mUnkF4;                     // 0xF4
    u32 mUnkF8;                     // 0xF8
    s32 mUnkFC;                     // 0xFC
    u32 mUnk100;                    // 0x100
    u32 unk104;
    u32 mUnk108;                    // 0x108
    u32 mUnk10C;                    // 0x10C
    u32 unk110[4];
    std::vector<void *> mUnk120;    // 0x120
    DinoBattleCreatureStatus mStatus;   // 0x12C
    u8 unk16C[0x174 - 0x16C];
    u8 mActing;                     // 0x174
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
    SendModelCreatureAbilityResultTask(MMessageObserver *view, MMessageObserver *target) {
        mView = view;
        mTarget = target;
    }
    virtual ~SendModelCreatureAbilityResultTask();
    virtual void Execute();

    MMessageObserver *mView;    // 0x10
    MMessageObserver *mTarget;  // 0x14
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
    PlayerUpdateTask(void *player, u32 arg) {
        mUnk10 = (u32)player;
        mUnk14 = arg;
    }
    virtual ~PlayerUpdateTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class DinoCreatureUpdateStateIconTask : public MTask {
public:
    DinoCreatureUpdateStateIconTask(void *player, u32 creature, u32 arg) {
        mUnk10 = (u32)player;
        mUnk14 = creature;
        mUnk18 = arg;
    }
    virtual ~DinoCreatureUpdateStateIconTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
};

class UpdateAbilityIconTask : public MTask {
public:
    UpdateAbilityIconTask(void *player, MMessageObserver *wait, u8 flag) {
        mUnk10 = (u32)player;
        mUnk14 = (u32)wait;
        mUnk18 = flag;
    }
    virtual ~UpdateAbilityIconTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u8 mUnk18;          // 0x18
};

class SetKasekiPointTask : public MTask {
public:
    SetKasekiPointTask(void *player, u32 points) {
        mUnk10 = (u32)player;
        mUnk14 = points;
    }
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
