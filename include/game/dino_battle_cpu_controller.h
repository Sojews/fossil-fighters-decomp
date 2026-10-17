// The computer opponent's battle controller.
#ifndef GAME_DINO_BATTLE_CPU_CONTROLLER_H
#define GAME_DINO_BATTLE_CPU_CONTROLLER_H

#include <vector>
#include "m/archive.h"
#include "game/dino_battle.h"
#include "game/dino_battle_controller.h"

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

struct DinoBattleFileGroup {
    DinoBattleFileSet mSets[5]; // 0x000
    MArchivePtr mArchive;       // 0x230
    MArchiveFile mFile;         // 0x234
    MArchivePtr mArchive2;      // 0x240
    MArchiveFile mFile2;        // 0x244
};

class DinoBattleCpuController : public DinoBattleController {
public:
    virtual ~DinoBattleCpuController();
    virtual void OnMessage(MMessage *msg);

    u32 mUnkB4;                         // 0xB4
    DinoBattleFileGroup mGroups[3];     // 0xB8
    DinoBattleFile mFiles[2];           // 0x7A8
    u32 mUnk7C8[12];                    // 0x7C8
    DinoBattleSide mSides[2];           // 0x7F8
    u32 mUnk910[8];                     // 0x910
    std::vector<void *> mUnk930;        // 0x930
};

#endif
