// Ov000021558a0.

#include "types.h"
#include "m/resource.h"
#include "game/dino_battle_cpu_controller.h"
#include "game/dino_static_init.h"

// Resources the file loads (their data types are unknown); until the code that constructs them is decompiled,
// these keep out-of-line copies of the constructors.
struct DinoOv000DataA;
struct DinoOv000DataB;
#pragma dont_inline on
MResource<DinoOv000DataA> DinoOv000DataALoad(std::string path) {
    return MResource<DinoOv000DataA>(path);
}
#pragma dont_inline reset
// (mwcc compiles a pending template instance when it reaches the next function, with that function's pragmas: this
// one makes the constructor above get compiled with inlining on.)
void DinoOv000DataAFlush() {
}
#pragma dont_inline on
MResource<DinoOv000DataB> DinoOv000DataBLoad(std::string path) {
    return MResource<DinoOv000DataB>(path);
}
#pragma dont_inline reset

void DinoOv000DataBFlush() {
}

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[4];
    const char *groupPath;  // 0x04: an AI group's file name format
    const char *aiPath;     // 0x08: "/btl_ai/btl_ai_%03d"
} data_ov000_02190fa0;

// The CPU's AI data for the id (none for 0).
extern "C" MResource<DinoOv000DataA> func_ov000_021558a0(int id) {
    if (id == 0) {
        return MResource<DinoOv000DataA>();
    }
    char path[256];
    OS_SPrintf(path, data_ov000_02190fa0.aiPath, id);
    return MResource<DinoOv000DataA>(std::string(path));
}

// An AI group's file: the ids of its five sets, and two more files.
struct DinoOv000DataB {
    u32 unk00;
    u32 unk04;
    u32 sets;       // 0x08: offset of the sets' offsets
    u32 dataA;      // 0x0C
    u32 dataB;      // 0x10
};

// A set of an AI group: its file and six more.
struct DinoBattleCpuAiSet {
    u32 id;         // 0x00
    u32 unk04;
    u32 files;      // 0x08: offset of the six ids
};

// Loads the side's AI: for each of its groups, the group's sets and their files, then the side's two files.
DinoBattleCpuController::DinoBattleCpuController(void *params, u32 index)
    : DinoBattleController(index), mParams(*(const DinoBattleParams *)params) {
    for (u32 k = 0; k < 3; k++) {
        u32 id = mParams.sides[mUnkA8].unk5C[k];
        if (id == 0) {
            continue;
        }
        char path[256];
        OS_SPrintf(path, data_ov000_02190fa0.groupPath, id);
        MResource<DinoOv000DataB> group((std::string(path)));
        for (u32 i = 0; i < 5; i++) {
            u32 j;
            const DinoOv000DataB *data = group.Get();
            const DinoBattleCpuAiSet *set =
                (const DinoBattleCpuAiSet *)((const u8 *)data + ((const u32 *)((const u8 *)data + data->sets))[i]);
            mAi.mGroups[k].mSets[i] = func_ov000_021558a0(set->id);
            for (j = 0; j < 6; j++) {
                mAi.mGroups[k].mSets[i].mFiles[j] = func_ov000_021558a0(((const u32 *)((const u8 *)set + set->files))[j]);
            }
        }
        mAi.mGroups[k].mDataA = func_ov000_021558a0(group.Get()->dataA);
        mAi.mGroups[k].mDataB = func_ov000_021558a0(group.Get()->dataB);
    }
    mAi.mFiles[0] = func_ov000_021558a0(mParams.sides[mUnkA8].unk68);
    mAi.mFiles[1] = func_ov000_021558a0(mParams.sides[mUnkA8].unk6C);
}


// The file's static initialization (see dino_static_init.h).
static struct Ov000021558a0StaticInit {
    Ov000021558a0StaticInit() {
        DINO_POOL_WORD(data_ov000_02190fa0, 0x34) = 128;
        DINO_POOL_WORD(data_ov000_02190fa0, 0x38) = 176;
        DINO_POOL_WORD(data_ov000_02190fa0, 0x14) = 8;
        DINO_POOL_WORD(data_ov000_02190fa0, 0x18) = 150;
        DINO_POOL_WORD(data_ov000_02190fa0, 0x24) = 64;
        DINO_POOL_WORD(data_ov000_02190fa0, 0x28) = 112;
    }
} sOv000021558a0StaticInit;
