// The computer opponent's battle controller.
#ifndef GAME_DINO_BATTLE_CPU_CONTROLLER_H
#define GAME_DINO_BATTLE_CPU_CONTROLLER_H

#include "m/resource.h"
#include <vector>
#include "m/archive.h"
#include "game/dino_battle.h"
#include "game/dino_battle_controller.h"
#include "m/fx32.h"

// A loaded file and the archive holding it.
struct DinoBattleFile {
    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

struct DinoBattleFileSet {
    MArchivePtr mArchive;       // 0x00
    MArchiveFile mFile;         // 0x04
    DinoBattleFile mFiles[6];   // 0x10
};

struct DinoOv000DataA;
struct DinoOv000DataB;

struct DinoBattleFileGroup {
    DinoBattleFileSet mSets[5];             // 0x000
    MResource<DinoOv000DataA> mDataA;       // 0x230
    MResource<DinoOv000DataB> mDataB;       // 0x240
};

// A 16-byte entry of a list the computer player keeps (only its size is known; not mUnk930, whose deleter the
// destructor shares with main's vector<void *>).
struct DinoBattleCpuEntry {
    u32 unk[4];
};

class DinoBattleModelPlayer;
class DinoBattleModelCreature;

class DinoBattleCpuController : public DinoBattleController {
public:
    DinoBattleCpuController(void *params, u32 index);
    virtual ~DinoBattleCpuController();
    virtual void OnMessage(MMessage *msg);

    BOOL LeaderHasStatus(DinoBattleModelPlayer *player, u32 bit);
    BOOL SupportHasStatus(DinoBattleModelPlayer *player, u32 bit);
    // The damage the attack would do (at least 1 if it hits).
    int EstimateDamage(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target, u32 attack);
    BOOL CanUseAttack(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature, u32 index);
    BOOL HasAttackUnk10(DinoBattleModelCreature *creature, u8 value);
    BOOL HasCostlyAttackUnk10(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature, u8 value);
    BOOL HasAttackUnk14(DinoBattleModelCreature *creature, u8 value);
    BOOL HasCostlyAttackUnk14(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature, u8 value);
    // The score of the file's conditions on the creature alone (its attacks, its position, the player's data).
    MFx32 ScoreCreatureConditions(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                  DinoBattleFile file);
    // The score of the file's conditions that hold for the creature (against the target).
    MFx32 ScoreConditions(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                          DinoBattleModelCreature *target, DinoBattleFile file);

    u32 mUnkB4;                         // 0xB4
    DinoBattleFileGroup mGroups[3];     // 0xB8
    DinoBattleFile mFiles[2];           // 0x7A8
    u32 mUnk7C8[12];                    // 0x7C8
    DinoBattleSide mSides[2];           // 0x7F8
    u32 mUnk910[8];                     // 0x910
    std::vector<void *> mUnk930;        // 0x930
};

#endif
